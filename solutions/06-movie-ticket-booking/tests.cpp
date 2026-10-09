#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"

using namespace lld::movie_ticket_booking;

int main() {
    test::Suite suite;
    suite.run("book and cancel numeric booking IDs", [] {
        PerSeatPricing pricing(1000);
        BookingService service(pricing);
        CHECK(service.add_show(10, 3));
        int id = service.book(10, 7, {0, 1});
        CHECK(id == 1);
        CHECK(service.booking(id).price_cents == 2000);
        CHECK(service.available_seats(10) == 1);
        CHECK(service.cancel(id));
        CHECK(service.available_seats(10) == 3);
        CHECK(!service.cancel(id));
    });
    suite.run("validate the entire request before taking seats", [] {
        PerSeatPricing pricing(1000);
        BookingService service(pricing);
        service.add_show(10, 3);
        CHECK(service.book(10, 1, {1}) > 0);
        CHECK(service.book(10, 2, {2, 1}) == -1);
        CHECK(service.available_seats(10) == 2);
        CHECK(service.book(10, 2, {2}) > 0);
    });
    suite.run("unknown show, duplicates and invalid seats", [] {
        PerSeatPricing pricing(1000);
        BookingService service(pricing);
        CHECK(service.book(10, 1, {0}) == -1);
        CHECK(service.add_show(10, 2));
        CHECK(!service.add_show(10, 2));
        CHECK(service.book(10, 1, {0, 0}) == -1);
        CHECK(service.book(10, 1, {-1}) == -1);
        CHECK(service.book(10, 1, {2}) == -1);
        CHECK(service.available_seats(10) == 2);
    });
    suite.run("seat-pricing Strategy substitutes", [] {
        BookingFeePricing pricing(1000, 200);
        BookingService service(pricing);
        service.add_show(10, 3);
        int id = service.book(10, 1, {0, 1});
        CHECK(service.booking(id).price_cents == 2200);
    });
    return suite.finish();
}
