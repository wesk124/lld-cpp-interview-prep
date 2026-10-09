#pragma once

#include <algorithm>
#include <cmath>
#include <mutex>
#include <stdexcept>
#include <unordered_map>

namespace lld {
namespace rate_limiter {

// Strategy: clients use one admission interface.
class RateLimiter {
public:
    virtual ~RateLimiter() {}
    virtual bool allow(int client_id, long long now_ms, int cost = 1) = 0;
};

class TokenBucket : public RateLimiter {
public:
    TokenBucket(int capacity, double tokens_per_second)
        : capacity_(capacity), rate_(tokens_per_second) {
        if (capacity <= 0 || !std::isfinite(rate_) || rate_ <= 0)
            throw std::invalid_argument("invalid rate configuration");
    }

    bool allow(int client_id, long long now_ms, int cost = 1) override {
        if (now_ms < 0 || cost <= 0 || cost > capacity_) return false;
        std::lock_guard<std::mutex> lock(mutex_);
        auto entry = buckets_.emplace(client_id, Bucket{static_cast<double>(capacity_), now_ms});
        Bucket& bucket = entry.first->second;
        if (now_ms < bucket.last_ms) return false;
        double elapsed = static_cast<double>(now_ms - bucket.last_ms) / 1000.0;
        bucket.tokens = std::min(static_cast<double>(capacity_), bucket.tokens + elapsed * rate_);
        bucket.last_ms = now_ms;
        if (bucket.tokens < cost) return false;
        bucket.tokens -= cost;
        return true;
    }

private:
    struct Bucket { double tokens; long long last_ms; };
    int capacity_;
    double rate_;
    std::unordered_map<int, Bucket> buckets_;
    std::mutex mutex_; // One lock covers refill and consumption.
};

class FixedWindow : public RateLimiter {
public:
    FixedWindow(int limit, long long window_ms) : limit_(limit), window_ms_(window_ms) {
        if (limit <= 0 || window_ms <= 0) throw std::invalid_argument("invalid window configuration");
    }

    bool allow(int client_id, long long now_ms, int cost = 1) override {
        if (now_ms < 0 || cost <= 0 || cost > limit_) return false;
        std::lock_guard<std::mutex> lock(mutex_);
        auto entry = windows_.emplace(client_id, Window{now_ms, now_ms, 0});
        Window& window = entry.first->second;
        if (now_ms < window.last_ms) return false;
        if (now_ms - window.start_ms >= window_ms_) {
            window.start_ms = now_ms;
            window.used = 0;
        }
        window.last_ms = now_ms;
        if (cost > limit_ - window.used) return false;
        window.used += cost;
        return true;
    }

private:
    struct Window { long long start_ms; long long last_ms; int used; };
    int limit_;
    long long window_ms_;
    std::unordered_map<int, Window> windows_;
    std::mutex mutex_;
};

class RequestGate {
public:
    explicit RequestGate(RateLimiter& limiter) : limiter_(limiter) {}
    bool admit(int client_id, long long now_ms, int cost = 1) {
        return limiter_.allow(client_id, now_ms, cost);
    }
private:
    RateLimiter& limiter_; // Caller keeps the selected strategy alive.
};

} // namespace rate_limiter
} // namespace lld
