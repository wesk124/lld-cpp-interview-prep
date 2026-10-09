# Rate Limiter

A 45–60-minute OOP exercise. Main pattern: **Strategy**.

## Core interview contract

1. Identify clients with int IDs and supply nonnegative long long millisecond timestamps.
2. Start token buckets full; continuously refill at tokens/second, cap at capacity and consume a positive cost only if available.
3. Reject invalid cost or backward time with false; isolate client state.
4. Keep refill/check/consume inside one mutex; expose both policies through RateLimiter.

## Scope and assumptions

One process with finite positive configurations and normal input magnitudes. Each policy keeps client state in memory. FixedWindow anchors a client's first window at its first request; after expiry the next request starts a new window. Algorithms have different quota semantics.

## Interviewee TODOs

- [ ] Implement token-bucket admission as one short protected operation.
- [ ] Use the abstract interface in RequestGate: Strategy.
- [ ] Implement the small fixed-window alternative after the token-bucket core; compare their guarantees.
- [ ] Explain ownership and the pattern's participating objects before coding.
- [ ] Test the main workflow, one boundary and one failure; discuss one follow-up.

## Run your attempt

Complete [starter.hpp](starter.hpp). It supplies interfaces and TODOs, not a solution. Run from the repository root:

```bash
bash scripts/test.sh 08-rate-limiter practice
```

Or use CMake with `-DLLD_PRACTICE_EXAMPLE=08-rate-limiter` and build/run `practice_tests`. The unfinished starter compiles but fails with TODO messages; it never includes the answer.

## Follow-ups for discussion

- Add retry-after information or idle-client eviction.
- Add a distributed backend and discuss failure behavior; wrap a service with a Decorator if admission is an extra service behavior.

[After your attempt: design](../../solutions/08-rate-limiter/design.md) · [Test checklist](test_plan.md) · [Pattern guide](../../docs/design-patterns.md)
