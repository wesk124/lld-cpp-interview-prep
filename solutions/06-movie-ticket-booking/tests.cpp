#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"
#include <atomic>
#include <thread>

using namespace lld::movie_ticket_booking;

int main() {
    test::Suite suite;
    suite.run("hold and idempotent confirmation", [] {
        BookingService service;
        service.add_show("show", "movie", 5);
        auto hold = service.hold("show", "alice", {0, 1}, TimePoint{}, std::chrono::minutes(1));
        CHECK(hold);
        CHECK(service.available_seats("show", TimePoint{}) == 3);
        auto booking = service.confirm(hold->id, TimePoint{} + std::chrono::seconds(1));
        CHECK(booking && booking->seats.size() == 2);
        auto retry = service.confirm(hold->id, TimePoint{} + std::chrono::seconds(2));
        CHECK(retry && retry->id == booking->id);
    });
    suite.run("multi-seat hold is all-or-nothing", [] {
        BookingService service;
        service.add_show("show", "movie", 3);
        CHECK(service.hold("show", "alice", {1}, TimePoint{}, std::chrono::minutes(1)));
        CHECK(!service.hold("show", "bob", {0, 1}, TimePoint{}, std::chrono::minutes(1)));
        CHECK(service.hold("show", "bob", {0, 2}, TimePoint{}, std::chrono::minutes(1)));
    });
    suite.run("expiration at exact deadline and reuse", [] {
        BookingService service;
        service.add_show("show", "movie", 1);
        auto old = service.hold("show", "alice", {0}, TimePoint{}, std::chrono::seconds(10));
        CHECK(old);
        CHECK(!service.confirm(old->id, TimePoint{} + std::chrono::seconds(10)));
        CHECK(service.hold("show", "bob", {0}, TimePoint{} + std::chrono::seconds(10), std::chrono::minutes(1)));
        CHECK(!service.cancel(old->id, TimePoint{} + std::chrono::seconds(10)));
        CHECK(service.available_seats("show", TimePoint{} + std::chrono::seconds(10)) == 0);
    });
    suite.run("booked seats are not expired or cancellable", [] {
        BookingService service;
        service.add_show("show", "movie", 1);
        auto hold = service.hold("show", "alice", {0}, TimePoint{}, std::chrono::seconds(1));
        CHECK(hold && service.confirm(hold->id, TimePoint{}));
        CHECK(service.available_seats("show", TimePoint{} + std::chrono::hours(1)) == 0);
        CHECK(!service.cancel(hold->id, TimePoint{} + std::chrono::hours(1)));
    });
    suite.run("cancellation releases seats and is idempotent", [] {
        BookingService service;
        service.add_show("show", "movie", 2);
        auto hold = service.hold("show", "alice", {0, 1}, TimePoint{}, std::chrono::minutes(1));
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
        CHECK(service.hold("a", "alice", {0}, TimePoint{}, std::chrono::minutes(1)));
        CHECK(service.hold("b", "bob", {0}, TimePoint{}, std::chrono::minutes(1)));
    });
    suite.run("validate inputs and reject backward time", [] {
        BookingService service;
        service.add_show("show", "movie", 2);
        EXPECT_THROW(std::invalid_argument, service.add_show("show", "movie", 2));
        EXPECT_THROW(std::invalid_argument, service.hold("show", "alice", {0, 0}, TimePoint{}, std::chrono::minutes(1)));
        EXPECT_THROW(std::invalid_argument, service.hold("show", "alice", {2}, TimePoint{}, std::chrono::minutes(1)));
        EXPECT_THROW(std::out_of_range, service.available_seats("missing", TimePoint{}));
        CHECK(!service.confirm(999, TimePoint{} + std::chrono::seconds(1)));
        EXPECT_THROW(std::invalid_argument, service.confirm(999, TimePoint{}));
    });
    suite.run("concurrent requests cannot sell the same seat twice", [] {
        BookingService service;
        service.add_show("show", "movie", 1);
        std::atomic<int> accepted{0};
        std::vector<std::thread> threads;
        for (int i = 0; i < 8; ++i) {
            threads.emplace_back([&, i] {
                if (service.hold("show", std::to_string(i), {0}, TimePoint{}, std::chrono::minutes(1))) { ++accepted; }
            });
        }
        for (auto& thread : threads) { thread.join(); }
        CHECK(accepted == 1);
    });
    return suite.finish();
}
