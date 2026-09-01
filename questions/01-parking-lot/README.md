# Question 01: Parking Lot

Design a parking lot that supports motorcycles, cars, and trucks.

## Requirements

1. Park a vehicle in the smallest available compatible spot.
2. Return a ticket containing stable vehicle and spot identifiers.
3. Exit using an active ticket, free the spot, and calculate a fee.
4. Reject parking when no compatible spot exists.
5. Reject an unknown or already-used ticket.

Compatibility rules:

- A motorcycle may use motorcycle, compact, or large spots.
- A car may use compact or large spots.
- A truck may use only large spots.

## Constraints

- Concurrent park and exit operations must preserve invariants.
- Tests must not depend on the wall clock.
- Pricing should be replaceable without changing allocation logic.
- Persistence, reservations, payment processing, and multiple floors are out of scope.

## Interviewee TODOs

- [ ] Clarify ambiguous requirements and record assumptions.
- [ ] Identify actors, use cases, entities, and invariants.
- [ ] Define vehicle, spot, ticket, receipt, and pricing-policy types.
- [ ] State the ownership and lifetime of every major object.
- [ ] Implement smallest-compatible-spot allocation.
- [ ] Prevent one license plate from receiving multiple active tickets.
- [ ] Make park and exit operations thread-safe.
- [ ] Inject timestamps so tests remain deterministic.
- [ ] Test success, capacity, duplicate, invalid-ticket, and fee-rounding cases.
- [ ] Explain one alternative design and why you rejected it.

Start from [`starter.hpp`](include/lld/parking_lot/starter.hpp) and [`starter.cpp`](src/starter.cpp). The starter files intentionally do not build as part of the root project until you finish them.

## Follow-up changes

Choose at least one:

1. Add electric charging spots.
2. Add weekend and event pricing.
3. Support multiple floors and capacity displays.
4. Add reservations with expiration.
5. Replace in-memory state with durable storage.

After your attempt, compare it with the [reference solution](../../solutions/01-parking-lot/design.md).
