#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <limits>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld::movie_ticket_booking {

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using HoldId = std::uint64_t;
enum class HoldStatus { held, booked, cancelled, expired };
struct Hold {
    HoldId id;
    std::string show_id;
    std::string customer_id;
    std::vector<int> seats;
    TimePoint expires_at;
    HoldStatus status;
};
struct Booking {
    HoldId id;
    std::string show_id;
    std::string customer_id;
    std::vector<int> seats;
};

class BookingService {
public:
    void add_show(std::string id, std::string movie, int seat_count) {
        if (id.empty() || movie.empty() || seat_count <= 0) {
            throw std::invalid_argument("show, movie, and positive capacity required");
        }
        std::lock_guard<std::mutex> lock(mutex_);
        if (shows_.count(id) != 0) { throw std::invalid_argument("show already exists"); }
        shows_.emplace(std::move(id), Show{std::move(movie), std::vector<HoldId>(
            static_cast<std::size_t>(seat_count), 0)});
    }

    std::optional<Hold> hold(const std::string& show_id, const std::string& customer,
                            const std::vector<int>& seats, TimePoint now, std::chrono::seconds ttl) {
        if (customer.empty() || seats.empty() || ttl.count() <= 0 || ttl > std::chrono::hours(24)) {
            throw std::invalid_argument("customer, seats, and TTL of 1 second to 24 hours required");
        }
        const auto duration = std::chrono::duration_cast<Clock::duration>(ttl);
        if (now > TimePoint::max() - duration) { throw std::overflow_error("expiration overflow"); }
        std::lock_guard<std::mutex> lock(mutex_);
        auto& show = find_show(show_id);
        std::set<int> unique;
        for (int seat : seats) {
            if (seat < 0 || static_cast<std::size_t>(seat) >= show.owners.size() ||
                !unique.insert(seat).second) {
                throw std::invalid_argument("invalid or duplicate seat");
            }
        }
        advance_time(now);
        for (int seat : seats) {
            if (show.owners[static_cast<std::size_t>(seat)] != 0) { return std::nullopt; }
        }
        if (next_id_ == std::numeric_limits<HoldId>::max()) { throw std::overflow_error("hold IDs exhausted"); }
        Hold result{next_id_, show_id, customer, seats, now + duration, HoldStatus::held};
        holds_.emplace(result.id, result);  // Allocate record before modifying seat ownership.
        for (int seat : seats) { show.owners[static_cast<std::size_t>(seat)] = result.id; }
        ++next_id_;
        return std::optional<Hold>(std::move(result));
    }

    std::optional<Booking> confirm(HoldId id, TimePoint now) {
        std::lock_guard<std::mutex> lock(mutex_);
        advance_time(now);
        auto it = holds_.find(id);
        if (it == holds_.end() || (it->second.status != HoldStatus::held &&
                                  it->second.status != HoldStatus::booked)) {
            return std::nullopt;
        }
        auto& value = it->second;
        Booking result{id, value.show_id, value.customer_id, value.seats};
        value.status = HoldStatus::booked;  // Same booking on repeat confirmation.
        return std::optional<Booking>(std::move(result));
    }

    bool cancel(HoldId id, TimePoint now) {
        std::lock_guard<std::mutex> lock(mutex_);
        advance_time(now);
        auto it = holds_.find(id);
        if (it == holds_.end()) { return false; }
        if (it->second.status == HoldStatus::cancelled) { return true; }
        if (it->second.status != HoldStatus::held) { return false; }
        release_seats(it->second);
        it->second.status = HoldStatus::cancelled;
        return true;
    }

    std::size_t available_seats(const std::string& show_id, TimePoint now) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& show = find_show(show_id);
        advance_time(now);
        return static_cast<std::size_t>(std::count(show.owners.begin(), show.owners.end(), HoldId{0}));
    }

private:
    struct Show {
        std::string movie;
        std::vector<HoldId> owners;  // 0 means available; nonzero ID owns the seat.
    };
    Show& find_show(const std::string& id) {
        auto it = shows_.find(id);
        if (it == shows_.end()) { throw std::out_of_range("unknown show"); }
        return it->second;
    }
    void release_seats(const Hold& value) {
        auto& owners = find_show(value.show_id).owners;
        for (int seat : value.seats) { owners[static_cast<std::size_t>(seat)] = 0; }
    }
    void advance_time(TimePoint now) {
        if (last_now_ && now < *last_now_) { throw std::invalid_argument("clock moved backwards"); }
        last_now_ = now;
        for (auto& entry : holds_) {
            auto& value = entry.second;
            if (value.status == HoldStatus::held && now >= value.expires_at) {
                release_seats(value);
                value.status = HoldStatus::expired;
            }
        }
    }

    std::mutex mutex_;
    std::map<std::string, Show> shows_;
    std::map<HoldId, Hold> holds_;
    HoldId next_id_{1};
    std::optional<TimePoint> last_now_;
};

}  // namespace lld::movie_ticket_booking
