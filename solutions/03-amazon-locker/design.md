# Amazon Locker: Interview Design

[Question](../../questions/03-amazon-locker/README.md) · [Core code](solution.hpp) · [Tests](tests.cpp)

## OOP responsibilities and pattern

Main pattern: **Strategy**.

| Object | Responsibility |
| --- | --- |
| `Slot` | Numeric ID, size and occupancy. |
| `Locker` | Package assignments and single-use pickup codes. |
| `AllocationPolicy` | Abstract compatible-slot selection. |
| `SmallestFit` | Preserve larger slots by choosing the smallest fit. |

deposit asks the allocation Strategy for an index, stores an assignment and occupies the slot. pickup removes the assignment and releases that same slot.

## Ownership and scope

Locker owns slots and assignment values and borrows the allocation policy. Slot indexes remain valid because the slot vector is fixed after construction.

One in-memory locker bank with nonnegative package IDs. Construction supplies unique, initially empty slots. AllocationPolicy returns an available compatible slot index or -1. Sequential numeric codes demonstrate lookup, not secure authentication.

## Small usage example

Within the example's namespace:

```cpp
SmallestFit allocation;
Locker locker({Slot(1, Size::large), Slot(2, Size::small)}, allocation);
int code = locker.deposit(100, Size::small);
int package = locker.pickup(code); // 100
```

Allocation/duplicate checking is O(slots + assignments); map lookup is O(log assignments).

## Follow-up discussion

- Add expiration, preserving physical occupancy until a courier removes the package.
- Add secure codes and an Adapter for actual door hardware; notify clients with Observer.

[Pattern map and catalog](../../docs/design-patterns.md)
