#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"
#include <atomic>
#include <thread>
#include <vector>

using namespace lld::inventory_management;

int main() {
    test::Suite suite;
    suite.run("stock receipt and availability", [] {
        Inventory inventory;
        inventory.add_sku("book", 3);
        inventory.receive("book", 2);
        CHECK(inventory.stock("book").available() == 5);
    });
    suite.run("multi-SKU reservation is atomic", [] {
        Inventory inventory;
        inventory.add_sku("a", 3);
        inventory.add_sku("b", 1);
        CHECK(!inventory.reserve("bad", {{"a", 2}, {"b", 2}}));
        CHECK(inventory.stock("a").reserved == 0);
        CHECK(inventory.stock("b").reserved == 0);
        CHECK(inventory.reserve("good", {{"a", 2}, {"b", 1}}));
        CHECK(inventory.stock("a").available() == 1);
    });
    suite.run("reservation and commit retries are idempotent", [] {
        Inventory inventory;
        inventory.add_sku("a", 5);
        CHECK(inventory.reserve("order", {{"a", 2}}));
        CHECK(inventory.reserve("order", {{"a", 2}}));
        CHECK(inventory.stock("a").reserved == 2);
        CHECK(inventory.commit("order"));
        CHECK(inventory.commit("order"));
        CHECK(inventory.reserve("order", {{"a", 2}}));
        CHECK(inventory.stock("a").on_hand == 3 && inventory.stock("a").reserved == 0);
        CHECK(!inventory.release("order"));
    });
    suite.run("release is idempotent and prevents commit", [] {
        Inventory inventory;
        inventory.add_sku("a", 2);
        CHECK(inventory.reserve("order", {{"a", 2}}));
        CHECK(inventory.release("order") && inventory.release("order"));
        CHECK(inventory.stock("a").available() == 2);
        CHECK(!inventory.commit("order"));
        CHECK(!inventory.reserve("order", {{"a", 2}}));
    });
    suite.run("order identity cannot change request payload", [] {
        Inventory inventory;
        inventory.add_sku("a", 5);
        CHECK(inventory.reserve("order", {{"a", 1}}));
        EXPECT_THROW(std::invalid_argument, inventory.reserve("order", {{"a", 2}}));
        CHECK(inventory.stock("a").reserved == 1);
    });
    suite.run("unknown SKU and order handling", [] {
        Inventory inventory;
        CHECK(!inventory.reserve("order", {{"missing", 1}}));
        CHECK(!inventory.commit("missing") && !inventory.release("missing"));
        EXPECT_THROW(std::out_of_range, inventory.stock("missing"));
    });
    suite.run("invalid quantities and overflow", [] {
        Inventory inventory;
        inventory.add_sku("a", 1);
        EXPECT_THROW(std::invalid_argument, inventory.reserve("order", {{"a", 0}}));
        EXPECT_THROW(std::invalid_argument, inventory.receive("a", -1));
        inventory.add_sku("full", std::numeric_limits<int>::max());
        EXPECT_THROW(std::overflow_error, inventory.receive("full", 1));
    });
    suite.run("concurrent orders cannot reserve the last item twice", [] {
        Inventory inventory;
        inventory.add_sku("a", 1);
        std::atomic<int> accepted{0};
        std::vector<std::thread> threads;
        for (int i = 0; i < 8; ++i) {
            threads.emplace_back([&, i] {
                if (inventory.reserve(std::to_string(i), {{"a", 1}})) { ++accepted; }
            });
        }
        for (auto& thread : threads) { thread.join(); }
        CHECK(accepted == 1 && inventory.stock("a").available() == 0);
    });
    return suite.finish();
}
