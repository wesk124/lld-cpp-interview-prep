# Connect Four: Reference Design

[Question](../../questions/02-connect-four/README.md) · [Tests](tests.cpp)

## Responsibilities, ownership, and invariants

Game owns the board, move count, current turn, and terminal status. Move is a value snapshot, not a pointer into the board.

A drop first validates the game and column, then finds the lowest empty cell. Illegal moves cannot mutate board/turn state.

Only lines through the newest piece can create a new win. Count both directions on four axes, checking boundaries before indexing.

Drop is O(R + K), with R rows and K winning length. Board storage is O(R×C); draw detection uses a move counter rather than rescanning the board.

The model has no UI dependencies and is deliberately single-threaded. A network server would serialize moves and validate player identity before calling it.

At terminal state, status is authoritative; next_player retains the final mover and is not a request to play again.


## OOP and design patterns

- **Current OOP design:** `Game` encapsulates the board, turn, move count, and terminal status. A move result is a value snapshot. The enum-based lifecycle is a state machine; it does not currently delegate behavior to GoF State objects.
- **Strategy (follow-up):** A move-selection interface could let human and AI players choose columns while `Game` remains responsible for legality and outcomes.
- **Command (follow-up):** A drop command could represent an executable move and retain enough prior state for undo. A Memento is another option for restoring a complete game snapshot.
- **State (follow-up):** Separate state objects could organize playing/finished behavior if replay, pause, or network-session rules make it complex enough to benefit from delegation.

[Pattern map and catalog](../../docs/design-patterns.md)

## Scope

Single-threaded rules engine only. No UI, AI, network play, or undo. Rows and columns are zero-based; row 0 is the top. next_player is meaningful only while playing.

## Extend it yourself

- Add move history and undo.
- Generalize to arbitrary connect-K rules.
- Add an AI player behind a move-selection interface.
