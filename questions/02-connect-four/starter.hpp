#pragma once

#include <map>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lld {
namespace connect_four {

[[noreturn]] inline void todo(const char* task) {
    throw std::logic_error(std::string("TODO: ") + task);
}

using Board = std::vector<std::vector<char>>;
enum class Status { playing, red_won, yellow_won, draw };
class WinRule {
public:
    virtual ~WinRule() {}
    virtual bool wins(const Board& board, int row, int column) const = 0;
};
class ConnectKRule : public WinRule {
public:
    explicit ConnectKRule(int /*length*/ = 4) { todo("store winning length"); }
    bool wins(const Board& /*board*/, int /*row*/, int /*column*/) const override {
        todo("count matching pieces in both directions on four axes");
    }
};
class Game {
public:
    Game(const WinRule& /*rule*/, int /*rows*/ = 6, int /*columns*/ = 7) {
        todo("initialize board and turn; borrow the winning-rule Strategy");
    }
    bool drop(int /*column*/) { todo("apply gravity, evaluate rule, update status and turn"); }
    char cell(int /*row*/, int /*column*/) const { todo("return a board cell"); }
    char next_player() const { todo("return current player"); }
    Status status() const { todo("return game status"); }
};

} // namespace connect_four
} // namespace lld
