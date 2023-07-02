#include <iostream>
#include "reversi.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "Size of Reversi: " << sizeof(Reversi) << std::endl;
    Reversi* reversi = createReversi();
    std::cout << "Size of Reversi: " << sizeof(Reversi) << std::endl;
    std::cout << "Size of Reversi: " << sizeof(*reversi) << std::endl;
    std::cout << "Size of Reversi board: " << sizeof(reversi->board) << std::endl;
    std::cout << "Size of Reversi board item: " << sizeof(reversi->board[0]) << std::endl;
    std::cout << "Size of Reversi board count: " << sizeof(reversi->board) / sizeof(reversi->board[0]) << std::endl;
    std::cout << "Size of Reversi turn" << sizeof(reversi->turn) << std::endl;
    std::cout << "Size of Reversi event" << sizeof(reversi->event) << std::endl;
    destroyReversi(reversi);
    return 0;
}
