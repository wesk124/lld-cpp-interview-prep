#pragma once

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace lld {
namespace connect_four {

enum class Cell { empty, red, yellow };
enum class Status { playing, red_won, yellow_won, draw };
struct Move {
    int row;
    int column;
    Cell player;
    Status status;
};

class Game {
public:
    explicit Game(int rows = 6, int columns = 7, int connect = 4)
        : rows_(rows), columns_(columns), connect_(connect) {
        if (rows <= 0 || columns <= 0 || connect < 2 || connect > std::max(rows, columns)) {
            throw std::invalid_argument("invalid board dimensions or winning length");
        }
        board_.assign(static_cast<std::size_t>(rows),
                      std::vector<Cell>(static_cast<std::size_t>(columns), Cell::empty));
    }

    Cell cell(int row, int column) const {
        if (!in_bounds(row, column)) {
            throw std::out_of_range("cell outside board");
        }
        return board_[static_cast<std::size_t>(row)][static_cast<std::size_t>(column)];
    }

    Cell next_player() const noexcept { return turn_; }
    Status status() const noexcept { return status_; }

    Move drop(int column) {
        if (column < 0 || column >= columns_) {
            throw std::out_of_range("column outside board");
        }
        if (status_ != Status::playing) {
            throw std::logic_error("game has ended");
        }
        int row = rows_ - 1;
        while (row >= 0 && cell(row, column) != Cell::empty) {
            --row;
        }
        if (row < 0) {
            throw std::logic_error("column is full");
        }
        const Cell player = turn_;
        board_[static_cast<std::size_t>(row)][static_cast<std::size_t>(column)] = player;
        ++moves_;
        const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
        for (const auto& direction : directions) {
            const int length = 1 + count(row, column, direction[0], direction[1], player) +
                               count(row, column, -direction[0], -direction[1], player);
            if (length >= connect_) {
                status_ = player == Cell::red ? Status::red_won : Status::yellow_won;
                break;
            }
        }
        if (status_ == Status::playing && moves_ == board_.size() * board_.front().size()) {
            status_ = Status::draw;
        }
        if (status_ == Status::playing) {
            turn_ = player == Cell::red ? Cell::yellow : Cell::red;
        }
        return Move{row, column, player, status_};
    }

private:
    bool in_bounds(int row, int column) const noexcept {
        return row >= 0 && row < rows_ && column >= 0 && column < columns_;
    }

    int count(int row, int column, int dr, int dc, Cell player) const {
        int result = 0;
        // At most connect_-1 neighbors are necessary in either direction.
        for (int i = 1; i < connect_; ++i) {
            row += dr;
            column += dc;
            if (!in_bounds(row, column) || cell(row, column) != player) {
                break;
            }
            ++result;
        }
        return result;
    }

    int rows_;
    int columns_;
    int connect_;
    std::vector<std::vector<Cell>> board_;
    std::size_t moves_{0};
    Cell turn_{Cell::red};
    Status status_{Status::playing};
};

}  // namespace connect_four
}  // namespace lld
