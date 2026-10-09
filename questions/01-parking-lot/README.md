# Parking Lot

A 45–60-minute OOP exercise. Main pattern: **Strategy**.

## Core interview contract

1. Park motorcycles, cars and trucks in the smallest free compatible spot.
2. Return a long long ticket ID, or -1 when parking fails; spot IDs are int.
3. Reject a second active session for the same plate. Checkout calculates cents before freeing the spot and invalidates the ticket.
4. Hourly pricing rounds partial hours up, with a one-hour minimum; flat pricing charges once.

## Scope and assumptions

Single-threaded, in-memory model. Time is supplied as nonnegative integer minutes. Construction supplies unique, initially free spot IDs and reasonable nonnegative prices/counts. There is no payment or persistence layer.

## Interviewee TODOs

- [ ] Implement spot compatibility and numeric ticket lookup.
- [ ] Implement park and checkout, preserving occupancy/session consistency.
- [ ] Call PricingPolicy through its interface and demonstrate hourly versus flat pricing.
- [ ] Explain ownership and the pattern's participating objects before coding.
- [ ] Test the main workflow, one boundary and one failure; discuss one follow-up.

## Run your attempt

Complete [starter.hpp](starter.hpp). It supplies interfaces and TODOs, not a solution. Run from the repository root:

```bash
bash scripts/test.sh 01-parking-lot practice
```

Or use CMake with `-DLLD_PRACTICE_EXAMPLE=01-parking-lot` and build/run `practice_tests`. The unfinished starter compiles but fails with TODO messages; it never includes the answer.

## Follow-ups for discussion

- Add a mutex around each complete park/checkout operation if concurrent entrances are required.
- Add a discount decorator, or extend the pricing input for time-of-day fees.

[After your attempt: design](../../solutions/01-parking-lot/design.md) · [Test checklist](test_plan.md) · [Pattern guide](../../docs/design-patterns.md)
