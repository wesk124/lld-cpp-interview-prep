#pragma once

#include <stdexcept>
#include <vector>

namespace lld {
namespace connect_four {

using Board = std::vector<std::vector<char>>;
enum class Status { playing, red_won, yellow_won, draw };

// Strategy: the game delegates the winning condition.
class WinRule {
public:
    virtual ~WinRule() {}
    virtual bool wins(const Board& board, int row, int column) const = 0;
};

class ConnectKRule : public WinRule {
public:
    explicit ConnectKRule(int length = 4) : length_(length) {
        if (length < 2) throw std::invalid_argument("winning length must be at least two");
    }

    bool wins(const Board& board, int row, int column) const override {
        const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
        for (const auto& direction : directions) {
            int count = 1 + count_direction(board, row, column, direction[0], direction[1])
                          + count_direction(board, row, column, -direction[0], -direction[1]);
            if (count >= length_) return true;
        }
        return false;
    }

private:
    int count_direction(const Board& board, int row, int column, int dr, int dc) const {
        char piece = board[static_cast<std::size_t>(row)][static_cast<std::size_t>(column)];
        int count = 0;
        for (int step = 1; step < length_; ++step) {
            int next_row = row + step * dr, next_column = column + step * dc;
            if (next_row < 0 || next_column < 0 ||
                next_row >= static_cast<int>(board.size()) ||
                next_column >= static_cast<int>(board[0].size()) ||
                board[static_cast<std::size_t>(next_row)][static_cast<std::size_t>(next_column)] != piece)
                break;
            ++count;
        }
        return count;
    }

    int length_;
};

class Game {
public:
    Game(const WinRule& rule, int rows = 6, int columns = 7)
        : rule_(rule), turn_('R'), status_(Status::playing), moves_(0) {
        if (rows <= 0 || columns <= 0) throw std::invalid_argument("invalid board size");
        board_.assign(static_cast<std::size_t>(rows),
                      std::vector<char>(static_cast<std::size_t>(columns), '.'));
    }

    bool drop(int column) {
        if (status_ != Status::playing || column < 0 ||
            column >= static_cast<int>(board_[0].size())) return false;
        int row = static_cast<int>(board_.size()) - 1;
        while (row >= 0 && cell(row, column) != '.') --row;
        if (row < 0) return false;

        board_[static_cast<std::size_t>(row)][static_cast<std::size_t>(column)] = turn_;
        ++moves_;
        if (rule_.wins(board_, row, column))
            status_ = turn_ == 'R' ? Status::red_won : Status::yellow_won;
        else if (moves_ == static_cast<int>(board_.size() * board_[0].size()))
            status_ = Status::draw;
        else
            turn_ = turn_ == 'R' ? 'Y' : 'R';
        return true;
    }

    char cell(int row, int column) const {
        return board_.at(static_cast<std::size_t>(row)).at(static_cast<std::size_t>(column));
    }
    char next_player() const { return turn_; }
    Status status() const { return status_; }

private:
    Board board_;
    const WinRule& rule_; // Caller keeps this rule alive.
    char turn_;
    Status status_;
    int moves_;
};

} // namespace connect_four
} // namespace lld
