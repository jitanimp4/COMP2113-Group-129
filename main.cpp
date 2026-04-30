#include "cryptex_boss.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <string>

int runCrosswordApp();
int runWordPyramidApp(int argc, char* argv[]);
int runDirectorsArchiveApp();
int runWordleApp();

namespace {
int readMenuChoice() {
    int choice = -1;
    std::cin >> choice;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

std::string selectCryptexMode() {
    std::cout << "\nSelect Cryptex difficulty:\n";
    std::cout << "1. Easy\n";
    std::cout << "2. Medium\n";
    std::cout << "3. Hard\n";
    std::cout << "> ";
    int choice = readMenuChoice();
    if (choice == 1) return "easy";
    if (choice == 3) return "hard";
    return "medium";
}
}  // namespace

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    bool running = true;
    while (running) {
        std::cout << "\n=====================================\n";
        std::cout << "   ODYSSEY OS: THE HERITAGE TERMINAL\n";
        std::cout << "=====================================\n";
        std::cout << "1. Crossword\n";
        std::cout << "2. The Director's Archive\n";
        std::cout << "3. Word Pyramid\n";
        std::cout << "4. Curator's Cryptex\n";
        std::cout << "5. Forger's Table (work in progress)\n";
        std::cout << "6. Time-Slip Syndicate (Wordle)\n";
        std::cout << "0. Exit\n";
        std::cout << "Select app > ";

        const int choice = readMenuChoice();
        switch (choice) {
            case 1:
                runCrosswordApp();
                break;
            case 2:
                runDirectorsArchiveApp();
                break;
            case 3:
                runWordPyramidApp(0, nullptr);
                break;
            case 4:
                runCryptexBoss(selectCryptexMode());
                break;
            case 5:
                std::cout << "\nForger's Table is still in progress and not playable yet.\n";
                break;
            case 6:
                runWordleApp();
                break;
            case 0:
                std::cout << "\nLogging out of Odyssey OS.\n";
                running = false;
                break;
            default:
                std::cout << "\nInvalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
