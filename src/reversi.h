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

    Reversi* createReversi(void);

    void destroyReversi(Reversi* reversi);

    uint8_t countWhitePoints(Reversi* reversi);

    uint8_t countBlackPoints(Reversi* reversi);

    void deepCopy(Reversi* reversi, Reversi* copy);

    // This functions check if there is a legal move
    // otherwise will set the event to PASS
    // if there was no PASS event before
    // if there was PASS event before
    // it will set the event to END
    bool passCheck(Reversi* reversi);

    uint8_t* getBoard(Reversi* reversi);

    void printBoard(Reversi* reversi);

    uint8_t getTurn(Reversi* reversi);

    uint8_t getEvent(Reversi* reversi);

    void makeMove(Reversi* reversi, int x, int y);

    void playerMove(Reversi* reversi, int x, int y);

    void computerMove(Reversi* reversi, int depth);

}

#endif //REVERSI_H