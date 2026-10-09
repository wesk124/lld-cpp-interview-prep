# Rate Limiter: Interview Design

[Question](../../questions/08-rate-limiter/README.md) · [Core code](solution.hpp) · [Tests](tests.cpp)

## OOP responsibilities and pattern

Main pattern: **Strategy**.

| Object | Responsibility |
| --- | --- |
| `RateLimiter` | Common admission interface. |
| `TokenBucket` | Continuous refill, capacity cap and weighted consumption. |
| `FixedWindow` | Window-based admission-count alternative. |
| `RequestGate` | Client using the selected limiter Strategy. |

RequestGate delegates allow. The selected policy finds client state, advances time, checks remaining capacity and consumes cost under its mutex.

## Ownership and scope

Each concrete policy owns its client state and mutex. RequestGate borrows RateLimiter&. There is no clock service; tests supply simple millisecond values.

One process with finite positive configurations and normal input magnitudes. Each policy keeps client state in memory. FixedWindow anchors a client's first window at its first request; after expiry the next request starts a new window. Algorithms have different quota semantics.

## Small usage example

Within the example's namespace:

```cpp
TokenBucket limiter(5, 2.0);
RequestGate gate(limiter);
bool first = gate.admit(7, 0, 5); // true
bool early = gate.admit(7, 0);    // false
bool later = gate.admit(7, 500);  // true
```

Expected O(1) admission per client with unordered_map, and O(clients) storage. Each policy uses one coarse lock.

## Follow-up discussion

- Add retry-after information or idle-client eviction.
- Add a distributed backend and discuss failure behavior; wrap a service with a Decorator if admission is an extra service behavior.

[Pattern map and catalog](../../docs/design-patterns.md)
