#ifndef REVERSI_H
#define REVERSI_H

enum Color {
    WHITE = 0,
    BLACK = 1,
    UNDEFINED = 2
};

enum Event {
    NONE = 0,
    PASS = 1,
    END = 2
};

struct Reversi {
    int size;
    Color* board;
    Event event;
    Color turn;
};


Reversi* createReversi(void);

void destroyReversi(Reversi* reversi);

int countWhitePoints(Reversi* reversi);

int countBlackPoints(Reversi* reversi);

Color* getBoard(Reversi* reversi);

Color getTurn(Reversi* reversi);

Event getEvent(Reversi* reversi);

void playerMove(Reversi* reversi, int x, int y);

void computerMove(Reversi* reversi);

#endif //REVERSI_H