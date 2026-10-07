#pragma once
#include <chrono>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace lld::parking_lot {
using TimePoint = std::chrono::system_clock::time_point;
enum class VehicleType { motorcycle, car, truck };
enum class SpotType { motorcycle, compact, large };
struct Vehicle { std::string license_plate; VehicleType type; };
struct Ticket { std::string id; std::string license_plate; std::string spot_id; TimePoint entered_at; };
struct Receipt {
    std::string ticket_id; std::string license_plate; std::string spot_id;
    TimePoint entered_at; TimePoint exited_at; int charged_hours; int fee_cents;
};
struct PricingQuote { int charged_hours; int fee_cents; };

[[noreturn]] inline void todo(const char* task) { throw std::logic_error(std::string("TODO: ") + task); }

class PricingPolicy {
public:
    virtual ~PricingPolicy() = default;
    virtual PricingQuote calculate(TimePoint entered_at, TimePoint exited_at) const = 0;
};
class HourlyPricingPolicy final : public PricingPolicy {
public:
    explicit HourlyPricingPolicy(int /*cents_per_hour*/) { todo("validate and retain the hourly rate"); }
    PricingQuote calculate(TimePoint /*entered_at*/, TimePoint /*exited_at*/) const override {
        todo("round up, enforce a one-hour minimum, and prevent fee overflow");
    }
};
class ParkingSpot {
public:
    ParkingSpot(std::string /*id*/, SpotType /*type*/) { todo("store spot identity and category"); }
    // TODO: Add compatibility and occupancy transitions; keep ownership explicit.
};
class ParkingLot {
public:
    ParkingLot(std::vector<ParkingSpot> /*spots*/, std::unique_ptr<PricingPolicy> /*policy*/) {
        todo("own spots and a pricing strategy; reject invalid configurations");
    }
    std::optional<Ticket> park(const Vehicle& /*vehicle*/, TimePoint /*entered_at*/) {
        todo("allocate atomically, reject duplicate plates, and create a stable ticket");
    }
    std::optional<Receipt> exit(const std::string& /*ticket_id*/, TimePoint /*exited_at*/) {
        todo("validate and price before atomically releasing the session");
    }
    std::size_t available_spots() const { todo("return a synchronized capacity snapshot"); }
private:
    // TODO: Define owned state, indexes, ticket sequence, and the lock boundary.
};
}  // namespace lld::parking_lot
