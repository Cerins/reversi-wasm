#ifndef REVERSI_H
#define REVERSI_H

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
    const int size = 8;
    int board[64];
    Event event;
    bool turn;
};


Reversi* createReversi(void);

void destroyReversi(Reversi* reversi);

int countWhitePoints(Reversi* reversi);

int countBlackPoints(Reversi* reversi);

// This functions check if there is a legal move
// otherwise will set the event to PASS
// if there was no PASS event before
// if there was PASS event before
// it will set the event to END
bool passCheck(Reversi* reversi);

int* getBoard(Reversi* reversi);

void printBoard(Reversi* reversi);

int getTurn(Reversi* reversi);

Event getEvent(Reversi* reversi);

void makeMove(Reversi* reversi, int x, int y);

void playerMove(Reversi* reversi, int x, int y);

void computerMove(Reversi* reversi);

#endif //REVERSI_H