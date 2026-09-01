#include "lld/parking_lot/parking_lot.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace lld::parking_lot {
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
}

std::optional<Ticket> ParkingLot::park(const Vehicle& vehicle, TimePoint entered_at) {
    if (vehicle.license_plate.empty()) {
        throw std::invalid_argument("license plate is required");
    }

    const std::lock_guard lock(mutex_);
    if (active_license_plates_.contains(vehicle.license_plate)) {
        return std::nullopt;
    }

    ParkingSpot* spot = find_best_spot(vehicle.type);
    if (spot == nullptr) {
        return std::nullopt;
    }

    spot->occupy(vehicle);
    Ticket ticket{
        .id = "T-" + std::to_string(next_ticket_number_++),
        .license_plate = vehicle.license_plate,
        .spot_id = spot->id(),
        .entered_at = entered_at,
    };

    active_license_plates_.insert(vehicle.license_plate);
    active_tickets_.emplace(ticket.id, ticket);
    return ticket;
}

std::optional<Receipt> ParkingLot::exit(const std::string& ticket_id, TimePoint exited_at) {
    const std::lock_guard lock(mutex_);
    const auto ticket_it = active_tickets_.find(ticket_id);
    if (ticket_it == active_tickets_.end()) {
        return std::nullopt;
    }

    const Ticket ticket = ticket_it->second;
    const auto spot_it = std::find_if(spots_.begin(), spots_.end(), [&ticket](const ParkingSpot& spot) {
        return spot.id() == ticket.spot_id;
    });
    if (spot_it == spots_.end()) {
        throw std::logic_error("active ticket points to an unknown spot");
    }

    const PricingQuote quote = pricing_policy_->calculate(ticket.entered_at, exited_at);

    spot_it->vacate();
    active_license_plates_.erase(ticket.license_plate);
    active_tickets_.erase(ticket_it);

    return Receipt{
        .ticket_id = ticket.id,
        .license_plate = ticket.license_plate,
        .spot_id = ticket.spot_id,
        .entered_at = ticket.entered_at,
        .exited_at = exited_at,
        .charged_hours = quote.charged_hours,
        .fee_cents = quote.fee_cents,
    };
}

std::size_t ParkingLot::available_spots() const {
    const std::lock_guard lock(mutex_);
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

}  // namespace lld::parking_lot
