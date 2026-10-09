#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"
#include <atomic>
#include <limits>
#include <thread>
#include <vector>

using namespace lld::rate_limiter;

int main() {
    test::Suite suite;
    suite.run("initial burst and denial", [] {
        TokenBucketLimiter limiter(3, 1.0);
        for (int i = 0; i < 3; ++i) { CHECK(limiter.allow("a", TimePoint{})); }
        CHECK(!limiter.allow("a", TimePoint{}));
    });
    suite.run("fractional refill reaches an exact boundary", [] {
        TokenBucketLimiter limiter(1, 2.0);
        CHECK(limiter.allow("a", TimePoint{}));
        CHECK(!limiter.allow("a", TimePoint{} + std::chrono::milliseconds(250)));
        CHECK(limiter.allow("a", TimePoint{} + std::chrono::milliseconds(500)));
    });
    suite.run("refill is capped at burst capacity", [] {
        TokenBucketLimiter limiter(2, 1.0);
        CHECK(limiter.allow("a", TimePoint{}, 2));
        CHECK(limiter.allow("a", TimePoint{} + std::chrono::hours(1), 2));
        CHECK(!limiter.allow("a", TimePoint{} + std::chrono::hours(1)));
    });
    suite.run("weighted requests do not borrow future tokens", [] {
        TokenBucketLimiter limiter(4, 1.0);
        CHECK(limiter.allow("a", TimePoint{}, 3));
        CHECK(!limiter.allow("a", TimePoint{}, 2));
        CHECK(limiter.allow("a", TimePoint{}, 1));
        CHECK(!limiter.allow("a", TimePoint{}, 5));
    });
    suite.run("clients are isolated", [] {
        TokenBucketLimiter limiter(1, 1.0);
        CHECK(limiter.allow("a", TimePoint{}));
        CHECK(limiter.allow("b", TimePoint{}));
        CHECK(limiter.tracked_clients() == 2);
    });
    suite.run("clock rollback rejected without granting tokens", [] {
        TokenBucketLimiter limiter(1, 1.0);
        CHECK(limiter.allow("a", TimePoint{} + std::chrono::seconds(10)));
        EXPECT_THROW(std::invalid_argument, limiter.allow("a", TimePoint{} + std::chrono::seconds(9)));
        CHECK(!limiter.allow("a", TimePoint{} + std::chrono::seconds(10)));
    });
    suite.run("configuration and request validation", [] {
        EXPECT_THROW(std::invalid_argument, TokenBucketLimiter(0, 1.0));
        EXPECT_THROW(std::invalid_argument, TokenBucketLimiter(1, std::numeric_limits<double>::infinity()));
        TokenBucketLimiter limiter(1, 1.0);
        EXPECT_THROW(std::invalid_argument, limiter.allow("", TimePoint{}));
        EXPECT_THROW(std::invalid_argument, limiter.allow("a", TimePoint{}, 0));
    });
    suite.run("concurrent admission preserves burst bound", [] {
        TokenBucketLimiter limiter(3, 1.0);
        std::atomic<int> accepted{0};
        std::vector<std::thread> threads;
        for (int i = 0; i < 12; ++i) {
            threads.emplace_back([&] { if (limiter.allow("a", TimePoint{})) { ++accepted; } });
        }
        for (auto& thread : threads) { thread.join(); }
        CHECK(accepted == 3);
    });
    return suite.finish();
}
