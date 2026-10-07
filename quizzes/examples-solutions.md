# Nine-Example Scenario Quiz: Answers

## Parking Lot

1. No. Validate and price before changing occupancy; retain the ticket so the caller can recover/retry.
2. No. Total availability and compatible availability are different. Allocation checks capability, not just the number of free slots.
3. IDs do not depend on container relocation or object lifetimes, and make session values independent of internal storage.

## Connect Four

1. The same player's turn. Validate before modifying turn or board state.
2. No. Only lines through the new piece can create a new win; check both directions along four axes.
3. To detect a full-board draw without an additional full scan after every move; increment only for legal moves.

## Amazon Locker

1. No. Code invalidation is not physical package removal. The compartment remains occupied until collection.
2. No. Reject the collision before publishing occupancy, keeping the existing assignment and free capacity intact.
3. No. Production requires unpredictable codes, authentication/attempt limits, and a secure lifecycle; the sample only demonstrates injection.

## Elevator

1. No. The next tick closes the doors at floor 3. A later tick can move.
2. No. It completes current-direction destinations before reversing when none remain ahead.
3. No. The sample uses distance-only selection; directional hall queues and bounded waiting are separate design requirements.

## File System

1. The directory can own each child exclusively through unique_ptr; the root owns the entire hierarchy through those relationships.
2. Not under this contract. The parent must exist; recursive mkdir is a separate operation.
3. They preserve synchronization and lifetime boundaries: callers cannot mutate state outside the lock or retain invalidated pointers.

## Movie Ticket Booking

1. Expiry processing, conflict validation, and seat ownership publication must be coordinated atomically; a separate availability check is not enough.
2. No. The contract expires holds when now >= expires_at, including the exact deadline.
3. Otherwise a stale ID could free a seat reused by a new hold. Lifecycle validation also supports idempotent confirmation/cancellation.

## Logging Service

1. No. Snapshot sinks while locked, then release the logger lock before fanout. Each sink protects its own state.
2. No. Isolate per-sink failure, continue fanout, and report delivered/failed counts.
3. No. Per-sink synchronization prevents corruption, not a shared total order. A single queue/consumer can provide global ordering.

## Rate Limiter

1. 3 tokens (before any new consumption), capped at capacity.
2. No. It bounds burst and refill rate; requests near a window boundary may form a larger fixed-window count.
3. Otherwise concurrent callers may observe the same budget and consume it twice, violating the burst bound.

## Inventory Management

1. No. The reservation is all-or-nothing across the entire order.
2. No. Stable ID plus matching payload/lifecycle permits idempotent acknowledgement without a second stock mutation.
3. Available is 6. Commit makes on_hand=6 and reserved=0; available remains 6.
