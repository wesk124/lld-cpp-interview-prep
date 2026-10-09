# Inventory Management

Design a stock and reservation service for an inventory catalog.

## Interview contract

1. Create unique SKUs with nonnegative on-hand stock and receive positive replenishments.
2. Track reserved stock separately; available = on_hand − reserved.
3. Reserve all SKUs in an order atomically; insufficient or unknown stock returns false without changing any SKU.
4. Use order ID as an idempotency key: matching retries of held/committed orders succeed without re-reserving; different payloads throw; released orders cannot be reserved again.
5. Commit held orders exactly once by decreasing on-hand and reserved quantities; release held orders exactly once by decreasing reserved only.

## Scope and assumptions

One in-memory catalog, no warehouses, prices, shipment, payment, persistence, or reservation timeout. Unknown order commit/release returns false. Integer overflow throws. Completed order records remain available for retry deduplication.

## Interviewee TODOs

- [ ] State and enforce 0 <= reserved <= on_hand.
- [ ] Validate every requested quantity before changing any stock.
- [ ] Keep held/committed/released states with stable request payloads.
- [ ] Make reserve/commit/release and receipt arithmetic safe under a shared lock.
- [ ] Test failed multi-item reservations and conflicting/idempotent retries.
- [ ] Explain object ownership, invariants, and error handling before writing code.
- [ ] Run the practice tests and discuss at least one alternative design.

## OOP and pattern discussion

- [ ] Explain Stock value semantics, reservation ownership, and why the inventory coordinates invariants across an entire order.
- [ ] Explore warehouse-allocation Strategy or richer reservation State objects, identifying the responsibilities that remain in inventory accounting.
- [ ] Discuss Observer for low-stock events and callback behavior after releasing the inventory lock.

[Pattern guide](../../docs/design-patterns.md). These discussion extensions are separate from the base test contract.

## Run your attempt

Complete the marked methods in [starter.hpp](starter.hpp). The starting code compiles but deliberately throws `TODO` errors until implemented. It does not include or link the reference implementation.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=09-inventory-management
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Without CMake:

```bash
bash scripts/test.sh 09-inventory-management practice
```

Run commands from the repository root. The tests are the same behavioral contract as the reference solution; incomplete attempts are expected to fail them.

## Follow-ups

1. Add multiple warehouses and transfer workflows.
2. Add reservation expiration and low-stock notifications.
3. Map operations to database transactions and durable idempotency records.

After your attempt: [design explanation](../../solutions/09-inventory-management/design.md) · [test checklist](test_plan.md) · [example quiz](../../quizzes/examples-questions.md).
