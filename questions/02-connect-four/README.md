# Connect Four

A 45–60-minute OOP exercise. Main pattern: **Strategy**.

## Core interview contract

1. Use R/Y pieces on a board initially filled with dots; a drop lands at the lowest free row.
2. Reject invalid columns, full columns and moves after a win/draw with false, without changing the turn.
3. Check horizontal, vertical and diagonal lines through the newest piece.
4. Delegate the winning condition through WinRule; configure ConnectKRule for connect-four or connect-three.

## Scope and assumptions

Single-threaded rules engine with no UI, AI or network transport. Row 0 is the top. A small configurable board supports deterministic tests; next_player is meaningful while playing.

## Interviewee TODOs

- [ ] Model board, turn and terminal status inside Game.
- [ ] Implement gravity and directional matching around the last move.
- [ ] Invoke the WinRule Strategy instead of putting its algorithm inside Game.
- [ ] Explain ownership and the pattern's participating objects before coding.
- [ ] Test the main workflow, one boundary and one failure; discuss one follow-up.

## Run your attempt

Complete [starter.hpp](starter.hpp). It supplies interfaces and TODOs, not a solution. Run from the repository root:

```bash
bash scripts/test.sh 02-connect-four practice
```

Or use CMake with `-DLLD_PRACTICE_EXAMPLE=02-connect-four` and build/run `practice_tests`. The unfinished starter compiles but fails with TODO messages; it never includes the answer.

## Follow-ups for discussion

- Add a move-selection Strategy for human/AI decisions.
- Add Command-based undo or a Memento; discuss when a richer lifecycle would benefit from State objects.

[After your attempt: design](../../solutions/02-connect-four/design.md) · [Test checklist](test_plan.md) · [Pattern guide](../../docs/design-patterns.md)
