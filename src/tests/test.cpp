#define CATCH_CONFIG_MAIN  
#include "catch2.hpp"
#include "../reversi.h"

TEST_CASE("Reversi board creation", "[board]") {
    Reversi* reversi = createReversi();
    Color* board = getBoard(reversi);
    for(int i=0;i<reversi->size*reversi->size;i++) {
        REQUIRE(board[i] == UNDEFINED);
    }
    REQUIRE(reversi->size == 8);
    REQUIRE(reversi->turn == BLACK);
    REQUIRE(reversi->event == NONE);
    destroyReversi(reversi);
}