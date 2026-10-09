# Parking Lot

Design an in-memory parking lot for motorcycles, cars, and trucks.

## Interview contract

1. Allocate the smallest available compatible spot: motorcycle → any spot; car → compact/large; truck → large.
2. Return a stable ticket containing plate, spot ID, and injected entry time.
3. Allow at most one active ticket per plate. Reject incompatible capacity or duplicate plates with an empty optional.
4. Checkout prices the session before freeing the spot; unknown/used tickets return an empty optional.
5. Hourly pricing rounds partial hours up, with a one-hour minimum. Invalid times and fee overflow throw without releasing the session.

## Scope and assumptions

No payments, persistence, reservations, or distributed coordination. Prices are integer cents. Use system_clock timestamps supplied by the caller; the lot rejects exits earlier than their entries.

## Interviewee TODOs

- [ ] Define spot compatibility and validate unique empty spots.
- [ ] Own spots by value and pricing exclusively through a polymorphic interface.
- [ ] Make ticket/plate/spot transitions atomic, including allocation failure rollback.
- [ ] Implement price-before-release checkout and single-use tickets.
- [ ] Test exact-hour, just-over-hour, policy failure, and concurrent capacity boundaries.
- [ ] Explain object ownership, invariants, and error handling before writing code.
- [ ] Run the practice tests and discuss at least one alternative design.

## Run your attempt

Complete the marked methods in [starter.hpp](starter.hpp). The starting code compiles but deliberately throws `TODO` errors until implemented. It does not include or link the reference implementation.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=01-parking-lot
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Without CMake:

```bash
bash scripts/test.sh 01-parking-lot practice
```

Run commands from the repository root. The tests are the same behavioral contract as the reference solution; incomplete attempts are expected to fail them.

## Follow-ups

1. Add EV charging capabilities without a subclass explosion.
2. Add weekend pricing or floors/capacity displays.
3. Explain durable checkout with a payment system.

After your attempt: [design explanation](../../solutions/01-parking-lot/design.md) · [test checklist](test_plan.md) · [example quiz](../../quizzes/examples-questions.md).
