#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace movie_ticket_booking {

[[noreturn]] inline void todo(const char* task) {
    throw std::logic_error(std::string("TODO: ") + task);
}

class SeatPricing {
public:
    virtual ~SeatPricing() {}
    virtual int total(int seats) const = 0;
};
class PerSeatPricing : public SeatPricing {
public:
    explicit PerSeatPricing(int /*cents_per_seat*/) { todo("store seat price"); }
    int total(int /*seats*/) const override { todo("calculate per-seat total"); }
};
class BookingFeePricing : public SeatPricing {
public:
    BookingFeePricing(int /*cents_per_seat*/, int /*booking_fee*/) { todo("store pricing values"); }
    int total(int /*seats*/) const override { todo("add booking fee to seat total"); }
};
struct Booking { int id; int show_id; int customer_id; std::vector<int> seats; int price_cents; };
class BookingService {
public:
    explicit BookingService(const SeatPricing& /*pricing*/) { todo("borrow pricing Strategy"); }
    bool add_show(int /*show_id*/, int /*seat_count*/) { todo("create seat availability"); }
    int book(int /*show_id*/, int /*customer_id*/, const std::vector<int>& /*seats*/) {
        todo("validate entire request, calculate price, create booking and mark seats");
    }
    bool cancel(int /*booking_id*/) { todo("free booking seats and remove booking"); }
    Booking booking(int /*id*/) const { todo("return booking snapshot"); }
    int available_seats(int /*show_id*/) const { todo("count free seats"); }
};

} // namespace movie_ticket_booking
} // namespace lld
