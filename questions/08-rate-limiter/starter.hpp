#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace rate_limiter {

[[noreturn]] inline void todo(const char* task) {
    throw std::logic_error(std::string("TODO: ") + task);
}

class RateLimiter {
public:
    virtual ~RateLimiter() {}
    virtual bool allow(int client_id, long long now_ms, int cost = 1) = 0;
};
class TokenBucket : public RateLimiter {
public:
    TokenBucket(int /*capacity*/, double /*tokens_per_second*/) { todo("store capacity and rate"); }
    bool allow(int /*client_id*/, long long /*now_ms*/, int /*cost*/ = 1) override {
        todo("lock, refill continuously, cap tokens and consume if available");
    }
};
class FixedWindow : public RateLimiter {
public:
    FixedWindow(int /*limit*/, long long /*window_ms*/) { todo("store window configuration"); }
    bool allow(int /*client_id*/, long long /*now_ms*/, int /*cost*/ = 1) override {
        todo("lock, reset elapsed window and count allowed request cost");
    }
};
class RequestGate {
public:
    explicit RequestGate(RateLimiter& /*limiter*/) { todo("borrow a Strategy"); }
    bool admit(int /*client_id*/, long long /*now_ms*/, int /*cost*/ = 1) {
        todo("delegate admission through the interface");
    }
};

} // namespace rate_limiter
} // namespace lld
