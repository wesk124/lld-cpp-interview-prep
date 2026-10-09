#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>

namespace lld {
namespace rate_limiter {

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;

class TokenBucketLimiter {
public:
    TokenBucketLimiter(int capacity, double tokens_per_second)
        : capacity_(capacity), rate_(tokens_per_second) {
        if (capacity <= 0 || !std::isfinite(rate_) || rate_ <= 0.0) {
            throw std::invalid_argument("positive capacity and finite positive rate required");
        }
    }

    bool allow(const std::string& client, TimePoint now, int cost = 1) {
        if (client.empty() || cost <= 0) { throw std::invalid_argument("client and positive cost required"); }
        std::lock_guard<std::mutex> lock(mutex_);
        const auto prior = buckets_.find(client);
        if (prior != buckets_.end() && now < prior->second.last_refill) {
            throw std::invalid_argument("client clock moved backwards");
        }
        if (cost > capacity_) { return false; }
        auto result = buckets_.emplace(client, Bucket{static_cast<double>(capacity_), now});
        auto& bucket = result.first->second;
        if (now < bucket.last_refill) { throw std::invalid_argument("client clock moved backwards"); }
        // Convert epochs separately to avoid overflowing an integer duration subtraction.
        const double now_seconds = std::chrono::duration<double>(now.time_since_epoch()).count();
        const double last_seconds = std::chrono::duration<double>(bucket.last_refill.time_since_epoch()).count();
        const double elapsed = now_seconds - last_seconds;
        bucket.tokens = std::min(static_cast<double>(capacity_), bucket.tokens + elapsed * rate_);
        bucket.last_refill = now;
        if (bucket.tokens < static_cast<double>(cost)) { return false; }
        bucket.tokens -= static_cast<double>(cost);
        return true;
    }

    std::size_t tracked_clients() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return buckets_.size();
    }

private:
    struct Bucket {
        double tokens;
        TimePoint last_refill;
    };
    int capacity_;
    double rate_;
    mutable std::mutex mutex_;
    std::map<std::string, Bucket> buckets_;
};

}  // namespace rate_limiter
}  // namespace lld
