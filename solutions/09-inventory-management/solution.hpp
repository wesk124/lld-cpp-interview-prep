#pragma once

#include <limits>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>

namespace lld::inventory_management {

struct Stock {
    int on_hand;
    int reserved;
    int available() const noexcept { return on_hand - reserved; }
};
enum class ReservationState { held, committed, released };
using Quantities = std::map<std::string, int>;

class Inventory {
public:
    void add_sku(const std::string& sku, int quantity) {
        if (sku.empty() || quantity < 0) { throw std::invalid_argument("SKU and nonnegative quantity required"); }
        std::lock_guard<std::mutex> lock(mutex_);
        if (!stocks_.emplace(sku, Stock{quantity, 0}).second) {
            throw std::invalid_argument("SKU already exists");
        }
    }

    void receive(const std::string& sku, int quantity) {
        if (quantity <= 0) { throw std::invalid_argument("receipt quantity must be positive"); }
        std::lock_guard<std::mutex> lock(mutex_);
        auto& stock = stocks_.at(sku);
        if (stock.on_hand > std::numeric_limits<int>::max() - quantity) {
            throw std::overflow_error("stock quantity overflow");
        }
        stock.on_hand += quantity;
    }

    Stock stock(const std::string& sku) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return stocks_.at(sku);
    }

    bool reserve(const std::string& order_id, const Quantities& quantities) {
        if (order_id.empty() || quantities.empty()) {
            throw std::invalid_argument("order and item quantities required");
        }
        for (const auto& item : quantities) {
            if (item.first.empty() || item.second <= 0) {
                throw std::invalid_argument("SKU and positive reservation quantity required");
            }
        }
        std::lock_guard<std::mutex> lock(mutex_);
        auto prior = reservations_.find(order_id);
        if (prior != reservations_.end()) {
            if (prior->second.quantities != quantities) {
                throw std::invalid_argument("order ID reused with different quantities");
            }
            return prior->second.state != ReservationState::released;
        }
        for (const auto& item : quantities) {
            auto found = stocks_.find(item.first);
            if (found == stocks_.end() || found->second.available() < item.second) { return false; }
        }
        reservations_.emplace(order_id, Reservation{quantities, ReservationState::held});
        for (const auto& item : quantities) { stocks_.at(item.first).reserved += item.second; }
        return true;
    }

    bool commit(const std::string& order_id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = reservations_.find(order_id);
        if (it == reservations_.end() || it->second.state == ReservationState::released) { return false; }
        if (it->second.state == ReservationState::committed) { return true; }
        for (const auto& item : it->second.quantities) {
            auto& stock = stocks_.at(item.first);
            stock.on_hand -= item.second;
            stock.reserved -= item.second;
        }
        it->second.state = ReservationState::committed;
        return true;
    }

    bool release(const std::string& order_id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = reservations_.find(order_id);
        if (it == reservations_.end() || it->second.state == ReservationState::committed) { return false; }
        if (it->second.state == ReservationState::released) { return true; }
        for (const auto& item : it->second.quantities) { stocks_.at(item.first).reserved -= item.second; }
        it->second.state = ReservationState::released;
        return true;
    }

private:
    struct Reservation {
        Quantities quantities;
        ReservationState state;
    };
    mutable std::mutex mutex_;
    std::map<std::string, Stock> stocks_;
    std::map<std::string, Reservation> reservations_;
};

}  // namespace lld::inventory_management
