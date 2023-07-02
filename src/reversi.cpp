#include "reversi.h"
#include <iostream>
#include <cstring>

// TODO there a lot of debug prints in this file
// clean them up

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


uint8_t* getBoard(Reversi* reversi){
    return reversi->board;
}

uint8_t getTurn(Reversi* reversi){
    return reversi->turn;
}

uint8_t getEvent(Reversi* reversi){
    return reversi->event;
}

uint8_t countWhitePoints(Reversi* reversi){
    int whitePoints = 0;
    for(int i=0;i<reversi->size*reversi->size;i++) {
        if(reversi->board[i] == WHITE) {
            whitePoints++;
        }
    }
    return whitePoints;
}

uint8_t countBlackPoints(Reversi* reversi){
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
            std::cout << (int)reversi->board[index];
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
        for(int dy=-1;dy<=1;dy++){
            int catched = 0;
            int direction = 1;
            x = oX;
            y = oY;
            // Now is i an extremely interesting loop
            // It works like so:
            // i go dx and dy until i i have found an item that is the same color as the current player
            // if i reached the end of the board or undefined cell i simply flip around and go the other way
            // on each step i do a xor 1 operation on the current item
            // this will make the opponent piece the opposite color
            // if i dont find the matching color item
            // then xor and going back ensures that i dont flip any pieces

            x += dx * direction;
            y += dy * direction;
            //std::cout << "----------------------------" << std::endl;
            while(true) {
                // If i am out of bounds i simply flip around
                /*std::cout << "moving: " << x << " " << y << std::endl;
                std::cout << "dx: " << dx << " dy: " << dy << std::endl;
                std::cout << "oX: " << oX << " oY: " << oY << std::endl;
                std::cout << "direction: " << direction << std::endl;
                std::cout << "cached: " << catched << std::endl;*/
                if(x < 0 || x >= reversi->size || y < 0 || y >= reversi->size) {
                    direction = -direction;
                    x += dx * direction;
                    y += dy * direction;
                }
                // If i am on an undefined cell i simply flip around
                index = x + y*reversi->size;
                if(reversi->board[index] == UNDEFINED) {
                    direction = -direction;
                    x += dx * direction;
                    y += dy * direction;
                }

                /*std::cout << "after checks: " << x << " " << y << std::endl;
                std::cout << "dx: " << dx << " dy: " << dy << std::endl;
                std::cout << "oX: " << oX << " oY: " << oY << std::endl;
                std::cout << "direction: " << direction << std::endl;
                std::cout << "cached: " << catched << std::endl;*/

                // Need to recalculate the index
                index = x + y*reversi->size;
                // If i have reached the same color then i exit the loop
                // the legal move is only if i catched at least one piece

                // ah there was a bug here
                // i can not check for the same color
                // since i flipped everything
                // so the actual check is rather simple - i arrived to the same point
                // or i  hit the same color with positive direction
                bool samePoint = x == oX && y == oY;
                bool sameColor = reversi->board[index] == reversi->turn;
                bool foundOtherPiece = sameColor && direction > 0;
                if(samePoint || foundOtherPiece) {
                    /*if(catched > 0){
                        std::cout << "catched final: " << catched << std::endl;
                        std::cout << "dx: " << dx << " dy: " << dy << std::endl;
                        std::cout << "oX: " << oX << " oY: " << oY << std::endl;
                        std::cout << "x: " << x << " y: " << y << std::endl;
                    }*/
                    legalMove = legalMove || catched > 0;
                    break;
                }
                // Otherwise i flip the piece
                reversi->board[index] = reversi->board[index] ^ 1;
                // Catch or uncatch a piece
                // Depending on the direction
                catched+=direction;
                x += dx * direction;
                y += dy * direction;
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
            //std::cout << (int)copy->event << std::endl;
            if(getEvent(copy) == NONE) {
                //std::cout << "Found a legal move" << std::endl;
                //std::cout << "x: " << j << " y: " << i << std::endl;
                pass = false;
                break;
            }
            // Reset the copy
            deepCopy(reversi, copy);
        }
    }
    // Delete the copy
    destroyReversi(copy);
    if(pass){
        if(reversi->event == PASS || reversi->event == END) {
            reversi->event = END;
        } else{
            reversi->event = PASS;
        }
        reversi->turn = !reversi->turn;
    }
    return pass;

}

int evalPos(Reversi* reversi, Color c){
    int boardSize = reversi->size;
    int score = 0;
    for(int i=0;i<boardSize;i++){
        for(int j=0;j<boardSize;j++){
            int index = j + i*boardSize;
            if(reversi->board[index] == c) {
                score++;
            } else if(reversi->board[index] != UNDEFINED) {
                score--;
            }
        }
    }
    return score;
}

int computerMoveUtil(Reversi* reversi, Color c, int depth, int startDepth, bool maximize) {
    bool gameEnded = reversi->event == END;
    if(depth == 0 || gameEnded) {
        return evalPos(reversi, c);
    }
    Reversi* best = new Reversi;
    Reversi* copy = new Reversi;
    int res = 0;
    //std::cout << "-----------------" << std::endl;
    //std::cout << "Depth " <<  depth << std::endl;
    if(maximize) {
        int maxEval = INT32_MIN;
        //std::cout << "Maximizing" << std::endl;
        for(int index=0;index<reversi->size*reversi->size;index++){
            //std::cout << "index: " << index << std::endl;
            if(reversi->board[index] == UNDEFINED) {
                deepCopy(reversi, copy);
                int x = index % reversi->size;
                int y = index / reversi->size;
                //std::cout << "Checking x: " << x << " y: " << y << std::endl;
                makeMove(copy, x, y);
                if(copy->event == ILLEGAL_MOVE) {
                    //std::cout << "Illegal move" << std::endl;
                    continue;
                }
                int eval = computerMoveUtil(copy, c, depth-1, depth, false);
                if(eval > maxEval) {
                    //std::cout << "Found a better move" << std::endl;
                    deepCopy(copy, best);
                }
                maxEval = std::max(maxEval, eval);
            }
        }
        // Did not find best move - all were illegal
        if(maxEval == INT32_MIN) {
            int eval = computerMoveUtil(reversi, c, depth-1, depth, false);
            if(eval > maxEval) {
                //std::cout << "Found no legal move" << std::endl;
                deepCopy(reversi, best);
            }
            maxEval = std::max(maxEval, eval);
        }
        res = maxEval;
    } else {
        int minEval = INT32_MAX;
        for(int index=0;index<reversi->size*reversi->size;index++){
            if(reversi->board[index] == UNDEFINED) {
                deepCopy(reversi, copy);
                int x = index % reversi->size;
                int y = index / reversi->size;
                makeMove(copy, x, y);
                if(copy->event == ILLEGAL_MOVE) {
                    continue;
                }
                int eval = computerMoveUtil(copy, c, depth-1, depth, true);
                if(eval < minEval) {
                    deepCopy(copy, best);
                }
                minEval = std::min(minEval, eval);
            }
        }
        // Did not find best move - all were illegal
        if(minEval == INT32_MAX) {
            int eval = computerMoveUtil(reversi, c, depth-1, depth, true);
            if(eval < minEval) {
                deepCopy(reversi, best);
            }
            minEval = std::min(minEval, eval);
        }
        res = minEval;
    }
    if(depth == startDepth) {
        deepCopy(best, reversi);
    }
    destroyReversi(copy);
    destroyReversi(best);
    return res;
}

void computerMove(Reversi* reversi, int depth){
    computerMoveUtil(reversi, (Color)reversi->turn, depth, depth, true);
}
