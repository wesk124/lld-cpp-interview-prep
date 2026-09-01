#include "lld/parking_lot/parking_lot.hpp"

#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {

using namespace lld::parking_lot;
using namespace std::chrono_literals;

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}

ParkingLot make_lot() {
    std::vector<ParkingSpot> spots;
    spots.emplace_back("M-1", SpotType::motorcycle);
    spots.emplace_back("C-1", SpotType::compact);
    spots.emplace_back("L-1", SpotType::large);
    return ParkingLot(std::move(spots), std::make_unique<HourlyPricingPolicy>(500));
}

void parks_in_smallest_compatible_spot() {
    auto lot = make_lot();
    const TimePoint now{};

    const auto motorcycle = lot.park({"MOTO", VehicleType::motorcycle}, now);
    const auto car = lot.park({"CAR", VehicleType::car}, now);
    const auto truck = lot.park({"TRUCK", VehicleType::truck}, now);

    expect(motorcycle && motorcycle->spot_id == "M-1", "motorcycle should use motorcycle spot");
    expect(car && car->spot_id == "C-1", "car should use compact spot");
    expect(truck && truck->spot_id == "L-1", "truck should use large spot");
    expect(lot.available_spots() == 0, "all spots should be occupied");
}

void rejects_when_no_compatible_spot_exists() {
    auto lot = make_lot();
    const TimePoint now{};

    expect(lot.park({"TRUCK-1", VehicleType::truck}, now).has_value(), "first truck should park");
    expect(!lot.park({"TRUCK-2", VehicleType::truck}, now).has_value(),
           "second truck should be rejected");
}

void rejects_duplicate_active_vehicle() {
    auto lot = make_lot();
    const TimePoint now{};

    expect(lot.park({"DUP", VehicleType::car}, now).has_value(), "vehicle should park once");
    expect(!lot.park({"DUP", VehicleType::car}, now).has_value(),
           "same plate should not receive two active tickets");
}

void exit_rounds_up_fee_and_releases_spot() {
    auto lot = make_lot();
    const TimePoint entered{};
    const auto ticket = lot.park({"CAR", VehicleType::car}, entered);
    expect(ticket.has_value(), "car should park");

    const auto receipt = lot.exit(ticket->id, entered + 61min);
    expect(receipt.has_value(), "active ticket should exit");
    expect(receipt->charged_hours == 2, "61 minutes should round to two hours");
    expect(receipt->fee_cents == 1000, "two hours should cost 1000 cents");
    expect(lot.available_spots() == 3, "checkout should release the spot");
    expect(!lot.exit(ticket->id, entered + 62min).has_value(), "ticket should be single-use");
}

void charges_at_least_one_hour() {
    auto lot = make_lot();
    const TimePoint entered{};
    const auto ticket = lot.park({"QUICK", VehicleType::motorcycle}, entered);
    expect(ticket.has_value(), "motorcycle should park");

    const auto receipt = lot.exit(ticket->id, entered);
    expect(receipt && receipt->charged_hours == 1, "zero elapsed time should charge one hour");
    expect(receipt && receipt->fee_cents == 500, "minimum charge should use hourly rate");
}

void rejects_exit_before_entry_without_mutating_state() {
    auto lot = make_lot();
    const TimePoint entered = TimePoint{} + 1h;
    const auto ticket = lot.park({"TIME", VehicleType::car}, entered);
    expect(ticket.has_value(), "car should park");

    bool threw = false;
    try {
        static_cast<void>(lot.exit(ticket->id, entered - 1min));
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    expect(threw, "invalid time should be rejected");
    expect(lot.available_spots() == 2, "failed checkout must not release the spot");
    expect(lot.exit(ticket->id, entered + 1min).has_value(), "ticket should remain active");
}

}  // namespace

int main() {
    parks_in_smallest_compatible_spot();
    rejects_when_no_compatible_spot_exists();
    rejects_duplicate_active_vehicle();
    exit_rounds_up_fee_and_releases_spot();
    charges_at_least_one_hour();
    rejects_exit_before_entry_without_mutating_state();
    std::cout << "All parking lot tests passed\n";
    return EXIT_SUCCESS;
}
