# Amazon Locker

A 45–60-minute OOP exercise. Main pattern: **Strategy**.

## Core interview contract

1. Deposit a package into the smallest available compatible slot.
2. Use nonnegative numeric package IDs and demonstration pickup codes; return -1 when allocation fails.
3. Allow one active assignment per package ID.
4. Pickup returns the package ID, frees the slot and invalidates the code.

## Scope and assumptions

One in-memory locker bank. Construction supplies unique, initially empty slots. AllocationPolicy returns an available compatible slot index or -1. Sequential numeric codes demonstrate lookup, not secure authentication.

## Interviewee TODOs

- [ ] Implement SmallestFit against a read-only slot list.
- [ ] Keep occupancy and code-to-package assignments consistent.
- [ ] Delegate allocation through AllocationPolicy; implement deposit and one-time pickup.
- [ ] Explain ownership and the pattern's participating objects before coding.
- [ ] Test the main workflow, one boundary and one failure; discuss one follow-up.

## Run your attempt

Complete [starter.hpp](starter.hpp). It supplies interfaces and TODOs, not a solution. Run from the repository root:

```bash
bash scripts/test.sh 03-amazon-locker practice
```

Or use CMake with `-DLLD_PRACTICE_EXAMPLE=03-amazon-locker` and build/run `practice_tests`. The unfinished starter compiles but fails with TODO messages; it never includes the answer.

## Follow-ups for discussion

- Add expiration, preserving physical occupancy until a courier removes the package.
- Add secure codes and an Adapter for actual door hardware; notify clients with Observer.

[After your attempt: design](../../solutions/03-amazon-locker/design.md) · [Test checklist](test_plan.md) · [Pattern guide](../../docs/design-patterns.md)
