#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"
#include <utility>

using namespace lld::inventory_management;

int main() {
    test::Suite suite;
    suite.run("reserve, commit and idempotent retries", [] {
        Inventory inventory;
        CHECK(inventory.add_sku("A", 10));
        CHECK(inventory.reserve(1, {{"A", 4}}));
        CHECK(inventory.reserve(1, {{"A", 4}}));
        CHECK(inventory.stock("A").reserved == 4);
        CHECK(inventory.commit(1));
        CHECK(inventory.commit(1));
        CHECK(inventory.stock("A").on_hand == 6);
        CHECK(inventory.stock("A").reserved == 0);
        CHECK(!inventory.release(1));
    });
    suite.run("whole-order validation and release", [] {
        Inventory inventory;
        inventory.add_sku("A", 10); inventory.add_sku("B", 1);
        CHECK(!inventory.reserve(1, {{"A", 2}, {"B", 2}}));
        CHECK(inventory.stock("A").reserved == 0);
        CHECK(inventory.reserve(2, {{"A", 2}}));
        CHECK(!inventory.reserve(2, {{"A", 3}}));
        CHECK(inventory.release(2));
        CHECK(inventory.release(2));
        CHECK(inventory.stock("A").available() == 10);
        CHECK(!inventory.reserve(2, {{"A", 2}}));
    });
    suite.run("low-stock Observer and unsubscribe", [] {
        class Alerts : public StockObserver {
        public:
            void on_low_stock(const std::string& sku, int available) override {
                values.emplace_back(sku, available);
            }
            std::vector<std::pair<std::string, int>> values;
        };
        Inventory inventory(3);
        Alerts alerts;
        inventory.subscribe(alerts);
        inventory.add_sku("A", 10);
        CHECK(inventory.reserve(1, {{"A", 8}}));
        CHECK(alerts.values.size() == 1);
        CHECK(alerts.values[0].first == "A" && alerts.values[0].second == 2);
        CHECK(inventory.reserve(1, {{"A", 8}}));
        CHECK(alerts.values.size() == 1);
        inventory.unsubscribe(alerts);
        CHECK(inventory.reserve(2, {{"A", 1}}));
        CHECK(alerts.values.size() == 1);
    });
    suite.run("basic catalog validation", [] {
        Inventory inventory;
        CHECK(inventory.add_sku("A", 1));
        CHECK(!inventory.add_sku("A", 2));
        CHECK(!inventory.add_sku("B", -1));
        CHECK(!inventory.reserve(1, {{"A", -1}}));
        CHECK(inventory.receive("A", 2));
        CHECK(inventory.stock("A").on_hand == 3);
        CHECK(!inventory.receive("missing", 2));
        CHECK(!inventory.commit(999));
    });
    return suite.finish();
}
