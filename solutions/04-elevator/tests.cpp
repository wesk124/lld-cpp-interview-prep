#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"

using namespace lld::elevator;

int main() {
    test::Suite suite;
    suite.run("doors close before the next movement", [] {
        Elevator car(10);
        car.request_stop(2); car.request_stop(4);
        car.step(); CHECK(car.floor() == 1);
        car.step(); CHECK(car.floor() == 2 && car.door_open());
        car.step(); CHECK(car.floor() == 2 && !car.door_open());
        car.step(); CHECK(car.floor() == 3);
        car.step(); CHECK(car.floor() == 4 && car.door_open());
    });
    suite.run("LOOK continues then reverses", [] {
        Elevator car(10, 4);
        car.request_stop(8); car.step(); car.request_stop(2);
        CHECK(car.floor() == 5);
        car.step(); car.step(); car.step();
        CHECK(car.floor() == 8 && car.door_open());
        car.step(); car.step();
        CHECK(car.floor() == 7 && car.direction() == Direction::down);
        for (int tick = 0; tick < 10; ++tick) car.step();
        CHECK(car.floor() == 2 && car.pending() == 0);
    });
    suite.run("dispatch Strategy substitutes", [] {
        NearestCar nearest;
        LeastBusyCar least_busy;
        ElevatorBank nearby(10, {0, 8}, nearest);
        CHECK(nearby.request(2) == 0);
        ElevatorBank workload(10, {0, 8}, least_busy);
        workload.select_floor(0, 9);
        CHECK(workload.request(2) == 1);
    });
    suite.run("coalesce requests and reject invalid floors", [] {
        Elevator car(10);
        car.request_stop(2); car.request_stop(2);
        CHECK(car.pending() == 1);
        EXPECT_THROW(std::out_of_range, car.request_stop(11));
        CHECK(car.pending() == 1);
    });
    return suite.finish();
}
