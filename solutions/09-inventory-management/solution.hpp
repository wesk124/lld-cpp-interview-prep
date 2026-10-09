#pragma once

#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace lld {
namespace inventory_management {

struct Stock {
    int on_hand;
    int reserved;
    int available() const { return on_hand - reserved; }
};
using Quantities = std::map<std::string, int>;
enum class ReservationState { held, committed, released };

// Observer: notification clients depend on stock events, not inventory internals.
class StockObserver {
public:
    virtual ~StockObserver() {}
    virtual void on_low_stock(const std::string& sku, int available) = 0;
};

class Inventory {
public:
    explicit Inventory(int low_stock_threshold = 3) : threshold_(low_stock_threshold) {}
    void subscribe(StockObserver& observer) { observers_.push_back(&observer); }
    void unsubscribe(StockObserver& observer) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), &observer), observers_.end());
    }
    bool add_sku(const std::string& sku, int quantity) {
        return !sku.empty() && quantity >= 0 && stocks_.emplace(sku, Stock{quantity, 0}).second;
    }
    bool receive(const std::string& sku, int quantity) {
        auto stock = stocks_.find(sku);
        if (stock == stocks_.end() || quantity <= 0) return false;
        stock->second.on_hand += quantity;
        notify(sku);
        return true;
    }
    Stock stock(const std::string& sku) const { return stocks_.at(sku); }

    bool reserve(int order_id, const Quantities& quantities) {
        if (quantities.empty()) return false;
        auto prior = reservations_.find(order_id);
        if (prior != reservations_.end())
            return prior->second.quantities == quantities && prior->second.state != ReservationState::released;
        for (const auto& item : quantities) {
            auto stock = stocks_.find(item.first);
            if (item.second <= 0 || stock == stocks_.end() || stock->second.available() < item.second)
                return false;
        }
        reservations_.emplace(order_id, Reservation{quantities, ReservationState::held});
        for (const auto& item : quantities) stocks_.at(item.first).reserved += item.second;
        for (const auto& item : quantities) notify(item.first);
        return true;
    }

    bool commit(int order_id) {
        auto order = reservations_.find(order_id);
        if (order == reservations_.end() || order->second.state == ReservationState::released) return false;
        if (order->second.state == ReservationState::committed) return true;
        for (const auto& item : order->second.quantities) {
            Stock& stock = stocks_.at(item.first);
            stock.on_hand -= item.second;
            stock.reserved -= item.second;
        }
        order->second.state = ReservationState::committed;
        return true;
    }

    bool release(int order_id) {
        auto order = reservations_.find(order_id);
        if (order == reservations_.end() || order->second.state == ReservationState::committed) return false;
        if (order->second.state == ReservationState::released) return true;
        for (const auto& item : order->second.quantities) stocks_.at(item.first).reserved -= item.second;
        order->second.state = ReservationState::released;
        for (const auto& item : order->second.quantities) notify(item.first);
        return true;
    }

private:
    struct Reservation { Quantities quantities; ReservationState state; };
    void notify(const std::string& sku) {
        int available = stocks_.at(sku).available();
        if (available < threshold_)
            for (StockObserver* observer : observers_) observer->on_low_stock(sku, available);
    }

    int threshold_;
    std::map<std::string, Stock> stocks_;
    std::map<int, Reservation> reservations_;
    std::vector<StockObserver*> observers_; // Non-owning subscribers.
};

} // namespace inventory_management
} // namespace lld
