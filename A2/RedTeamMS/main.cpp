#include "minesweeper.h" 

int main() {
    int width, height;
    std::vector< std::vector<cell> > board = make_board(&width, &height);
    play(board, width, height);
}