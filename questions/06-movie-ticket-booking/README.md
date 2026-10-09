# Movie Ticket Booking

A 45–60-minute OOP exercise. Main pattern: **Strategy**.

## Core interview contract

1. Add shows with seats indexed from 0.
2. Validate every requested seat before marking any of them booked; reject overlaps, duplicates and invalid seats with -1.
3. Delegate price calculation to SeatPricing and return a numeric booking ID.
4. Cancel a booking to free exactly its seats; return false for unknown/used booking IDs.

## Scope and assumptions

Single-threaded in-memory booking/cancellation. Prices are integer cents with ordinary interview-scale values. This core has no timed holds, payment integration or concurrent callers; those change the workflow and are explicit follow-ups.

## Interviewee TODOs

- [ ] Own seat availability per show and bookings keyed by integer IDs.
- [ ] Validate the whole request before changing availability.
- [ ] Use a SeatPricing Strategy; implement cancellation and booking snapshots.
- [ ] Explain ownership and the pattern's participating objects before coding.
- [ ] Test the main workflow, one boundary and one failure; discuss one follow-up.

## Run your attempt

Complete [starter.hpp](starter.hpp). It supplies interfaces and TODOs, not a solution. Run from the repository root:

```bash
bash scripts/test.sh 06-movie-ticket-booking practice
```

Or use CMake with `-DLLD_PRACTICE_EXAMPLE=06-movie-ticket-booking` and build/run `practice_tests`. The unfinished starter compiles but fails with TODO messages; it never includes the answer.

## Follow-ups for discussion

- Add one lock spanning availability checks and updates for concurrent bookings.
- Add timed holds/State-based lifecycle behavior and an Adapter for payment, with compensation.

[After your attempt: design](../../solutions/06-movie-ticket-booking/design.md) · [Test checklist](test_plan.md) · [Pattern guide](../../docs/design-patterns.md)
