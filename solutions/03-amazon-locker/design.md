# Amazon Locker: Reference Design

[Question](../../questions/03-amazon-locker/README.md) · [Tests](tests.cpp)

## Responsibilities, ownership, and invariants

Slot describes stable capacity; Assignment records one active package session; Locker coordinates physical occupancy and pickup-code lookup.

Slots and assignment records are owned values. A supplied std::function creates codes; security is an external concern, not a claim about the deterministic test generator.

Optional results use the small [C++11 helper](../../common/optional.hpp), which owns its value and returns an empty result when a request cannot be fulfilled. Deposit stages the result and map entry before publishing occupancy. One mutex covers allocation, assignment validity, pickup, and collection.

Expiry revokes customer pickup but does not remove a physical package. Only courier collection frees expired occupied slots.

A nondecreasing injected steady clock makes exact-deadline tests deterministic. The sample bounds TTL to 24 hours and rejects deadline overflow.

With S slots and A assignments, allocation is O(S + A + log A), pickup O(S + log A), collection O(A×S), space O(S+A). Replace scans/indexes when actual scale warrants it.


## OOP and design patterns

- **Strategy via a callable (implemented):** `Locker` is the context and its supplied `std::function<std::string()>` is the replaceable code-generation behavior. This is dependency injection without a dedicated strategy class hierarchy. Smallest-compatible-slot allocation remains fixed in `deposit`.
- **OOP responsibilities:** `Slot` describes capacity, `Assignment` describes a package session, and `Locker` coordinates occupancy and code validity. Expiration and physical package removal have distinct meanings.
- **Strategy / Adapter (follow-ups):** An allocation policy could choose slots; a door-controller adapter could translate a hardware API into domain operations. These isolate different sources of change.
- **Observer / State (follow-ups):** Package events could notify clients; state objects could organize a richer lifecycle while preserving the fact that expired packages still occupy slots.

[Pattern map and catalog](../../docs/design-patterns.md)

## Scope

One in-memory bank; no actual Amazon API, hardware doors, authentication, notifications, or production security. TTL is 1 second through 24 hours. Inject steady_clock time points in nondecreasing processing order. The generator must be bounded and non-reentrant; predictable codes in tests are not production-safe.

## Extend it yourself

- Introduce door-controller and notification interfaces.
- Add secure code generation, authentication, and attempt throttling.
- Add multiple banks and courier audit logs.
