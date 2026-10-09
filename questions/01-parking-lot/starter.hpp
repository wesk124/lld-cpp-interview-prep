#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace parking_lot {

[[noreturn]] inline void todo(const char* task) {
    throw std::logic_error(std::string("TODO: ") + task);
}

enum class VehicleType { motorcycle, car, truck };
enum class SpotType { motorcycle, compact, large };
struct Vehicle { std::string license_plate; VehicleType type; };
struct Ticket { long long id; int spot_id; std::string license_plate; int entered_at; };

class ParkingSpot {
public:
    ParkingSpot(int /*id*/, SpotType /*type*/) { todo("store numeric ID, type and occupancy"); }
    int id() const { todo("return the spot ID"); }
    SpotType type() const { todo("return the spot type"); }
    bool available() const { todo("check occupancy"); }
    bool accepts(VehicleType /*type*/) const { todo("implement vehicle compatibility"); }
    void occupy() { todo("mark occupied"); }
    void vacate() { todo("mark available"); }
};
class PricingPolicy {
public:
    virtual ~PricingPolicy() {}
    virtual int fee(int minutes) const = 0;
};
class HourlyPricing : public PricingPolicy {
public:
    explicit HourlyPricing(int /*cents_per_hour*/) { todo("store hourly rate"); }
    int fee(int /*minutes*/) const override { todo("round up with a one-hour minimum"); }
};
class FlatPricing : public PricingPolicy {
public:
    explicit FlatPricing(int /*cents*/) { todo("store flat fee"); }
    int fee(int /*minutes*/) const override { todo("return flat fee"); }
};
class ParkingLot {
public:
    ParkingLot(std::vector<ParkingSpot> /*spots*/, const PricingPolicy& /*pricing*/) {
        todo("own spots and tickets; borrow the pricing Strategy");
    }
    long long park(const Vehicle& /*vehicle*/, int /*now_minutes*/) {
        todo("reject duplicates, choose smallest compatible spot, create numeric ticket");
    }
    int checkout(long long /*ticket_id*/, int /*now_minutes*/) {
        todo("price through Strategy, free spot and remove ticket");
    }
    int available_spots() const { todo("count free spots"); }
};

} // namespace parking_lot
} // namespace lld
