#pragma once
#include <chrono>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
namespace lld::amazon_locker {
using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
enum class Size { small = 0, medium = 1, large = 2 };
struct Slot { std::string id; Size size; };
struct Assignment {
    std::string package_id; std::string slot_id; std::string pickup_code;
    TimePoint deposited_at; TimePoint expires_at;
};
[[noreturn]] inline void todo(const char* task) { throw std::logic_error(std::string("TODO: ") + task); }
class Locker {
public:
    Locker(std::vector<Slot> /*slots*/, std::function<std::string()> /*code_generator*/) {
        todo("validate slots and own the injected pickup-code generator");
    }
    std::optional<Assignment> deposit(const std::string& /*package_id*/, Size /*size*/,
                                      TimePoint /*now*/, std::chrono::seconds /*ttl*/) {
        todo("choose the smallest compatible slot and create one active assignment");
    }
    std::optional<std::string> pickup(const std::string& /*code*/, TimePoint /*now*/) {
        todo("validate an unexpired single-use code and release the slot");
    }
    std::vector<std::string> collect_expired(TimePoint /*now*/) {
        todo("return physically collected packages before making slots available");
    }
    std::size_t available_slots() const { todo("read capacity under the state lock"); }
private:
    // TODO: Maintain package/code/slot consistency across concurrent operations.
    // TODO: Reject non-monotonic time and duplicate active pickup codes.
};
}  // namespace lld::amazon_locker
