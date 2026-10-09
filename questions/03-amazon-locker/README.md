# Amazon Locker

Design an Amazon Locker-inspired package pickup system for a single locker bank.

## Interview contract

1. Slots have unique IDs and small/medium/large capacities. Allocate the smallest compatible free slot.
2. Deposit a uniquely active package and return its slot, pickup code, and expiration.
3. Inject a code generator; reject empty or duplicate active codes without occupying a slot.
4. Pickup requires an unexpired code; successful codes are single-use.
5. Expired packages remain physically present until collect_expired is called by the courier workflow.

## Scope and assumptions

One in-memory bank; no actual Amazon API, hardware doors, authentication, notifications, or production security. TTL is 1 second through 24 hours. Inject steady_clock time points in nondecreasing processing order. The generator must be bounded and non-reentrant; predictable codes in tests are not production-safe.

## Interviewee TODOs

- [ ] Keep physical occupancy distinct from code validity.
- [ ] Maintain consistent assignment/slot identities under concurrent requests.
- [ ] Inject and validate code generation instead of hardcoding a global counter.
- [ ] Enforce exact expiration boundaries and deterministic time.
- [ ] Handle collection without releasing an uncollected package's slot.
- [ ] Explain object ownership, invariants, and error handling before writing code.
- [ ] Run the practice tests and discuss at least one alternative design.

## Run your attempt

Complete the marked methods in [starter.hpp](starter.hpp). The starting code compiles but deliberately throws `TODO` errors until implemented. It does not include or link the reference implementation.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=03-amazon-locker
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Without CMake:

```bash
bash scripts/test.sh 03-amazon-locker practice
```

Run commands from the repository root. The tests are the same behavioral contract as the reference solution; incomplete attempts are expected to fail them.

## Follow-ups

1. Introduce door-controller and notification interfaces.
2. Add secure code generation, authentication, and attempt throttling.
3. Add multiple banks and courier audit logs.

After your attempt: [design explanation](../../solutions/03-amazon-locker/design.md) · [test checklist](test_plan.md) · [example quiz](../../quizzes/examples-questions.md).
