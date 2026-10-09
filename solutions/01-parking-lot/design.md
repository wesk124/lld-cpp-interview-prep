# Parking Lot: Reference Design

[Question](../../questions/01-parking-lot/README.md) · [Tests](tests/parking_lot_test.cpp)

## Responsibilities, ownership, and invariants

The existing multi-file design uses Vehicle/Ticket/Receipt values, ParkingSpot for compatibility and occupancy, PricingPolicy for fee rules, and ParkingLot for session coordination.

The lot owns spots in a vector and its pricing strategy via unique_ptr. Cross-object relationships use stable IDs; returned values do not borrow internal storage. Optional ticket and receipt snapshots use the small [C++11 helper](../../common/optional.hpp), which owns its value and copies it independently.

One mutex guards spots, active tickets, active plates, and ticket numbering. Parking stages allocations and rolls back indexes on failure. Checkout builds a receipt and validates/prices before changing any occupied state.

Allocation and checkout scan N spots: O(N), plus expected O(1) unordered index operations. Space is O(N + A), with A active sessions.

HourlyPricingPolicy truncates elapsed time to whole hours, adds an hour for any remainder, checks arithmetic overflow, and charges a minimum hour. The lot does not depend on the concrete hourly strategy.

The strategy runs under the lot lock. Custom policies must be bounded, non-reentrant, and free of I/O; production payments require an explicit transaction boundary.


## Scope

No payments, persistence, reservations, or distributed coordination. Prices are integer cents. Use system_clock timestamps supplied by the caller; the lot rejects exits earlier than their entries.

## Extend it yourself

- Add EV charging capabilities without a subclass explosion.
- Add weekend pricing or floors/capacity displays.
- Explain durable checkout with a payment system.
