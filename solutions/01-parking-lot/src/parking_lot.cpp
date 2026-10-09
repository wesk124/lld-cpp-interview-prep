#include "lld/parking_lot/parking_lot.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <utility>

namespace lld {
namespace parking_lot {
namespace {

int spot_rank(SpotType type) {
    switch (type) {
        case SpotType::motorcycle:
            return 0;
        case SpotType::compact:
            return 1;
        case SpotType::large:
            return 2;
    }
    return 3;
}

}  // namespace

ParkingLot::ParkingLot(std::vector<ParkingSpot> spots,
                       std::unique_ptr<PricingPolicy> pricing_policy)
    : spots_(std::move(spots)), pricing_policy_(std::move(pricing_policy)) {
    if (!pricing_policy_) {
        throw std::invalid_argument("pricing policy is required");
    }
    std::unordered_set<std::string> ids;
    for (const auto& spot : spots_) {
        if (!spot.is_available() || !ids.insert(spot.id()).second) {
            throw std::invalid_argument("spots must be empty with unique IDs");
        }
    }
}

lld::Optional<Ticket> ParkingLot::park(const Vehicle& vehicle, TimePoint entered_at) {
    if (vehicle.license_plate.empty()) {
        throw std::invalid_argument("license plate is required");
    }

    const std::lock_guard<std::mutex> lock(mutex_);
    if (active_license_plates_.count(vehicle.license_plate) != 0) {
        return {};
    }

    ParkingSpot* spot = find_best_spot(vehicle.type);
    if (spot == nullptr) {
        return {};
    }

    if (next_ticket_number_ == std::numeric_limits<unsigned long long>::max()) {
        throw std::overflow_error("ticket IDs exhausted");
    }
    Ticket ticket{"T-" + std::to_string(next_ticket_number_), vehicle.license_plate,
                  spot->id(), entered_at};
    lld::Optional<Ticket> result(ticket);  // Allocate the returned snapshot before committing.
    active_tickets_.emplace(ticket.id, ticket);
    try {
        active_license_plates_.insert(vehicle.license_plate);
        try {
            spot->occupy(vehicle);
        } catch (...) {
            active_license_plates_.erase(vehicle.license_plate);
            throw;
        }
    } catch (...) {
        active_tickets_.erase(ticket.id);
        throw;
    }
    ++next_ticket_number_;
    return result;
}

lld::Optional<Receipt> ParkingLot::exit(const std::string& ticket_id, TimePoint exited_at) {
    const std::lock_guard<std::mutex> lock(mutex_);
    const auto ticket_it = active_tickets_.find(ticket_id);
    if (ticket_it == active_tickets_.end()) {
        return {};
    }

    const Ticket ticket = ticket_it->second;
    const auto spot_it = std::find_if(spots_.begin(), spots_.end(), [&ticket](const ParkingSpot& spot) {
        return spot.id() == ticket.spot_id;
    });
    if (spot_it == spots_.end()) {
        throw std::logic_error("active ticket points to an unknown spot");
    }

    if (exited_at < ticket.entered_at) {
        throw std::invalid_argument("exit time cannot precede entry time");
    }
    const PricingQuote quote = pricing_policy_->calculate(ticket.entered_at, exited_at);
    if (quote.fee_cents < 0 || quote.charged_hours < 0) {
        throw std::logic_error("pricing policy returned an invalid quote");
    }
    Receipt receipt{ticket.id, ticket.license_plate, ticket.spot_id, ticket.entered_at,
                    exited_at, quote.charged_hours, quote.fee_cents};
    lld::Optional<Receipt> result(std::move(receipt));

    spot_it->vacate();
    active_license_plates_.erase(ticket.license_plate);
    active_tickets_.erase(ticket_it);

    return result;
}

std::size_t ParkingLot::available_spots() const {
    const std::lock_guard<std::mutex> lock(mutex_);
    return static_cast<std::size_t>(std::count_if(
        spots_.begin(), spots_.end(), [](const ParkingSpot& spot) { return spot.is_available(); }));
}

ParkingSpot* ParkingLot::find_best_spot(VehicleType vehicle_type) {
    ParkingSpot* best = nullptr;
    for (auto& spot : spots_) {
        if (!spot.is_available() || !spot.accepts(vehicle_type)) {
            continue;
        }
        if (best == nullptr || spot_rank(spot.type()) < spot_rank(best->type())) {
            best = &spot;
        }
    }
    return best;
}

}  // namespace parking_lot
}  // namespace lld
