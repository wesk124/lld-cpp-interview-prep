#ifdef LLD_PRACTICE
#include "starter.hpp"
#else
#include "solution.hpp"
#endif
#include "test_support.hpp"

using namespace lld::connect_four;

int main() {
    test::Suite suite;
    suite.run("gravity and invalid move preserve turn", [] {
        ConnectKRule rule;
        Game game(rule, 2, 2);
        CHECK(game.drop(0));
        CHECK(game.cell(1, 0) == 'R');
        CHECK(game.drop(0));
        CHECK(game.cell(0, 0) == 'Y');
        CHECK(!game.drop(0));
        CHECK(!game.drop(-1));
        CHECK(game.next_player() == 'R');
    });
    suite.run("horizontal and vertical wins", [] {
        ConnectKRule rule;
        Game horizontal(rule), vertical(rule);
        for (int column : {0, 6, 1, 6, 2, 6, 3}) CHECK(horizontal.drop(column));
        CHECK(horizontal.status() == Status::red_won);
        CHECK(!horizontal.drop(4));
        for (int column : {0, 1, 0, 1, 0, 1, 0}) CHECK(vertical.drop(column));
        CHECK(vertical.status() == Status::red_won);
    });
    suite.run("diagonal and draw", [] {
        ConnectKRule rule;
        Game diagonal(rule), draw(rule, 2, 2);
        for (int column : {0, 1, 1, 2, 2, 3, 2, 3, 3, 5, 3}) CHECK(diagonal.drop(column));
        CHECK(diagonal.status() == Status::red_won);
        for (int column : {0, 1, 0, 1}) CHECK(draw.drop(column));
        CHECK(draw.status() == Status::draw);
    });
    suite.run("winning-rule Strategy changes the rules", [] {
        ConnectKRule connect_three(3);
        Game game(connect_three);
        for (int column : {0, 1, 0, 1, 0}) CHECK(game.drop(column));
        CHECK(game.status() == Status::red_won);
    });
    return suite.finish();
}
