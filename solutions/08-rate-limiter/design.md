# Rate Limiter: Reference Design

[Question](../../questions/08-rate-limiter/README.md) · [Tests](tests.cpp)

## Responsibilities, ownership, and invariants

TokenBucketLimiter owns one Bucket per client, containing fractional tokens and last_refill. The limiter validates request/configuration values before consumption.

A single mutex spans lookup, refill, cap, and subtract. This is the admission linearization boundary; independent locks around refill and consume would permit over-admission.

Injected steady-clock values make time-sensitive behavior testable. Backward per-client time is rejected instead of increasing tokens or corrupting refill history.

Refill is min(capacity, old_tokens + elapsed_seconds×rate). A denial never subtracts tokens; the refill timestamp still advances. A request larger than total capacity is immediately denied.

A std::map makes each admission O(log C) for C clients and storage O(C). Floating-point arithmetic is convenient for interview scale but not an exact accounting system.

There is no eviction, persistence, or cross-process coordination. Token buckets bound burst and long-term rate, not the exact request count in every fixed window.


## OOP and design patterns

- **Current OOP design:** `TokenBucketLimiter` encapsulates client buckets and the atomic lookup/refill/consume operation. Token bucket is an algorithm; the reference has no interface for selecting alternative algorithms and therefore no Strategy collaboration yet.
- **Strategy (follow-up):** A `RateLimitPolicy` interface could expose admission, with token-bucket and sliding-window implementations supplied to a calling service. The contract would describe costs, time semantics, and each algorithm's quota guarantees.
- **Decorator (follow-up):** A wrapper implementing a service's interface could consult a limiter before forwarding to an inner service. This separates admission checks from the service's main behavior.
- **Ownership discussion:** Define whether the wrapper owns its policy and inner service or borrows longer-lived collaborators; per-client mutable state still belongs to the chosen policy.

[Pattern map and catalog](../../docs/design-patterns.md)

## Scope

Single-process token bucket, not a distributed limiter or strict fixed/sliding-window quota. Inject steady_clock time; timestamps must be nondecreasing for each client. Requests whose cost exceeds capacity return false. Client entries are retained; floating-point token counts are approximate.

## Extend it yourself

- Return retry-after durations.
- Evict idle buckets without allowing unintended fresh bursts.
- Implement a distributed admission backend and compare failure semantics.
