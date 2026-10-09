#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace inventory_management {

[[noreturn]] inline void todo(const char* task) {
    throw std::logic_error(std::string("TODO: ") + task);
}

struct Stock {
    int on_hand; int reserved;
    int available() const { return on_hand - reserved; }
};
using Quantities = std::map<std::string, int>;
enum class ReservationState { held, committed, released };
class StockObserver {
public:
    virtual ~StockObserver() {}
    virtual void on_low_stock(const std::string& sku, int available) = 0;
};
class Inventory {
public:
    explicit Inventory(int /*low_stock_threshold*/ = 3) { todo("store notification threshold"); }
    void subscribe(StockObserver& /*observer*/) { todo("register Observer"); }
    void unsubscribe(StockObserver& /*observer*/) { todo("remove Observer"); }
    bool add_sku(const std::string& /*sku*/, int /*quantity*/) { todo("add unique SKU"); }
    bool receive(const std::string& /*sku*/, int /*quantity*/) { todo("increase on-hand stock"); }
    Stock stock(const std::string& /*sku*/) const { todo("return stock snapshot"); }
    bool reserve(int /*order_id*/, const Quantities& /*quantities*/) {
        todo("validate whole order, reserve quantities and notify low-stock Observers");
    }
    bool commit(int /*order_id*/) { todo("consume reserved stock once"); }
    bool release(int /*order_id*/) { todo("release reserved stock once"); }
};

} // namespace inventory_management
} // namespace lld
