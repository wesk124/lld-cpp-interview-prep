#pragma once
#include <stdexcept>
#include <string>
namespace lld::connect_four {
enum class Cell { empty, red, yellow };
enum class Status { playing, red_won, yellow_won, draw };
struct Move { int row; int column; Cell player; Status status; };
[[noreturn]] inline void todo(const char* task) { throw std::logic_error(std::string("TODO: ") + task); }
class Game {
public:
    explicit Game(int /*rows*/ = 6, int /*columns*/ = 7, int /*connect*/ = 4) {
        todo("validate configuration and initialize an empty board");
    }
    Cell cell(int /*row*/, int /*column*/) const { todo("provide checked board access"); }
    Cell next_player() const { todo("expose the current turn"); }
    Status status() const { todo("expose terminal or active game state"); }
    Move drop(int /*column*/) {
        todo("validate, apply gravity, detect four directions, and transition turn/state");
    }
private:
    // TODO: Choose board storage, move count, turn, and terminal state.
    // TODO: Count contiguous matching pieces on both sides of the latest move.
};
}  // namespace lld::connect_four
