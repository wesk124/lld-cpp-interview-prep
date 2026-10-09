#pragma once

#include "lld/parking_lot/types.hpp"

#include "optional.hpp"
#include <stdexcept>
#include <string>
#include <utility>

namespace lld {
namespace parking_lot {

class ParkingSpot {
public:
    ParkingSpot(std::string id, SpotType type) : id_(std::move(id)), type_(type) {
        if (id_.empty() || (type != SpotType::motorcycle && type != SpotType::compact &&
                            type != SpotType::large)) {
            throw std::invalid_argument("spot ID and valid type required");
        }
    }

    const std::string& id() const noexcept { return id_; }
    SpotType type() const noexcept { return type_; }
    bool is_available() const noexcept { return !license_plate_.has_value(); }
    const lld::Optional<std::string>& license_plate() const noexcept {
        return license_plate_;
    }

    bool accepts(VehicleType vehicle_type) const noexcept;

    void occupy(const Vehicle& vehicle) {
        if (vehicle.license_plate.empty() || !is_available() || !accepts(vehicle.type)) {
            throw std::logic_error("spot cannot accept vehicle");
        }
        license_plate_ = vehicle.license_plate;
    }

    void vacate() {
        if (is_available()) {
            throw std::logic_error("cannot vacate an empty spot");
        }
        license_plate_.reset();
    }

private:
    std::string id_;
    SpotType type_;
    lld::Optional<std::string> license_plate_;
};

inline bool ParkingSpot::accepts(VehicleType vehicle_type) const noexcept {
    switch (vehicle_type) {
        case VehicleType::motorcycle:
            return true;
        case VehicleType::car:
            return type_ == SpotType::compact || type_ == SpotType::large;
        case VehicleType::truck:
            return type_ == SpotType::large;
    }
    return false;
}

}  // namespace parking_lot
}  // namespace lld
