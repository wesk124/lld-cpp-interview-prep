#pragma once

#include <chrono>
#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld::amazon_locker {

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
enum class Size { small = 0, medium = 1, large = 2 };
struct Slot {
    std::string id;
    Size size;
};
struct Assignment {
    std::string package_id;
    std::string slot_id;
    std::string pickup_code;
    TimePoint deposited_at;
    TimePoint expires_at;
};

class Locker {
public:
    Locker(std::vector<Slot> slots, std::function<std::string()> code_generator)
        : slots_(std::move(slots)), codes_(slots_.size()), code_generator_(std::move(code_generator)) {
        if (!code_generator_) { throw std::invalid_argument("code generator required"); }
        std::set<std::string> ids;
        for (const auto& slot : slots_) {
            if (slot.id.empty() || !valid_size(slot.size) || !ids.insert(slot.id).second) {
                throw std::invalid_argument("slots require unique IDs and valid sizes");
            }
        }
    }

    std::optional<Assignment> deposit(const std::string& package_id, Size size, TimePoint now,
                                       std::chrono::seconds ttl) {
        if (package_id.empty() || !valid_size(size) || ttl.count() <= 0 || ttl > std::chrono::hours(24)) {
            throw std::invalid_argument("invalid package, size, or TTL (1 second to 24 hours)");
        }
        const auto duration = std::chrono::duration_cast<Clock::duration>(ttl);
        if (now > TimePoint::max() - duration) { throw std::overflow_error("expiration overflow"); }
        std::lock_guard<std::mutex> lock(mutex_);
        check_time(now);
        for (const auto& entry : assignments_) {
            if (entry.second.package_id == package_id) { return std::nullopt; }
        }
        std::optional<std::size_t> best;
        for (std::size_t i = 0; i < slots_.size(); ++i) {
            if (codes_[i].empty() && static_cast<int>(slots_[i].size) >= static_cast<int>(size) &&
                (!best || slots_[i].size < slots_[*best].size)) {
                best = i;
            }
        }
        if (!best) { return std::nullopt; }
        std::string code = code_generator_();
        if (code.empty() || assignments_.count(code) != 0) {
            throw std::logic_error("generator returned empty or duplicate active code");
        }
        Assignment result{package_id, slots_[*best].id, code, now, now + duration};
        assignments_.emplace(code, result);  // Finish allocations before publishing occupancy.
        codes_[*best] = std::move(code);
        return std::optional<Assignment>(std::move(result));
    }

    std::optional<std::string> pickup(const std::string& code, TimePoint now) {
        std::lock_guard<std::mutex> lock(mutex_);
        check_time(now);
        const auto found = assignments_.find(code);
        if (found == assignments_.end() || now >= found->second.expires_at) { return std::nullopt; }
        std::string package_id = found->second.package_id;
        clear_slot(code);
        assignments_.erase(found);
        return std::optional<std::string>(std::move(package_id));
    }

    std::vector<std::string> collect_expired(TimePoint now) {
        std::lock_guard<std::mutex> lock(mutex_);
        check_time(now);
        std::vector<std::string> packages;
        for (const auto& entry : assignments_) {
            if (now >= entry.second.expires_at) { packages.push_back(entry.second.package_id); }
        }
        for (auto it = assignments_.begin(); it != assignments_.end();) {
            if (now >= it->second.expires_at) {
                clear_slot(it->first);
                it = assignments_.erase(it);
            } else { ++it; }
        }
        return packages;
    }

    std::size_t available_slots() const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::size_t count = 0;
        for (const auto& code : codes_) { if (code.empty()) { ++count; } }
        return count;
    }

private:
    static bool valid_size(Size size) {
        return size == Size::small || size == Size::medium || size == Size::large;
    }
    void check_time(TimePoint now) {
        if (last_now_ && now < *last_now_) { throw std::invalid_argument("clock moved backwards"); }
        last_now_ = now;
    }
    void clear_slot(const std::string& code) {
        for (auto& value : codes_) { if (value == code) { value.clear(); return; } }
        throw std::logic_error("assignment has no occupied slot");
    }

    mutable std::mutex mutex_;
    std::vector<Slot> slots_;
    std::vector<std::string> codes_;
    std::function<std::string()> code_generator_;
    std::map<std::string, Assignment> assignments_;
    std::optional<TimePoint> last_now_;
};

}  // namespace lld::amazon_locker
