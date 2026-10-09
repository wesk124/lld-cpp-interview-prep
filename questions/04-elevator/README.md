# Elevator

A 45–60-minute OOP exercise. Main pattern: **Strategy**.

## Core interview contract

1. Validate stop floors and coalesce repeated requests.
2. A step closes open doors without moving; otherwise it services the current floor or moves one floor.
3. Use LOOK: continue toward pending stops ahead, then reverse.
4. Bank requests use a DispatchPolicy; nearest and least-busy policies demonstrate substitution.

## Scope and assumptions

Single-threaded discrete simulation with valid initial floors in a nonnegative building range. The bank is nonempty. It models destination requests, not passengers or safety-certified hardware.

## Interviewee TODOs

- [ ] Encapsulate car state and door/movement transitions.
- [ ] Implement LOOK using an ordered stop set.
- [ ] Separate bank dispatch Strategy from the car's movement algorithm.
- [ ] Explain ownership and the pattern's participating objects before coding.
- [ ] Test the main workflow, one boundary and one failure; discuss one follow-up.

## Run your attempt

Complete [starter.hpp](starter.hpp). It supplies interfaces and TODOs, not a solution. Run from the repository root:

```bash
bash scripts/test.sh 04-elevator practice
```

Or use CMake with `-DLLD_PRACTICE_EXAMPLE=04-elevator` and build/run `practice_tests`. The unfinished starter compiles but fails with TODO messages; it never includes the answer.

## Follow-ups for discussion

- Add direction-aware hall calls, capacity and a waiting-time policy.
- Use State for emergency/maintenance behavior and Observer for floor displays.

[After your attempt: design](../../solutions/04-elevator/design.md) · [Test checklist](test_plan.md) · [Pattern guide](../../docs/design-patterns.md)
