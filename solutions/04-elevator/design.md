# Elevator: Reference Design

[Question](../../questions/04-elevator/README.md) · [Tests](tests.cpp)

## Responsibilities, ownership, and invariants

Elevator owns the car state and ordered stop set. ElevatorBank owns cars by value and an exclusive DispatchPolicy, and serializes external access with one mutex.

A car closes open doors before any movement; servicing the current floor also consumes a tick. Movement and opening at the arrival floor form one transition.

LOOK continues in the existing direction until no pending destination remains ahead, then reverses. An idle car chooses its nearest destination with a lower-floor tie break.

NearestCarPolicy chooses by distance and stable car-index tie breaking. Its interface accepts hall direction so a richer policy can replace it.

Request insertion is O(log P) for P stops. The step implementation scans for an idle target and copies a snapshot, so O(P). Bank dispatch is O(C + total pending stops) including snapshots.

No claim is made about real elevator safety or passenger fairness. Requests can starve under continuous arrivals; production routing needs directional hall queues and bounded waiting.


## OOP and design patterns

- **Strategy (implemented):** `ElevatorBank` is the context, `DispatchPolicy` defines car selection, and `NearestCarPolicy` is one implementation. The bank owns its policy and cars; dispatch can vary independently of each car's movement rules.
- **OOP responsibilities:** `Elevator` encapsulates stops, direction, floor, and doors. `ElevatorBank` coordinates access to multiple cars. LOOK is a scheduling algorithm; the current enum-based transitions are not GoF State delegation.
- **State (follow-up):** Normal, emergency, and maintenance state objects could define permitted requests and actions, with explicit door/movement transition rules.
- **Observer (follow-up):** Displays and arrival listeners could consume car events without being part of the car's movement logic.

[Pattern map and catalog](../../docs/design-patterns.md)

## Scope

A simulation, not safety-certified hardware. Single-car Elevator access must be externally serialized; ElevatorBank provides thread-safe orchestration. The nearest-car policy ignores hall direction after validation, and stop requests represent destinations rather than passengers. No capacity, emergency controls, or starvation guarantee.

## Extend it yourself

- Make dispatch direction-aware and account for pending workload.
- Separate up/down hall queues and enforce bounded waiting.
- Model capacity, emergency stops, and hardware door sensors.
