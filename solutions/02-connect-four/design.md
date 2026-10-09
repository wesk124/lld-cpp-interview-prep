# Connect Four: Reference Design

[Question](../../questions/02-connect-four/README.md) · [Tests](tests.cpp)

## Responsibilities, ownership, and invariants

Game owns the board, move count, current turn, and terminal status. Move is a value snapshot, not a pointer into the board.

A drop first validates the game and column, then finds the lowest empty cell. Illegal moves cannot mutate board/turn state.

Only lines through the newest piece can create a new win. Count both directions on four axes, checking boundaries before indexing.

Drop is O(R + K), with R rows and K winning length. Board storage is O(R×C); draw detection uses a move counter rather than rescanning the board.

The model has no UI dependencies and is deliberately single-threaded. A network server would serialize moves and validate player identity before calling it.

At terminal state, status is authoritative; next_player retains the final mover and is not a request to play again.


## Scope

Single-threaded rules engine only. No UI, AI, network play, or undo. Rows and columns are zero-based; row 0 is the top. next_player is meaningful only while playing.

## Extend it yourself

- Add move history and undo.
- Generalize to arbitrary connect-K rules.
- Add an AI player behind a move-selection interface.
