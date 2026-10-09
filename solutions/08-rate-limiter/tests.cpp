#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"
#include <atomic>
#include <thread>
#include <vector>

using namespace lld::rate_limiter;

int main() {
    test::Suite suite;
    suite.run("continuous refill, weighted requests and capacity cap", [] {
        TokenBucket bucket(5, 2);
        CHECK(bucket.allow(1, 0, 5));
        CHECK(!bucket.allow(1, 0));
        CHECK(bucket.allow(1, 1500, 3));
        CHECK(!bucket.allow(1, 1500));
        CHECK(bucket.allow(1, 10000, 5));
        CHECK(!bucket.allow(1, 10000));
    });
    suite.run("client isolation and backward time", [] {
        TokenBucket bucket(2, 1);
        CHECK(bucket.allow(1, 1000, 2));
        CHECK(bucket.allow(2, 1000, 2));
        CHECK(!bucket.allow(1, 999));
        CHECK(!bucket.allow(1, 1000, 3));
        CHECK(!bucket.allow(1, 1000, 0));
        CHECK(bucket.allow(1, 2000));
    });
    suite.run("one Strategy interface, different quota semantics", [] {
        TokenBucket tokens(5, 2);
        FixedWindow windows(5, 1000);
        RequestGate token_gate(tokens), window_gate(windows);
        CHECK(token_gate.admit(1, 0, 5));
        CHECK(window_gate.admit(1, 0, 5));
        CHECK(token_gate.admit(1, 500));
        CHECK(!window_gate.admit(1, 500));
        CHECK(window_gate.admit(1, 1000, 5));
        CHECK(!window_gate.admit(1, 1000));
    });
    suite.run("fixed window weights and monotonic timestamps", [] {
        FixedWindow window(5, 1000);
        CHECK(window.allow(1, 100, 3));
        CHECK(!window.allow(1, 200, 3));
        CHECK(window.allow(1, 200, 2));
        CHECK(!window.allow(1, 199));
        CHECK(window.allow(1, 1100, 5));
    });
    suite.run("atomic refill plus consume", [] {
        TokenBucket bucket(5, 1);
        std::atomic<int> admitted(0);
        std::vector<std::thread> threads;
        for (int i = 0; i < 20; ++i)
            threads.emplace_back([&] { if (bucket.allow(1, 0)) ++admitted; });
        for (auto& thread : threads) thread.join();
        CHECK(admitted.load() == 5);
    });
    return suite.finish();
}
