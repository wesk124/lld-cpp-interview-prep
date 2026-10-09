#pragma once
#include <map>
#include <stdexcept>
#include <string>
namespace lld {
namespace inventory_management {
struct Stock {
    int on_hand; int reserved;
    int available() const noexcept { return on_hand - reserved; }
};
enum class ReservationState { held, committed, released };
using Quantities = std::map<std::string, int>;
[[noreturn]] inline void todo(const char* task) { throw std::logic_error(std::string("TODO: ") + task); }
class Inventory {
public:
    void add_sku(const std::string& /*sku*/, int /*quantity*/) { todo("create a unique SKU with valid stock"); }
    void receive(const std::string& /*sku*/, int /*quantity*/) { todo("add stock with overflow checks"); }
    Stock stock(const std::string& /*sku*/) const { todo("return a synchronized stock snapshot"); }
    bool reserve(const std::string& /*order_id*/, const Quantities& /*quantities*/) {
        todo("validate the entire order, reserve atomically, and deduplicate matching retries");
    }
    bool commit(const std::string& /*order_id*/) { todo("decrease on-hand and reserved stock exactly once"); }
    bool release(const std::string& /*order_id*/) { todo("return held inventory exactly once"); }
private:
    // TODO: Preserve 0 <= reserved <= on_hand for every SKU.
    // TODO: Keep terminal order records so retries do not double-consume stock.
};
}  // namespace inventory_management
}  // namespace lld
