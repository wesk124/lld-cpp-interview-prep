# Inventory Management

A 45–60-minute OOP exercise. Main pattern: **Observer**.

## Core interview contract

1. Track 0 <= reserved <= on_hand for each named SKU.
2. Validate the entire multi-SKU order before reserving any quantity.
3. Commit consumes both on-hand and reserved quantities; release changes only reserved quantities.
4. Matching reserve/commit/release retries do not update stock twice; changed payloads reject.
5. Notify subscribed observers after availability changes when available stock is below the threshold.

## Scope and assumptions

Single-threaded in-memory catalog with numeric order IDs and text SKU names. Notifications occur after the full mutation, use successful non-mutating callbacks, and may repeat while stock remains low. Subscribers outlive registration. Warehouses, expiration and durable storage are follow-ups.

## Interviewee TODOs

- [ ] Encapsulate catalog and reservation ownership inside Inventory.
- [ ] Implement validate-before-update and idempotent lifecycle transitions.
- [ ] Publish low-stock events through StockObserver instead of calling a concrete notification class.
- [ ] Explain ownership and the pattern's participating objects before coding.
- [ ] Test the main workflow, one boundary and one failure; discuss one follow-up.

## Run your attempt

Complete [starter.hpp](starter.hpp). It supplies interfaces and TODOs, not a solution. Run from the repository root:

```bash
bash scripts/test.sh 09-inventory-management practice
```

Or use CMake with `-DLLD_PRACTICE_EXAMPLE=09-inventory-management` and build/run `practice_tests`. The unfinished starter compiles but fails with TODO messages; it never includes the answer.

## Follow-ups for discussion

- Add warehouse-selection Strategy or an expanded reservation State design.
- Add concurrency, durable idempotency, callback failure rules and event ordering to the model.

[After your attempt: design](../../solutions/09-inventory-management/design.md) · [Test checklist](test_plan.md) · [Pattern guide](../../docs/design-patterns.md)
