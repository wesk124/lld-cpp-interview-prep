#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"

using namespace lld::elevator;

int main() {
    test::Suite suite;
    suite.run("idle elevator remains stationary", [] {
        Elevator car(10);
        CHECK(car.step().floor == 0);
        CHECK(car.snapshot().direction == Direction::idle);
    });
    suite.run("one floor per tick and door interlock", [] {
        Elevator car(10);
        car.request_stop(2);
        CHECK(car.step().floor == 1);
        auto arrived = car.step();
        CHECK(arrived.floor == 2 && arrived.door == Door::open);
        car.request_stop(4);
        auto closed = car.step();
        CHECK(closed.floor == 2 && closed.door == Door::closed);
        CHECK(car.step().floor == 3);
    });
    suite.run("LOOK finishes upward requests before reversing", [] {
        Elevator car(10, 3);
        car.request_stop(6);
        CHECK(car.step().floor == 4);
        car.request_stop(1);
        car.request_stop(5);
        std::vector<int> served;
        for (int i = 0; i < 20; ++i) {
            auto state = car.step();
            if (state.door == Door::open) { served.push_back(state.floor); }
            if (state.pending.empty() && state.door == Door::closed) { break; }
        }
        const std::vector<int> expected{5, 6, 1};
        CHECK(served == expected);
    });
    suite.run("coalesced request at current floor", [] {
        Elevator car(10, 3);
        car.request_stop(3);
        car.request_stop(3);
        CHECK(car.snapshot().pending.size() == 1);
        CHECK(car.step().door == Door::open);
        car.request_stop(3);
        CHECK(car.snapshot().pending.empty());
    });
    suite.run("nearest-car dispatch and internal button", [] {
        ElevatorBank bank(10, {0, 8}, std::make_unique<NearestCarPolicy>());
        CHECK(bank.request(7, Direction::down) == 1);
        auto state = bank.step_all();
        CHECK(state[1].floor == 7 && state[1].door == Door::open);
        bank.select_floor(1, 2);
        CHECK(bank.snapshots()[1].pending.front() == 2);
    });
    suite.run("nearest-car ties use smallest index", [] {
        ElevatorBank bank(10, {2, 6}, std::make_unique<NearestCarPolicy>());
        CHECK(bank.request(4, Direction::up) == 0);
    });
    suite.run("invalid floors and hall directions", [] {
        EXPECT_THROW(std::invalid_argument, Elevator(0));
        Elevator car(10);
        EXPECT_THROW(std::out_of_range, car.request_stop(11));
        ElevatorBank bank(10, {0}, std::make_unique<NearestCarPolicy>());
        EXPECT_THROW(std::invalid_argument, bank.request(0, Direction::down));
        EXPECT_THROW(std::out_of_range, bank.select_floor(4, 3));
    });
    suite.run("downward sweep stays within bounds", [] {
        Elevator car(10, 10);
        car.request_stop(0);
        for (int i = 0; i < 10; ++i) { car.step(); }
        CHECK(car.snapshot().floor == 0 && car.snapshot().door == Door::open);
        CHECK(car.step().floor == 0);
    });
    return suite.finish();
}
