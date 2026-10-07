# Connect Four

Design the core engine for a two-player Connect Four game, separate from any UI.

## Interview contract

1. Default to a 6×7 board and four consecutive pieces; allow configurable positive dimensions and a valid winning length.
2. Red starts. A drop lands in the lowest empty row of the selected column.
3. Detect horizontal, vertical, and both diagonal wins from the latest move.
4. Declare a draw when the board fills without a winner.
5. Reject out-of-range columns, full columns, and moves after the game ends without advancing the turn.

## Scope and assumptions

Single-threaded rules engine only. No UI, AI, network play, or undo. Rows and columns are zero-based; row 0 is the top. next_player is meaningful only while playing.

## Interviewee TODOs

- [ ] Choose board storage and strongly typed cells/game status.
- [ ] Validate constructor input and checked cell access.
- [ ] Implement gravity and alternating turns.
- [ ] Count matching neighbors in both directions along four axes.
- [ ] Separate terminal status from the next playable turn.
- [ ] Explain object ownership, invariants, and error handling before writing code.
- [ ] Run the practice tests and discuss at least one alternative design.

## Run your attempt (C++17)

Complete the marked methods in [starter.hpp](starter.hpp). The starting code compiles but deliberately throws `TODO` errors until implemented. It does not include or link the reference implementation.

```bash
cmake -S . -B build-practice -DLLD_PRACTICE_EXAMPLE=02-connect-four
cmake --build build-practice --target practice_tests
ctest --test-dir build-practice -R '^practice_tests$' --output-on-failure
```

Without CMake:

```bash
bash scripts/test.sh 02-connect-four practice
```

Run commands from the repository root. The tests are the same behavioral contract as the reference solution; incomplete attempts are expected to fail them.

## Follow-ups

1. Add move history and undo.
2. Generalize to arbitrary connect-K rules.
3. Add an AI player behind a move-selection interface.

After your attempt: [design explanation](../../solutions/02-connect-four/design.md) · [test checklist](test_plan.md) · [example quiz](../../quizzes/examples-questions.md).
