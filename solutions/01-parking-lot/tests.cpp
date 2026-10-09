#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"

using namespace lld::parking_lot;

int main() {
    test::Suite suite;
    suite.run("numeric IDs, capacity and checkout", [] {
        HourlyPricing pricing(500);
        ParkingLot lot({ParkingSpot(1, SpotType::large)}, pricing);
        long long ticket = lot.park(Vehicle{"ABC123", VehicleType::car}, 0);
        CHECK(ticket == 1);
        CHECK(lot.available_spots() == 0);
        CHECK(lot.park(Vehicle{"XYZ", VehicleType::truck}, 0) == -1);
        CHECK(lot.checkout(ticket, 61) == 1000);
        CHECK(lot.available_spots() == 1);
        CHECK(lot.checkout(ticket, 62) == -1);
    });
    suite.run("smallest compatible spot", [] {
        HourlyPricing pricing(500);
        ParkingLot lot({ParkingSpot(1, SpotType::large), ParkingSpot(2, SpotType::compact)}, pricing);
        CHECK(lot.park(Vehicle{"CAR", VehicleType::car}, 0) > 0);
        CHECK(lot.park(Vehicle{"TRUCK", VehicleType::truck}, 0) > 0);
    });
    suite.run("invalid requests leave state unchanged", [] {
        FlatPricing pricing(700);
        ParkingLot lot({ParkingSpot(1, SpotType::compact)}, pricing);
        CHECK(lot.park(Vehicle{"", VehicleType::car}, 0) == -1);
        long long ticket = lot.park(Vehicle{"CAR", VehicleType::car}, 10);
        CHECK(lot.park(Vehicle{"CAR", VehicleType::car}, 10) == -1);
        CHECK(lot.checkout(ticket, 9) == -1);
        CHECK(lot.available_spots() == 0);
        CHECK(lot.checkout(ticket, 10) == 700);
    });
    suite.run("pricing Strategy substitutes without changing the lot", [] {
        HourlyPricing hourly(500);
        FlatPricing flat(700);
        CHECK(hourly.fee(0) == 500);
        CHECK(hourly.fee(60) == 500);
        CHECK(hourly.fee(61) == 1000);
        ParkingLot lot({ParkingSpot(1, SpotType::compact)}, flat);
        CHECK(lot.checkout(lot.park(Vehicle{"CAR", VehicleType::car}, 0), 180) == 700);
    });
    return suite.finish();
}
