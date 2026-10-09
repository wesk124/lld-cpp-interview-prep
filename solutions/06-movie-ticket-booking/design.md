# Movie Ticket Booking: Interview Design

[Question](../../questions/06-movie-ticket-booking/README.md) · [Core code](solution.hpp) · [Tests](tests.cpp)

## OOP responsibilities and pattern

Main pattern: **Strategy**.

| Object | Responsibility |
| --- | --- |
| `BookingService` | Show seats, numeric booking IDs and book/cancel workflows. |
| `Booking` | Customer/show IDs, selected seats and price snapshot. |
| `SeatPricing` | Abstract booking-price operation. |
| `PerSeatPricing / BookingFeePricing` | Alternative pricing rules. |

book checks all seats, calculates the price through the policy, stores a booking and marks seats. cancel restores its seats and erases the booking.

## Ownership and scope

The service owns show and booking values and borrows its pricing policy. booking(id) returns a copied value snapshot.

Single-threaded in-memory booking/cancellation. Prices are integer cents with ordinary interview-scale values. This core has no timed holds, payment integration or concurrent callers; those change the workflow and are explicit follow-ups.

## Small usage example

Within the example's namespace:

```cpp
BookingFeePricing pricing(1000, 200);
BookingService service(pricing);
service.add_show(10, 5);
int id = service.book(10, 7, {0, 1});
// service.booking(id).price_cents == 2200
```

For K requested seats, validation costs O(K log K) using a set, plus O(log shows + log bookings) lookups.

## Follow-up discussion

- Add one lock spanning availability checks and updates for concurrent bookings.
- Add timed holds/State-based lifecycle behavior and an Adapter for payment, with compensation.

[Pattern map and catalog](../../docs/design-patterns.md)
