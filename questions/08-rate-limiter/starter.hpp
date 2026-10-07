#pragma once
#include <chrono>
#include <stdexcept>
#include <string>
namespace lld::rate_limiter {
using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
[[noreturn]] inline void todo(const char* task) { throw std::logic_error(std::string("TODO: ") + task); }
class TokenBucketLimiter {
public:
    TokenBucketLimiter(int /*capacity*/, double /*tokens_per_second*/) {
        todo("validate finite positive refill rate and burst capacity");
    }
    bool allow(const std::string& /*client*/, TimePoint /*now*/, int /*cost*/ = 1) {
        todo("refill and consume tokens atomically per client, without borrowing");
    }
    std::size_t tracked_clients() const { todo("return a synchronized client count"); }
private:
    // TODO: Own per-client tokens and last-refill timestamps.
    // TODO: Reject backward client time and cap accumulated tokens at capacity.
};
}  // namespace lld::rate_limiter
