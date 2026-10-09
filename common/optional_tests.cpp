#include "optional.hpp"
#include "test_support.hpp"

#include <memory>
#include <string>
#include <utility>

struct FragileCopy {
    int number;
    bool fail_on_copy;
    FragileCopy(int value, bool fail) : number(value), fail_on_copy(fail) {}
    FragileCopy(const FragileCopy& other)
        : number(other.number), fail_on_copy(other.fail_on_copy) {
        if (fail_on_copy) { throw std::runtime_error("copy failure"); }
    }
};

int main() {
    test::Suite suite;
    suite.run("empty result rejects value access", [] {
        lld::Optional<std::string> empty;
        CHECK(!empty && !empty.has_value());
        EXPECT_THROW(std::logic_error, empty.value());
        EXPECT_THROW(std::logic_error, *empty);
    });
    suite.run("copies own independent snapshots", [] {
        lld::Optional<std::string> original(std::string("ticket"));
        lld::Optional<std::string> copy(original);
        *copy = "receipt";
        CHECK(*original == "ticket" && *copy == "receipt");
        copy = original;
        CHECK(copy->size() == 6);
    });
    suite.run("moves support uniquely owned values", [] {
        lld::Optional<std::unique_ptr<int>> original(std::unique_ptr<int>(new int(7)));
        lld::Optional<std::unique_ptr<int>> moved(std::move(original));
        CHECK(!original && moved && **moved == 7);
        lld::Optional<std::unique_ptr<int>> assigned;
        assigned = std::move(moved);
        CHECK(!moved && assigned && **assigned == 7);
    });
    suite.run("reset releases the owned value", [] {
        auto owner = std::make_shared<int>(7);
        std::weak_ptr<int> observer(owner);
        lld::Optional<std::shared_ptr<int>> result(owner);
        owner.reset();
        CHECK(!observer.expired());
        result.reset();
        CHECK(!result && observer.expired());
    });
    suite.run("failed assignment preserves the previous snapshot", [] {
        FragileCopy source(9, true);
        lld::Optional<FragileCopy> destination(FragileCopy(4, false));
        EXPECT_THROW(std::runtime_error, destination = source);
        CHECK(destination->number == 4);
    });
    suite.run("empty assignment and reuse", [] {
        lld::Optional<int> result(5);
        result = lld::Optional<int>();
        CHECK(!result);
        result = 8;
        const auto& snapshot = result;
        CHECK(snapshot.value() == 8);
    });
    return suite.finish();
}
