#include "game.h"

#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    CrosswordGame game("crossword_save.txt");
    game.run();
    return 0;
}
