#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace lld {
namespace parking_lot {

enum class VehicleType { motorcycle, car, truck };
enum class SpotType { motorcycle, compact, large };

struct Vehicle {
    std::string license_plate;
    VehicleType type;
};

class ParkingSpot {
public:
    ParkingSpot(int id, SpotType type) : id_(id), type_(type), occupied_(false) {}
    int id() const { return id_; }
    bool available() const { return !occupied_; }
    void occupy() { occupied_ = true; }
    void vacate() { occupied_ = false; }

    bool accepts(VehicleType type) const {
        if (type == VehicleType::motorcycle) return true;
        if (type == VehicleType::car) return type_ != SpotType::motorcycle;
        return type_ == SpotType::large;
    }

    SpotType type() const { return type_; }

private:
    int id_;
    SpotType type_;
    bool occupied_;
};

// Strategy: ParkingLot delegates fee calculation to this interface.
class PricingPolicy {
public:
    virtual ~PricingPolicy() {}
    virtual int fee(int minutes) const = 0;
};

class HourlyPricing : public PricingPolicy {
public:
    explicit HourlyPricing(int cents_per_hour) : rate_(cents_per_hour) {}
    int fee(int minutes) const override {
        return std::max(1, minutes / 60 + (minutes % 60 != 0)) * rate_;
    }
private:
    int rate_;
};

class FlatPricing : public PricingPolicy {
public:
    explicit FlatPricing(int cents) : cents_(cents) {}
    int fee(int /*minutes*/) const override { return cents_; }
private:
    int cents_;
};

struct Ticket {
    long long id;
    int spot_id;
    std::string license_plate;
    int entered_at;
};

class ParkingLot {
public:
    ParkingLot(std::vector<ParkingSpot> spots, const PricingPolicy& pricing)
        : spots_(std::move(spots)), pricing_(pricing), next_id_(1) {}

    long long park(const Vehicle& vehicle, int now_minutes) {
        if (vehicle.license_plate.empty() || now_minutes < 0) return -1;
        for (const auto& entry : tickets_)
            if (entry.second.license_plate == vehicle.license_plate) return -1;

        ParkingSpot* best = nullptr;
        for (auto& spot : spots_)
            if (spot.available() && spot.accepts(vehicle.type) &&
                (!best || spot.type() < best->type())) best = &spot;

        if (!best) return -1;
        long long id = next_id_++;
        tickets_.emplace(id, Ticket{id, best->id(), vehicle.license_plate, now_minutes});
        best->occupy();
        return id;
    }

    int checkout(long long ticket_id, int now_minutes) {
        auto ticket = tickets_.find(ticket_id);
        if (ticket == tickets_.end() || now_minutes < ticket->second.entered_at) return -1;
        int cents = pricing_.fee(now_minutes - ticket->second.entered_at);
        for (auto& spot : spots_)
            if (spot.id() == ticket->second.spot_id) { spot.vacate(); break; }
        tickets_.erase(ticket);
        return cents;
    }

    int available_spots() const {
        int count = 0;
        for (const auto& spot : spots_) if (spot.available()) ++count;
        return count;
    }

private:
    std::vector<ParkingSpot> spots_;
    const PricingPolicy& pricing_; // Caller keeps this policy alive.
    std::unordered_map<long long, Ticket> tickets_;
    long long next_id_;
};

} // namespace parking_lot
} // namespace lld
