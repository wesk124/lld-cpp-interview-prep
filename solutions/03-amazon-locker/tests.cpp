#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"

using namespace lld::amazon_locker;

int main() {
    test::Suite suite;
    suite.run("smallest compatible allocation", [] {
        SmallestFit allocation;
        Locker locker({Slot(1, Size::large), Slot(2, Size::small)}, allocation);
        int small = locker.deposit(100, Size::small);
        CHECK(small > 0);
        CHECK(locker.deposit(101, Size::large) > 0);
        CHECK(locker.available_slots() == 0);
        CHECK(locker.deposit(102, Size::small) == -1);
    });
    suite.run("single-use pickup and reusable slots", [] {
        SmallestFit allocation;
        Locker locker({Slot(1, Size::medium)}, allocation);
        int code = locker.deposit(100, Size::medium);
        CHECK(locker.pickup(code) == 100);
        CHECK(locker.pickup(code) == -1);
        CHECK(locker.available_slots() == 1);
        int new_code = locker.deposit(101, Size::small);
        CHECK(new_code != code);
        CHECK(locker.pickup(new_code) == 101);
    });
    suite.run("duplicates and incompatible packages", [] {
        SmallestFit allocation;
        Locker locker({Slot(1, Size::small), Slot(2, Size::small)}, allocation);
        CHECK(locker.deposit(100, Size::large) == -1);
        CHECK(locker.deposit(100, Size::small) > 0);
        CHECK(locker.deposit(100, Size::small) == -1);
        CHECK(locker.available_slots() == 1);
    });
    suite.run("allocation Strategy interface", [] {
        SmallestFit smallest;
        const AllocationPolicy& policy = smallest;
        CHECK(policy.choose({Slot(1, Size::large), Slot(2, Size::small)}, Size::small) == 1);
        CHECK(policy.choose({Slot(1, Size::small)}, Size::large) == -1);
    });
    return suite.finish();
}
