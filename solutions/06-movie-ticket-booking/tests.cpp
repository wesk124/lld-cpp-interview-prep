#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"
#include <atomic>
#include <thread>

using namespace lld::movie_ticket_booking;
using namespace std::chrono_literals;

int main() {
    test::Suite suite;
    suite.run("hold and idempotent confirmation", [] {
        BookingService service;
        service.add_show("show", "movie", 5);
        auto hold = service.hold("show", "alice", {0, 1}, TimePoint{}, 1min);
        CHECK(hold);
        CHECK(service.available_seats("show", TimePoint{}) == 3);
        auto booking = service.confirm(hold->id, TimePoint{} + 1s);
        CHECK(booking && booking->seats.size() == 2);
        auto retry = service.confirm(hold->id, TimePoint{} + 2s);
        CHECK(retry && retry->id == booking->id);
    });
    suite.run("multi-seat hold is all-or-nothing", [] {
        BookingService service;
        service.add_show("show", "movie", 3);
        CHECK(service.hold("show", "alice", {1}, TimePoint{}, 1min));
        CHECK(!service.hold("show", "bob", {0, 1}, TimePoint{}, 1min));
        CHECK(service.hold("show", "bob", {0, 2}, TimePoint{}, 1min));
    });
    suite.run("expiration at exact deadline and reuse", [] {
        BookingService service;
        service.add_show("show", "movie", 1);
        auto old = service.hold("show", "alice", {0}, TimePoint{}, 10s);
        CHECK(old);
        CHECK(!service.confirm(old->id, TimePoint{} + 10s));
        CHECK(service.hold("show", "bob", {0}, TimePoint{} + 10s, 1min));
        CHECK(!service.cancel(old->id, TimePoint{} + 10s));
        CHECK(service.available_seats("show", TimePoint{} + 10s) == 0);
    });
    suite.run("booked seats are not expired or cancellable", [] {
        BookingService service;
        service.add_show("show", "movie", 1);
        auto hold = service.hold("show", "alice", {0}, TimePoint{}, 1s);
        CHECK(hold && service.confirm(hold->id, TimePoint{}));
        CHECK(service.available_seats("show", TimePoint{} + 1h) == 0);
        CHECK(!service.cancel(hold->id, TimePoint{} + 1h));
    });
    suite.run("cancellation releases seats and is idempotent", [] {
        BookingService service;
        service.add_show("show", "movie", 2);
        auto hold = service.hold("show", "alice", {0, 1}, TimePoint{}, 1min);
        CHECK(hold);
        CHECK(service.cancel(hold->id, TimePoint{}));
        CHECK(service.cancel(hold->id, TimePoint{}));
        CHECK(!service.confirm(hold->id, TimePoint{}));
        CHECK(service.available_seats("show", TimePoint{}) == 2);
    });
    suite.run("shows have independent seats", [] {
        BookingService service;
        service.add_show("a", "movie", 1);
        service.add_show("b", "movie", 1);
        CHECK(service.hold("a", "alice", {0}, TimePoint{}, 1min));
        CHECK(service.hold("b", "bob", {0}, TimePoint{}, 1min));
    });
    suite.run("validate inputs and reject backward time", [] {
        BookingService service;
        service.add_show("show", "movie", 2);
        EXPECT_THROW(std::invalid_argument, service.add_show("show", "movie", 2));
        EXPECT_THROW(std::invalid_argument, service.hold("show", "alice", {0, 0}, TimePoint{}, 1min));
        EXPECT_THROW(std::invalid_argument, service.hold("show", "alice", {2}, TimePoint{}, 1min));
        EXPECT_THROW(std::out_of_range, service.available_seats("missing", TimePoint{}));
        CHECK(!service.confirm(999, TimePoint{} + 1s));
        EXPECT_THROW(std::invalid_argument, service.confirm(999, TimePoint{}));
    });
    suite.run("concurrent requests cannot sell the same seat twice", [] {
        BookingService service;
        service.add_show("show", "movie", 1);
        std::atomic<int> accepted{0};
        std::vector<std::thread> threads;
        for (int i = 0; i < 8; ++i) {
            threads.emplace_back([&, i] {
                if (service.hold("show", std::to_string(i), {0}, TimePoint{}, 1min)) { ++accepted; }
            });
        }
        for (auto& thread : threads) { thread.join(); }
        CHECK(accepted == 1);
    });
    return suite.finish();
}
