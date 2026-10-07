#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"

using namespace lld::connect_four;

int main() {
    test::Suite suite;
    suite.run("gravity and alternating turns", [] {
        Game game;
        const auto first = game.drop(2);
        CHECK(first.row == 5 && first.player == Cell::red);
        CHECK(game.next_player() == Cell::yellow);
        CHECK(game.drop(2).row == 4);
        CHECK(game.cell(5, 2) == Cell::red);
    });
    suite.run("horizontal win", [] {
        Game game;
        for (int column : {0, 0, 1, 1, 2, 2, 3}) { game.drop(column); }
        CHECK(game.status() == Status::red_won);
        EXPECT_THROW(std::logic_error, game.drop(4));
    });
    suite.run("vertical win", [] {
        Game game;
        for (int column : {0, 1, 0, 1, 0, 1, 0}) { game.drop(column); }
        CHECK(game.status() == Status::red_won);
    });
    suite.run("rising diagonal win", [] {
        Game game;
        for (int column : {0, 1, 1, 2, 4, 2, 2, 3, 4, 3, 5, 3, 3}) { game.drop(column); }
        CHECK(game.status() == Status::red_won);
    });
    suite.run("falling diagonal win", [] {
        Game game;
        for (int column : {6, 5, 5, 4, 2, 4, 4, 3, 2, 3, 1, 3, 3}) { game.drop(column); }
        CHECK(game.status() == Status::red_won);
    });
    suite.run("full column rejection does not advance turn", [] {
        Game game;
        for (int i = 0; i < 6; ++i) { game.drop(0); }
        const Cell turn = game.next_player();
        EXPECT_THROW(std::logic_error, game.drop(0));
        CHECK(game.next_player() == turn);
    });
    suite.run("draw on a configurable board", [] {
        Game game(2, 3, 3);
        for (int column : {0, 1, 2, 0, 1, 2}) { game.drop(column); }
        CHECK(game.status() == Status::draw);
    });
    suite.run("invalid inputs", [] {
        EXPECT_THROW(std::invalid_argument, Game(0, 7, 4));
        Game game;
        EXPECT_THROW(std::out_of_range, game.drop(-1));
        EXPECT_THROW(std::out_of_range, game.cell(6, 0));
        CHECK(game.next_player() == Cell::red);
    });
    suite.run("yellow can win", [] {
        Game game;
        for (int column : {0, 1, 0, 1, 2, 1, 2, 1}) { game.drop(column); }
        CHECK(game.status() == Status::yellow_won);
    });
    return suite.finish();
}
