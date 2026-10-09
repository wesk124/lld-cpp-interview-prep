# Movie Ticket Booking

Design seat reservations for movie shows with expiring holds and confirmation.

## Interview contract

1. Create uniquely identified shows with zero-based seats.
2. Hold an entire requested seat set atomically; invalid/duplicate seat numbers throw and conflicts return no hold.
3. A hold has a customer, stable ID, and deadline. It expires when now >= deadline.
4. Confirm a live hold into a booking; repeated confirmation returns the same booking.
5. Cancel an unconfirmed hold idempotently. Expired/cancelled holds never free seats now owned by newer holds; bookings do not expire.

## Scope and assumptions

Single-process in-memory service; no payments, authentication, refunds, seat pricing, theaters, or durable storage. TTL is 1 second through 24 hours. Inject steady_clock time points in nondecreasing processing order; expiry is processed on timed operations. Hold confirmation models an already-approved checkout, not a payment transaction.

## Interviewee TODOs

- [ ] Define show, seat ownership, hold, and booking records.
- [ ] Validate all requested seats before changing any of them.
- [ ] Serialize conflict checking and seat assignment.
- [ ] Expire only live holds and preserve terminal states for retries.
- [ ] Document the payment boundary and add contention tests.
- [ ] Explain object ownership, invariants, and error handling before writing code.
- [ ] Run the practice tests and discuss at least one alternative design.

## Run your attempt

Complete the marked methods in [starter.hpp](starter.hpp). The starting code compiles but deliberately throws `TODO` errors until implemented. It does not include or link the reference implementation.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=06-movie-ticket-booking
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Without CMake:

```bash
bash scripts/test.sh 06-movie-ticket-booking practice
```

Run commands from the repository root. The tests are the same behavioral contract as the reference solution; incomplete attempts are expected to fail them.

## Follow-ups

1. Add payment authorization and compensating release.
2. Add per-show locks or transactional durable storage.
3. Add seat categories, pricing policies, and booking refunds.

After your attempt: [design explanation](../../solutions/06-movie-ticket-booking/design.md) · [test checklist](test_plan.md) · [example quiz](../../quizzes/examples-questions.md).
