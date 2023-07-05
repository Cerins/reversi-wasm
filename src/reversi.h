#ifndef REVERSI_H
#define REVERSI_H
#include <cstdint>

enum Color {
    WHITE = 0,
    BLACK = 1,
    UNDEFINED = 2
};
inline Color operator^(Color a, int b)
{
    return static_cast<Color>(static_cast<int>(a) | b);
}

enum Event {
    NONE = 0,
    PASS = 1,
    ILLEGAL_MOVE = 2,
    END = 3
};

struct Reversi {
    const uint8_t size = 8;
    uint8_t board[64];
    uint8_t event;
    bool turn;
};

extern "C" {

    /**
     * Generate and return a new reversi game pointer
     * The game is initialized to the initial board state
     * @return a pointer to a new reversi game
    */
    Reversi* createReversi(void);

    /**
     * Destroy a reversi game, thus freeing the allocated memory
     * @param reversi a pointer to a reversi game
    */
    void destroyReversi(Reversi* reversi);

    /**
     * Count the total points of the white player
     * @param reversi a pointer to a reversi game
    */
    uint8_t countWhitePoints(Reversi* reversi);

    /**
     * Count the total points of the black player
     * @param reversi a pointer to a reversi game
    */
    uint8_t countBlackPoints(Reversi* reversi);

    /**
     * Deep copy one reversi game to another
    */
    void deepCopy(Reversi* reversi, Reversi* copy);

    /**
     * This functions check if there is a legal move
     * otherwise will set the event to PASS
     * if there was no PASS event before
     * if there was PASS event before
     * it will set the event to END
    */
    bool passCheck(Reversi* reversi);

    /**
     * Get the 64 item array which represents the board
    */
    uint8_t* getBoard(Reversi* reversi);

    /**
     * Print the board to the console
    */
    void printBoard(Reversi* reversi);

    /**
     * Get the current turn
    */
    uint8_t getTurn(Reversi* reversi);

    /**
     * Get the current event
    */
    uint8_t getEvent(Reversi* reversi);

    /**
     * Make a move on the board
     * If the move is illegal, the event will be set to ILLEGAL_MOVE
     * The board will not change
     * If the move is legal then the board changes based on the games rules
     * And the turn will be switched
    */
    void makeMove(Reversi* reversi, int x, int y);

    /**
     * Let the player make a move
    */
    void playerMove(Reversi* reversi, int x, int y);

    /**
     * Make the computer make a best move based on the minimax algorithm
     * Used alpha-beta pruning to speed up the algorithm
    */
    void computerMove(Reversi* reversi, int depth);

}

#endif //REVERSI_H