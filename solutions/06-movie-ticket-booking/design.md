# Movie Ticket Booking: Reference Design

[Question](../../questions/06-movie-ticket-booking/README.md) · [Tests](tests.cpp)

## Responsibilities, ownership, and invariants

BookingService owns shows and a lifecycle ledger of Hold records. Each seat stores 0 (free) or a stable HoldId; one mutex spans conflict check, ownership assignment, expiry, and confirmation.

The lifecycle is held → booked, cancelled, or expired. Terminal records are retained to make confirmation/cancellation retries deterministic.

An injected monotonic clock and exact >= deadline rule make expiration deterministic. Expiry only processes records still in held state, so stale holds cannot free newer reservations.

Optional hold and booking snapshots use the small [C++11 helper](../../common/optional.hpp), which owns its value independently of the ledger. Multi-seat hold checks every seat first, allocates the result and hold record, then publishes integer ownership IDs. Confirmation builds the returned Booking value before committing state.

A timed operation scans H hold records and releases affected seats. Holding K seats adds O(K log K + log H), listing capacity O(S), space O(S + retained hold-seat history).

Expired history grows without bound in this sample. Production needs retention/cleanup, per-show or database transaction locks, and explicit payment idempotency and compensation.


## OOP and design patterns

- **Current OOP design:** `BookingService` coordinates show capacity, seat ownership, and hold records. `Hold` and `Booking` are value snapshots. `HoldStatus` encodes an explicit lifecycle; there are no GoF State objects in the reference.
- **State (follow-up):** A hold context could delegate confirm/cancel behavior to lifecycle state objects as refund or payment-pending rules expand. Seat conflict checks and updates still need their atomic boundary.
- **Strategy (follow-up):** A seat-pricing interface could support category, event, or membership pricing independently of ownership and expiry rules.
- **Adapter (follow-up):** A payment adapter could translate a provider's API into domain checkout operations. Idempotency, failure compensation, and I/O placement remain explicit workflow decisions.

[Pattern map and catalog](../../docs/design-patterns.md)

## Scope

Single-process in-memory service; no payments, authentication, refunds, seat pricing, theaters, or durable storage. TTL is 1 second through 24 hours. Inject steady_clock time points in nondecreasing processing order; expiry is processed on timed operations. Hold confirmation models an already-approved checkout, not a payment transaction.

## Extend it yourself

- Add payment authorization and compensating release.
- Add per-show locks or transactional durable storage.
- Add seat categories, pricing policies, and booking refunds.
