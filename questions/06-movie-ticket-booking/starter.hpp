#pragma once
#include <chrono>
#include <cstdint>
#include "optional.hpp"
#include <stdexcept>
#include <string>
#include <vector>
namespace lld {
namespace movie_ticket_booking {
using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using HoldId = std::uint64_t;
enum class HoldStatus { held, booked, cancelled, expired };
struct Hold {
    HoldId id; std::string show_id; std::string customer_id; std::vector<int> seats;
    TimePoint expires_at; HoldStatus status;
};
struct Booking { HoldId id; std::string show_id; std::string customer_id; std::vector<int> seats; };
[[noreturn]] inline void todo(const char* task) { throw std::logic_error(std::string("TODO: ") + task); }
class BookingService {
public:
    void add_show(std::string /*id*/, std::string /*movie*/, int /*seat_count*/) {
        todo("create a show with initially available zero-based seats");
    }
    lld::Optional<Hold> hold(const std::string& /*show*/, const std::string& /*customer*/,
                            const std::vector<int>& /*seats*/, TimePoint /*now*/,
                            std::chrono::seconds /*ttl*/) {
        todo("validate all seats before reserving any; reject conflicts atomically");
    }
    lld::Optional<Booking> confirm(HoldId /*id*/, TimePoint /*now*/) {
        todo("expire stale holds, then confirm idempotently");
    }
    bool cancel(HoldId /*id*/, TimePoint /*now*/) {
        todo("release a live hold without cancelling a completed booking");
    }
    std::size_t available_seats(const std::string& /*show*/, TimePoint /*now*/) {
        todo("expire stale holds before reporting capacity");
    }
private:
    // TODO: Own show seat state and the hold lifecycle ledger under one mutex.
    // TODO: A stale hold must never free a seat now owned by another hold.
};
}  // namespace movie_ticket_booking
}  // namespace lld
