#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "lld/parking_lot/parking_lot.hpp"
#endif
#include "test_support.hpp"
#include <atomic>
#include <chrono>
#include <limits>
#include <thread>

using namespace lld::parking_lot;

std::unique_ptr<ParkingLot> make_lot() {
    std::vector<ParkingSpot> spots{ParkingSpot("M-1", SpotType::motorcycle),
                                   ParkingSpot("C-1", SpotType::compact),
                                   ParkingSpot("L-1", SpotType::large)};
    return std::unique_ptr<ParkingLot>(new ParkingLot(
        std::move(spots), std::unique_ptr<PricingPolicy>(new HourlyPricingPolicy(500))));
}

class FlatPricing final : public PricingPolicy {
public:
    PricingQuote calculate(TimePoint, TimePoint) const override { return PricingQuote{0, 200}; }
};
class FailingPricing final : public PricingPolicy {
public:
    PricingQuote calculate(TimePoint, TimePoint) const override { throw std::runtime_error("pricing failure"); }
};

int main() {
    test::Suite suite;
    suite.run("smallest compatible allocation", [] {
        auto owner = make_lot();
        auto& lot = *owner;
        auto motorcycle = lot.park({"m", VehicleType::motorcycle}, TimePoint{});
        auto car = lot.park({"c", VehicleType::car}, TimePoint{});
        auto truck = lot.park({"t", VehicleType::truck}, TimePoint{});
        CHECK(motorcycle && motorcycle->spot_id == "M-1");
        CHECK(car && car->spot_id == "C-1");
        CHECK(truck && truck->spot_id == "L-1");
        CHECK(lot.available_spots() == 0);
    });
    suite.run("larger spot fallback and compatible capacity", [] {
        auto owner = make_lot();
        auto& lot = *owner;
        CHECK(lot.park({"c1", VehicleType::car}, TimePoint{}));
        auto second = lot.park({"c2", VehicleType::car}, TimePoint{});
        CHECK(second && second->spot_id == "L-1");
        CHECK(!lot.park({"t", VehicleType::truck}, TimePoint{}));
        CHECK(lot.available_spots() == 1);
    });
    suite.run("duplicate active plate rejected", [] {
        auto owner = make_lot();
        auto& lot = *owner;
        CHECK(lot.park({"same", VehicleType::car}, TimePoint{}));
        CHECK(!lot.park({"same", VehicleType::car}, TimePoint{}));
    });
    suite.run("partial-hour billing, release, and single-use ticket", [] {
        auto owner = make_lot();
        auto& lot = *owner;
        auto ticket = lot.park({"car", VehicleType::car}, TimePoint{});
        CHECK(ticket);
        auto receipt = lot.exit(ticket->id, TimePoint{} + std::chrono::minutes(61));
        CHECK(receipt && receipt->charged_hours == 2 && receipt->fee_cents == 1000);
        CHECK(lot.available_spots() == 3);
        CHECK(!lot.exit(ticket->id, TimePoint{} + std::chrono::minutes(62)));
        CHECK(lot.park({"car", VehicleType::car}, TimePoint{} + std::chrono::minutes(62)));
    });
    suite.run("minimum and exact-hour billing", [] {
        HourlyPricingPolicy policy(500);
        CHECK(policy.calculate(TimePoint{}, TimePoint{}).fee_cents == 500);
        CHECK(policy.calculate(TimePoint{}, TimePoint{} + std::chrono::hours(1)).fee_cents == 500);
        CHECK(policy.calculate(TimePoint{}, TimePoint{} + std::chrono::hours(1) + TimePoint::duration{1}).fee_cents == 1000);
    });
    suite.run("invalid checkout time leaves session active", [] {
        auto owner = make_lot();
        auto& lot = *owner;
        auto ticket = lot.park({"car", VehicleType::car}, TimePoint{} + std::chrono::hours(1));
        CHECK(ticket);
        EXPECT_THROW(std::invalid_argument, lot.exit(ticket->id, TimePoint{}));
        CHECK(lot.available_spots() == 2);
        CHECK(lot.exit(ticket->id, TimePoint{} + std::chrono::hours(2)));
    });
    suite.run("pricing strategy does not alter allocation", [] {
        ParkingLot lot({ParkingSpot("a", SpotType::large)}, std::unique_ptr<PricingPolicy>(new FlatPricing()));
        auto ticket = lot.park({"truck", VehicleType::truck}, TimePoint{});
        CHECK(ticket);
        auto receipt = lot.exit(ticket->id, TimePoint{} + std::chrono::hours(10));
        CHECK(receipt && receipt->fee_cents == 200);
    });
    suite.run("pricing failure cannot release a spot", [] {
        ParkingLot lot({ParkingSpot("a", SpotType::large)}, std::unique_ptr<PricingPolicy>(new FailingPricing()));
        auto ticket = lot.park({"truck", VehicleType::truck}, TimePoint{});
        CHECK(ticket);
        EXPECT_THROW(std::runtime_error, lot.exit(ticket->id, TimePoint{} + std::chrono::hours(1)));
        CHECK(lot.available_spots() == 0);
        CHECK(!lot.park({"truck", VehicleType::truck}, TimePoint{}));
    });
    suite.run("constructor and vehicle validation", [] {
        EXPECT_THROW(std::invalid_argument,
                     ParkingLot({ParkingSpot("same", SpotType::large), ParkingSpot("same", SpotType::large)},
                                std::unique_ptr<PricingPolicy>(new HourlyPricingPolicy(500))));
        auto owner = make_lot();
        auto& lot = *owner;
        EXPECT_THROW(std::invalid_argument, lot.park({"", VehicleType::car}, TimePoint{}));
        CHECK(!lot.exit("unknown", TimePoint{}));
    });
    suite.run("fee and duration overflow are rejected", [] {
        HourlyPricingPolicy policy(std::numeric_limits<int>::max());
        EXPECT_THROW(std::overflow_error, policy.calculate(TimePoint{}, TimePoint{} + std::chrono::hours(2)));
        EXPECT_THROW(std::overflow_error, policy.calculate(TimePoint::min(), TimePoint::max()));
    });
    suite.run("concurrent parks cannot overfill", [] {
        ParkingLot lot({ParkingSpot("a", SpotType::large)}, std::unique_ptr<PricingPolicy>(new HourlyPricingPolicy(500)));
        std::atomic<int> accepted{0};
        std::vector<std::thread> threads;
        for (int i = 0; i < 8; ++i) {
            threads.emplace_back([&, i] {
                if (lot.park({std::to_string(i), VehicleType::truck}, TimePoint{})) { ++accepted; }
            });
        }
        for (auto& thread : threads) { thread.join(); }
        CHECK(accepted == 1 && lot.available_spots() == 0);
    });
    return suite.finish();
}
