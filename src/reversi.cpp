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
    // Places the piece on the board
    reversi->board[index] = reversi->turn;
    // Initially we assume that the move is illegal
    bool legalMove = false;
    // Now this function both flips all the items
    // dx, dy ensures that each direction is checked
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
            while(true) {
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

                // Need to recalculate the index
                index = x + y*reversi->size;
                // If i have reached the same color then i exit the loop
                // the legal move is only if i caught at least one piece

                // ah there was a bug here
                // i can not check for the same color
                // since i flipped everything
                // so the actual check is rather simple - i arrived to the same point
                // or i  hit the same color with positive direction
                bool samePoint = x == oX && y == oY;
                bool sameColor = reversi->board[index] == reversi->turn;
                bool foundOtherPiece = sameColor && direction > 0;
                if(samePoint || foundOtherPiece) {
                    // Found a way that the move is legal because I either hit a black piece
                    // and catched at least one piece
                    legalMove = legalMove || catched > 0;
                    break;
                }
                // Otherwise i flip the piece, since it gets "captured" or "uncaptured" if i go back
                reversi->board[index] = reversi->board[index] ^ 1;
                // Catch or uncatch a piece
                // Depending on the direction
                catched+=direction;
                x += dx * direction;
                y += dy * direction;
            }
        }
    }
    // Set the proper event and undo the move if it was illegal
    if(!legalMove) {
        reversi->event = ILLEGAL_MOVE;
        // undo the move
        index = oX + oY*reversi->size;
        reversi->board[index] = UNDEFINED;
        return;
    }
    // Otherwise clear the event
    reversi->event = NONE;
    // Change the turn
    // this works because the turn is represented as a 0 or 1
    // black or white - binary
    reversi->turn = !reversi->turn;
}

void playerMove(Reversi* reversi, int x, int y){
    // Simply call the makeMove function
    makeMove(reversi, x, y);
}

void deepCopy(Reversi* reversi, Reversi* copy){
    // Reversi struct does not have any pointers, so a simple memcpy is enough
    memcpy(copy, reversi, sizeof(Reversi));
}

/**
 * This function checks if a player has to pass
 * By playing all the possible moves and checking if there is a legal move
*/
bool passCheck(Reversi* reversi){
    int boardSize = reversi->size;
    // Make a copy of the reversi struct
    Reversi* copy = new Reversi;
    deepCopy(reversi, copy);
    // Assume that the player has to pass
    bool pass = true;
    for(int i=0;i<boardSize;i++){
        for(int j=0;j<boardSize;j++){
            // Make a move on the copy
            makeMove(copy, j, i);
            if(getEvent(copy) == NONE) {
                // Oh I found a legal move, so dont pass
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
        // If there was already a pass or an end event then the game is over
        if(reversi->event == PASS || reversi->event == END) {
            reversi->event = END;
        } else{
            // Otherwise the player has to pass
            reversi->event = PASS;
        }
        // And since the player has to pass, the turn is switched
        reversi->turn = !reversi->turn;
    }
    return pass;

}

/**
 * This function evaluates the current position for the given player
 * It counts the number of pieces minus the number of opponent pieces
 * There is nothing else that defines a good position in reversi
*/
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

/** 
 * The util function for calculating the best move for the computer
*/
int computerMoveUtil(Reversi* reversi, Color c, int depth, int startDepth, bool maximize, int alpha, int beta) {
    // Check if the game ended
    bool gameEnded = reversi->event == END;
    // If reached the maximum depth or the game ended
    // Then wer can return the position evaluation
    if(depth == 0 || gameEnded) {
        return evalPos(reversi, c);
    }
    // Currently the best move
    Reversi* best = new Reversi;
    // Temporary struct to try out moves
    Reversi* copy = new Reversi;

    // The current depth position evaluation
    int eval = 0;
    // If we are maximizing then we want to find the maximum evaluation
    if(maximize) {
        // Assume that the maximum evaluation is the worst possible
        int maxEval = INT32_MIN;
        // And then simply make all the possible moves and check the evaluation
        for(int index=0; index<reversi->size*reversi->size; index++){
            if(reversi->board[index] == UNDEFINED) {
                deepCopy(reversi, copy);
                int x = index % reversi->size;
                int y = index / reversi->size;
                makeMove(copy, x, y);
                if(copy->event == ILLEGAL_MOVE) {
                    continue;
                }
                int eval = computerMoveUtil(copy, c, depth-1, startDepth, false, alpha, beta);
                if(eval > maxEval) {
                    deepCopy(copy, best);
                }
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break; // Beta cut-off
                }
            }
        }
        // If there was no legal move then we simply do not change the board
        if(maxEval == INT32_MIN) {
            int eval = computerMoveUtil(reversi, c, depth-1, startDepth, false, alpha, beta);
            if(eval > maxEval) {
                deepCopy(reversi, best);
            }
            maxEval = std::max(maxEval, eval);
        }
        eval = maxEval;
    } else {
        // If we are minimizing then we want to find the minimum evaluation
        int minEval = INT32_MAX;
        // Also make all the possible moves and check the evaluation
        for(int index=0;index<reversi->size*reversi->size;index++){
            if(reversi->board[index] == UNDEFINED) {
                deepCopy(reversi, copy);
                int x = index % reversi->size;
                int y = index / reversi->size;
                makeMove(copy, x, y);
                if(copy->event == ILLEGAL_MOVE) {
                    continue;
                }
                int eval = computerMoveUtil(copy, c, depth-1, startDepth, true, alpha, beta);
                if(eval < minEval) {
                    deepCopy(copy, best);
                }
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break; // Alpha cut-off
                }
            }
        }
        if(minEval == INT32_MAX) {
            int eval = computerMoveUtil(reversi, c, depth-1, startDepth, true, alpha, beta);
            if(eval < minEval) {
                deepCopy(reversi, best);
            }
            minEval = std::min(minEval, eval);
        }
        eval = minEval;
    }
    if(depth == startDepth) {
        deepCopy(best, reversi);
    }
    destroyReversi(copy);
    destroyReversi(best);
    return eval;
}

void computerMove(Reversi* reversi, int depth){
    // At the start of calculations alpha is the worst possible score
    int alpha = INT32_MIN;
    // At the start of calculations beta is the best possible score
    int beta = INT32_MAX;
    computerMoveUtil(reversi, (Color)reversi->turn, depth, depth, true, alpha, beta);
}
