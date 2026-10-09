# Elevator: Interview Design

[Question](../../questions/04-elevator/README.md) · [Core code](solution.hpp) · [Tests](tests.cpp)

## OOP responsibilities and pattern

Main pattern: **Strategy**.

| Object | Responsibility |
| --- | --- |
| `Elevator` | Floor, direction, doors and pending stops. |
| `ElevatorBank` | Own cars and route requests. |
| `DispatchPolicy` | Abstract car selection. |
| `NearestCar / LeastBusyCar` | Alternative selection algorithms. |

A bank request selects one car through policy.choose and enqueues a stop. step_all advances every car independently.

## Ownership and scope

The bank owns its cars by value and borrows the dispatch policy. car(index) returns a read-only reference tied to the bank's lifetime.

Single-threaded discrete simulation with valid initial floors in a nonnegative building range. The bank is nonempty. It models destination requests, not passengers or safety-certified hardware.

## Small usage example

Within the example's namespace:

```cpp
NearestCar policy;
ElevatorBank bank(10, {0, 8}, policy);
std::size_t car = bank.request(2); // 0
bank.step_all(); // car 0 moves to floor 1
```

Stop insertion is O(log pending); nearest/least-busy selection is O(cars). Idle target selection scans pending stops.

## Follow-up discussion

- Add direction-aware hall calls, capacity and a waiting-time policy.
- Use State for emergency/maintenance behavior and Observer for floor displays.

[Pattern map and catalog](../../docs/design-patterns.md)
