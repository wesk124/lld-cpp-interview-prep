# Rate Limiter

Design an in-process per-client token-bucket rate limiter.

## Interview contract

1. Configure positive integer burst capacity and a finite positive refill rate.
2. Start a new client with a full bucket. Refill continuously and cap at capacity.
3. Consume a positive integer request cost only when enough tokens exist; never borrow future tokens.
4. Isolate client buckets; reject backward time for an existing client.
5. Serialize refill plus consume so concurrent callers cannot exceed the available burst.

## Scope and assumptions

Single-process token bucket, not a distributed limiter or strict fixed/sliding-window quota. Inject steady_clock time; timestamps must be nondecreasing for each client. Requests whose cost exceeds capacity return false. Client entries are retained; floating-point token counts are approximate.

## Interviewee TODOs

- [ ] Store fractional tokens and last-refill time per client.
- [ ] Validate configuration, client identity, and request cost.
- [ ] Cap refill after long idle periods.
- [ ] Make admission one atomic state transition.
- [ ] Test partial refills, weighted requests, clients, rollback, and simultaneous admission.
- [ ] Explain object ownership, invariants, and error handling before writing code.
- [ ] Run the practice tests and discuss at least one alternative design.

## Run your attempt

Complete the marked methods in [starter.hpp](starter.hpp). The starting code compiles but deliberately throws `TODO` errors until implemented. It does not include or link the reference implementation.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=08-rate-limiter
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Without CMake:

```bash
bash scripts/test.sh 08-rate-limiter practice
```

Run commands from the repository root. The tests are the same behavioral contract as the reference solution; incomplete attempts are expected to fail them.

## Follow-ups

1. Return retry-after durations.
2. Evict idle buckets without allowing unintended fresh bursts.
3. Implement a distributed admission backend and compare failure semantics.

After your attempt: [design explanation](../../solutions/08-rate-limiter/design.md) · [test checklist](test_plan.md) · [example quiz](../../quizzes/examples-questions.md).
