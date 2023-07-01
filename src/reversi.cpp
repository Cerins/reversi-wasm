#include "reversi.h"
#include <iostream>
#include <cstring>

Reversi* createReversi(void){
    Reversi* reversi = new Reversi;
    // Reversi is played on a 8x8 board
    // The board is represented as a 1D array
    // Initialize the board to UNDEFINED
    for(int i=0;i<reversi->size*reversi->size;i++) {
        reversi->board[i] = UNDEFINED;
    }
    // This seems pretty random
    // but actually corresponds to the initial board state
    // x0
    // 0x
    // two players facing each other
    reversi->board[27] = WHITE;
    reversi->board[28] = BLACK;
    reversi->board[35] = BLACK;
    reversi->board[36] = WHITE;
    reversi->turn = BLACK;
    reversi->event = NONE;
    return reversi;
}

void destroyReversi(Reversi* reversi){
    delete reversi;
}


int* getBoard(Reversi* reversi){
    return reversi->board;
}

int getTurn(Reversi* reversi){
    return reversi->turn;
}

Event getEvent(Reversi* reversi){
    return reversi->event;
}

int countWhitePoints(Reversi* reversi){
    int whitePoints = 0;
    for(int i=0;i<reversi->size*reversi->size;i++) {
        if(reversi->board[i] == WHITE) {
            whitePoints++;
        }
    }
    return whitePoints;
}

int countBlackPoints(Reversi* reversi){
    int blackPoints = 0;
    for(int i=0;i<reversi->size*reversi->size;i++) {
        if(reversi->board[i] == BLACK) {
            blackPoints++;
        }
    }
    return blackPoints;
}

void printBoard(Reversi* reversi) {
    std::cout << std::endl;
    for(int i=0;i<reversi->size;i++) {
        std::cout << i << ":";
        for(int j=0;j<reversi->size;j++) {
            int index = j + i*reversi->size;
            std::cout << reversi->board[index];
        }
        std::cout << std::endl;
    }
}

void makeMove(Reversi* reversi, int x, int y){
    int oX = x;
    int oY = y;
    int index = x + y*reversi->size;
    // Can not make a move on a non-empty cell
    if(reversi->board[index] != UNDEFINED) {
        reversi->event = ILLEGAL_MOVE;
        return;
    }
    reversi->board[index] = reversi->turn;
    bool legalMove = false;
    // Now this function both flips all the items
    for(int dx=-1;dx<=1;dx++){
        for(int dj=-1;dj<=1;dj++){
            int catched = 0;
            int direction = 1;
            // Now is i an extremely interesting loop
            // It works like so:
            // i go dx and dy until i i have found an item that is the same color as the current player
            // if i reached the end of the board or undefined cell i simply flip around and go the other way
            // on each step i do a xor 1 operation on the current item
            // this will make the opponent piece the opposite color
            // if i dont find the matching color item
            // then xor and going back ensures that i dont flip any pieces

            x += dx * direction;
            y += dj * direction;
            while(true) {
                // If i am out of bounds i simply flip around
                if(x < 0 || x >= reversi->size || y < 0 || y >= reversi->size) {
                    direction = -direction;
                    x += dx * direction;
                    y += dj * direction;
                }
                // If i am on an undefined cell i simply flip around
                index = x + y*reversi->size;
                if(reversi->board[index] == UNDEFINED) {
                    direction = -direction;
                    x += dx * direction;
                    y += dj * direction;
                }
                // Need to recalculate the index
                index = x + y*reversi->size;
                // If i have reached the same color then i exit the loop
                // the legal move is only if i catched at least one piece
                if(reversi->board[index] == reversi->turn) {
                    legalMove = legalMove || catched > 0;
                    break;
                }
                // Otherwise i flip the piece
                reversi->board[index] = reversi->board[index] ^ 1;
                // Catch or uncatch a piece
                // Depending on the direction
                catched+=direction;
                x += dx * direction;
                y += dj * direction;
            }
        }
    }
    if(!legalMove) {
        reversi->event = ILLEGAL_MOVE;
        // undo the move
        index = oX + oY*reversi->size;
        reversi->board[index] = UNDEFINED;
        return;
    }
    reversi->event = NONE;
    // Change the turn
    // this works because the turn is represented as a 0 or 1
    // black or white - binary
    reversi->turn = !reversi->turn;
}

void playerMove(Reversi* reversi, int x, int y){
    makeMove(reversi, x, y);
}

void deepCopy(Reversi* reversi, Reversi* copy){
    memcpy(copy, reversi, sizeof(Reversi));
}
bool passCheck(Reversi* reversi){
    int boardSize = reversi->size;
    // Make a copy of the reversi struct
    Reversi* copy = new Reversi;
    deepCopy(reversi, copy);
    bool pass = true;
    for(int i=0;i<boardSize;i++){
        for(int j=0;j<boardSize;j++){
            makeMove(copy, j, i);
            if(getEvent(copy) == NONE) {
                pass = false;
                break;
            }
            // Reset the copy
            deepCopy(reversi, copy);
        }
    }
    // Delete the copy
    destroyReversi(copy);
    if(reversi->event == PASS || reversi->event == END) {
        reversi->event = END;
    } else{
        reversi->event = PASS;
    }
    reversi->turn = !reversi->turn;
    return pass;
}