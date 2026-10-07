# Elevator

Design a discrete elevator simulator with multiple cars and replaceable hall-call dispatch.

## Interview contract

1. Floors range from 0 to top_floor. A simulation tick moves a car at most one floor.
2. Doors open at a requested stop; closing consumes a tick during which the car must not move.
3. Coalesce repeated stop requests. Serve current-direction stops before reversing (LOOK).
4. An idle car picks its closest pending stop, breaking equal distances toward the lower floor.
5. A bank assigns hall calls to the nearest car, breaking ties by car index; internal buttons target a specific car.

## Scope and assumptions

A simulation, not safety-certified hardware. Single-car Elevator access must be externally serialized; ElevatorBank provides thread-safe orchestration. The nearest-car policy ignores hall direction after validation, and stop requests represent destinations rather than passengers. No capacity, emergency controls, or starvation guarantee.

## Interviewee TODOs

- [ ] Model floor, direction, doors, and pending requests independently.
- [ ] Implement LOOK scheduling and explicit open/close/move transitions.
- [ ] Define a dispatch interface and own the selected policy.
- [ ] Validate floor and hall direction boundaries.
- [ ] Serialize bank requests/ticks and return snapshots by value.
- [ ] Explain object ownership, invariants, and error handling before writing code.
- [ ] Run the practice tests and discuss at least one alternative design.

## Run your attempt (C++17)

Complete the marked methods in [starter.hpp](starter.hpp). The starting code compiles but deliberately throws `TODO` errors until implemented. It does not include or link the reference implementation.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=04-elevator
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Without CMake:

```bash
bash scripts/test.sh 04-elevator practice
```

Run commands from the repository root. The tests are the same behavioral contract as the reference solution; incomplete attempts are expected to fail them.

## Follow-ups

1. Make dispatch direction-aware and account for pending workload.
2. Separate up/down hall queues and enforce bounded waiting.
3. Model capacity, emergency stops, and hardware door sensors.

After your attempt: [design explanation](../../solutions/04-elevator/design.md) · [test checklist](test_plan.md) · [example quiz](../../quizzes/examples-questions.md).
