#pragma once

#include "lld/parking_lot/parking_spot.hpp"
#include "lld/parking_lot/pricing_policy.hpp"
#include "lld/parking_lot/types.hpp"

#include <cstddef>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace lld::parking_lot {

class ParkingLot {
public:
    ParkingLot(std::vector<ParkingSpot> spots, std::unique_ptr<PricingPolicy> pricing_policy);

    ParkingLot(const ParkingLot&) = delete;
    ParkingLot& operator=(const ParkingLot&) = delete;

    [[nodiscard]] std::optional<Ticket> park(const Vehicle& vehicle, TimePoint entered_at);
    [[nodiscard]] std::optional<Receipt> exit(const std::string& ticket_id, TimePoint exited_at);
    [[nodiscard]] std::size_t available_spots() const;

private:
    [[nodiscard]] ParkingSpot* find_best_spot(VehicleType vehicle_type);

    mutable std::mutex mutex_;
    std::vector<ParkingSpot> spots_;
    std::unique_ptr<PricingPolicy> pricing_policy_;
    std::unordered_map<std::string, Ticket> active_tickets_;
    std::unordered_set<std::string> active_license_plates_;
    unsigned long long next_ticket_number_{1};
};

}  // namespace lld::parking_lot
