#define CATCH_CONFIG_MAIN  
#include "catch2.hpp"
#include <array>
#include "../reversi.h"

Reversi* createFrom(const std::array<std::array<char,8>, 8> &board, Color turn) {
    Reversi* reversi = createReversi();
    // W = WHITE
    // B = BLACK
    // anything else = UNDEFINED
    reversi->turn = turn;
    int size = reversi->size;
    for(int i=0;i<size*size;i++) {
        if(board[i/size][i%size] == 'W') {
            reversi->board[i] = WHITE;
        } else if(board[i/size][i%size] == 'B') {
            reversi->board[i] = BLACK;
        } else {
            reversi->board[i] = UNDEFINED;
        }
    }
    return reversi;
}

void sameBoard(const std::array<std::array<char, 8>, 8> &expected, Reversi* actual) {
    int size = actual->size;
    for(int i=0;i<size*size;i++) {
        if(expected[i/size][i%size] == 'W') {
            REQUIRE(actual->board[i] == WHITE);
        } else if(expected[i/size][i%size] == 'B') {
            REQUIRE(actual->board[i] == BLACK);
        } else {
            REQUIRE(actual->board[i] == UNDEFINED);
        }
    }
}

TEST_CASE("Reversi board creation", "[board]") {
    Reversi* reversi = createReversi();
    int* board = getBoard(reversi);
    for(int i=0;i<reversi->size*reversi->size;i++) {
        if(i == 27 || i == 36){
            REQUIRE(board[i] == WHITE);
        } else if(i == 28 || i == 35){
            REQUIRE(board[i] == BLACK);
        } else {
            REQUIRE(board[i] == UNDEFINED);
        }
    }
    REQUIRE(reversi->size == 8);
    REQUIRE(reversi->turn == BLACK);
    REQUIRE(reversi->event == NONE);
    destroyReversi(reversi);
}

TEST_CASE("getTurn", "[turn]") {
    Reversi* reversi = createReversi();
    REQUIRE(getTurn(reversi) == BLACK);
    reversi->turn = WHITE;
    REQUIRE(getTurn(reversi) == WHITE);
    reversi->turn = WHITE;
    REQUIRE(getTurn(reversi) == WHITE);
    destroyReversi(reversi);
}

TEST_CASE("getEvent", "[event]") {
    Reversi* reversi = createReversi();
    REQUIRE(getEvent(reversi) == NONE);
    reversi->event = PASS;
    REQUIRE(getEvent(reversi) == PASS);
    reversi->event = END;
    REQUIRE(getEvent(reversi) == END);
    destroyReversi(reversi);
}

TEST_CASE("countWhitePoints", "[whitePoints]") {
    Reversi* reversi = createReversi();
    REQUIRE(countWhitePoints(reversi) == 2);
    reversi->board[0] = WHITE;
    REQUIRE(countWhitePoints(reversi) == 3);
    reversi->board[0] = BLACK;
    REQUIRE(countWhitePoints(reversi) == 2);
    destroyReversi(reversi);
}

TEST_CASE("countBlackPoints", "[blackPoints]") {
    Reversi* reversi = createReversi();
    REQUIRE(countBlackPoints(reversi) == 2);
    reversi->board[0] = BLACK;
    REQUIRE(countBlackPoints(reversi) == 3);
    reversi->board[0] = WHITE;
    REQUIRE(countBlackPoints(reversi) == 2);
    destroyReversi(reversi);
}

TEST_CASE("makeMove no piecies on the board", "[makeMove]") {
    std::array<std::array<char, 8>, 8> board = {{
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'}
    }};
    Reversi* reversi = createFrom(board, BLACK);
    makeMove(reversi, 1, 1);
    REQUIRE(reversi->event == ILLEGAL_MOVE);
    sameBoard(board, reversi);
    destroyReversi(reversi);
}

TEST_CASE("makeMove legal diagonal move", "[makeMove]") {
    std::array<std::array<char, 8>, 8> board = {{
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'B', '.', '.', '.', '.', '.'},
        {'.', '.', '.', 'W', '.', '.', '.', '.'},
        {'.', '.', '.', '.', 'W', '.', '.', '.'},
        {'.', '.', '.', '.', '.', 'W', '.', '.'},
        {'.', '.', '.', '.', '.', '.', 'W', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'}
    }};
    std::array<std::array<char, 8>, 8> result = {{
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'B', '.', '.', '.', '.', '.'},
        {'.', '.', '.', 'B', '.', '.', '.', '.'},
        {'.', '.', '.', '.', 'B', '.', '.', '.'},
        {'.', '.', '.', '.', '.', 'B', '.', '.'},
        {'.', '.', '.', '.', '.', '.', 'B', '.'},
        {'.', '.', '.', '.', '.', '.', '.', 'B'}
    }};
    Reversi* reversi = createFrom(board, BLACK);
    makeMove(reversi, 7, 7);
    REQUIRE(reversi->event == NONE);
    sameBoard(result, reversi);
    destroyReversi(reversi);
}

TEST_CASE("makeMove legal horizontal move", "[makeMove]") {
    std::array<std::array<char, 8>, 8> board = {{
        {'.', 'B', 'B', 'B', 'W', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'}
    }};
    std::array<std::array<char, 8>, 8> result = {{
        {'W', 'W', 'W', 'W', 'W', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'}
    }};
    Reversi* reversi = createFrom(board, WHITE);
    makeMove(reversi, 0 , 0);
    REQUIRE(reversi->event == NONE);
    sameBoard(result, reversi);
    destroyReversi(reversi);
}

TEST_CASE("Multiple makeMove calls", "[makeMove]") {
    std::array<std::array<char, 8>, 8> initial = {{
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'W', '.', '.', '.', '.', '.'},
        {'.', '.', 'B', '.', '.', '.', '.', '.'},
        {'.', '.', 'W', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'}
    }};
    std::array<std::array<char, 8>, 8> afterBlack = {{
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'W', '.', '.', '.', '.', '.'},
        {'.', '.', 'B', '.', '.', '.', '.', '.'},
        {'.', '.', 'B', '.', '.', '.', '.', '.'},
        {'.', '.', 'B', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'}
    }};
    std::array<std::array<char, 8>, 8> afterWhite = {{
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', 'W', '.', '.', '.', '.', '.'},
        {'.', '.', 'W', '.', '.', '.', '.', '.'},
        {'.', '.', 'W', '.', '.', '.', '.', '.'},
        {'.', '.', 'W', '.', '.', '.', '.', '.'},
        {'.', '.', 'W', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'}
    }};
    Reversi* reversi = createFrom(initial, BLACK);
    makeMove(reversi, 2 , 4);
    REQUIRE(reversi->event == NONE);
    sameBoard(afterBlack, reversi);
    makeMove(reversi, 2 , 5);
    REQUIRE(reversi->event == NONE);
    sameBoard(afterWhite, reversi);
    destroyReversi(reversi);
}

TEST_CASE("PassCheck on empty board", "[passCheck]"){
    std::array<std::array<char, 8>, 8> board = {{
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'}
    }};
    Reversi* reversi = createFrom(board, BLACK);
    REQUIRE(passCheck(reversi) == true);
    REQUIRE(reversi->event == PASS);
    REQUIRE(passCheck(reversi) == true);
    REQUIRE(reversi->event == END);
    REQUIRE(passCheck(reversi) == true);
    REQUIRE(reversi->event == END);
    sameBoard(board, reversi);
    destroyReversi(reversi);
}

TEST_CASE("PassCheck on filled board but no legal moves", "[passCheck]"){
    std::array<std::array<char, 8>, 8> board = {{
        {'W', '.', '.', '.', '.', '.', '.', '.'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
        {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'}
    }};
    Reversi* reversi = createFrom(board, WHITE);
    REQUIRE(passCheck(reversi) == false);
    REQUIRE(reversi->event == PASS);
    sameBoard(board, reversi);
    REQUIRE(passCheck(reversi) == false);
    REQUIRE(reversi->event == END);
    sameBoard(board, reversi);
    destroyReversi(reversi);
}