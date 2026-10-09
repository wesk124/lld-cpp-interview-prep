#pragma once

#include <map>
#include <set>
#include <vector>

namespace lld {
namespace movie_ticket_booking {

// Strategy: seat booking does not choose a concrete pricing rule.
class SeatPricing {
public:
    virtual ~SeatPricing() {}
    virtual int total(int seats) const = 0;
};

class PerSeatPricing : public SeatPricing {
public:
    explicit PerSeatPricing(int cents_per_seat) : price_(cents_per_seat) {}
    int total(int seats) const override { return seats * price_; }
private:
    int price_;
};

class BookingFeePricing : public SeatPricing {
public:
    BookingFeePricing(int cents_per_seat, int booking_fee)
        : price_(cents_per_seat), fee_(booking_fee) {}
    int total(int seats) const override { return seats * price_ + fee_; }
private:
    int price_;
    int fee_;
};

struct Booking {
    int id;
    int show_id;
    int customer_id;
    std::vector<int> seats;
    int price_cents;
};

class BookingService {
public:
    explicit BookingService(const SeatPricing& pricing) : pricing_(pricing), next_id_(1) {}

    bool add_show(int show_id, int seat_count) {
        if (seat_count <= 0 || shows_.count(show_id) != 0) return false;
        shows_.emplace(show_id, std::vector<bool>(static_cast<std::size_t>(seat_count), false));
        return true;
    }

    int book(int show_id, int customer_id, const std::vector<int>& seats) {
        auto show = shows_.find(show_id);
        if (show == shows_.end() || seats.empty()) return -1;
        std::set<int> unique;
        for (int seat : seats)
            if (seat < 0 || seat >= static_cast<int>(show->second.size()) ||
                show->second[static_cast<std::size_t>(seat)] || !unique.insert(seat).second) return -1;

        int price = pricing_.total(static_cast<int>(seats.size()));
        int id = next_id_++;
        bookings_.emplace(id, Booking{id, show_id, customer_id, seats, price});
        for (int seat : seats) show->second[static_cast<std::size_t>(seat)] = true;
        return id;
    }

    bool cancel(int booking_id) {
        auto booking = bookings_.find(booking_id);
        if (booking == bookings_.end()) return false;
        auto& seats = shows_.at(booking->second.show_id);
        for (int seat : booking->second.seats) seats[static_cast<std::size_t>(seat)] = false;
        bookings_.erase(booking);
        return true;
    }

    Booking booking(int id) const { return bookings_.at(id); }
    int available_seats(int show_id) const {
        int count = 0;
        for (bool booked : shows_.at(show_id)) if (!booked) ++count;
        return count;
    }

private:
    const SeatPricing& pricing_; // Caller keeps this policy alive.
    std::map<int, std::vector<bool>> shows_;
    std::map<int, Booking> bookings_;
    int next_id_;
};

} // namespace movie_ticket_booking
} // namespace lld
