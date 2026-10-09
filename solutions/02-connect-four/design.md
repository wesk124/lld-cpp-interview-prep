# Connect Four: Interview Design

[Question](../../questions/02-connect-four/README.md) · [Core code](solution.hpp) · [Tests](tests.cpp)

## OOP responsibilities and pattern

Main pattern: **Strategy**.

| Object | Responsibility |
| --- | --- |
| `Game` | Own the board, turn, move count and terminal status. |
| `WinRule` | Abstract winning-condition operation. |
| `ConnectKRule` | Count matching pieces along four axes. |

drop validates the column, places one piece, asks rule.wins, then updates status or switches the turn.

## Ownership and scope

Game owns the board and borrows const WinRule&. The rule outlives the game and treats the supplied board as read-only.

Single-threaded rules engine with no UI, AI or network transport. Row 0 is the top. A small configurable board supports deterministic tests; next_player is meaningful while playing.

## Small usage example

Within the example's namespace:

```cpp
ConnectKRule rule(4);
Game game(rule);
for (int column : {0, 6, 1, 6, 2, 6, 3}) game.drop(column);
// game.status() == Status::red_won
```

A drop scans O(rows) for gravity and O(K) for its rule; storage is O(rows × columns).

## Follow-up discussion

- Add a move-selection Strategy for human/AI decisions.
- Add Command-based undo or a Memento; discuss when a richer lifecycle would benefit from State objects.

[Pattern map and catalog](../../docs/design-patterns.md)
