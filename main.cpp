// ============================================================
// ODYSSEY OS - ROOT MENU (main.cpp)
// ============================================================
#include <iostream>
#include <string>

using namespace std;

// --- FORWARD DECLARATIONS ---
// This tells the compiler your mini-games exist in the other folders
void runForgersTable();
// Uncomment these below as you fix the main() functions in your other files!
// void runDirectorsArchive(); 
// void runCrossword();
// void runWordPyramid();
// void runWordle();
// void runCryptexBoss();

int main() {
    bool systemRunning = true;

    while (systemRunning) {
        cout << "\n\033[1;36m========================================\033[0m\n";
        cout << "\033[1;32m      ODYSSEY OS: TERMINAL v2.7       \033[0m\n";
        cout << "\033[1;36m========================================\033[0m\n";
        cout << "1. The Forger's Table (Interpol Unit)\n";
        cout << "2. The Director's Archive (Celluloid Citadel)\n";
        cout << "3. Crossword Puzzle\n";
        cout << "4. Word Pyramids\n";
        cout << "5. Architect's Wordle\n";
        cout << "6. The Curator's Cryptex (BOSS LEVEL)\n";
        cout << "7. Exit System\n";
        cout << "Select App > ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 1) {
            runForgersTable(); 
        } else if (choice == 2) {
            cout << "Loading The Director's Archive...\n";
            // runDirectorsArchive(); 
        } else if (choice == 3) {
            cout << "Loading Crossword...\n";
            // runCrossword();
        } else if (choice == 4) {
            cout << "Loading Word Pyramids...\n";
            // runWordPyramid();
        } else if (choice == 5) {
            cout << "Loading Architect's Wordle...\n";
            // runWordle();
        } else if (choice == 6) {
            cout << "Loading The Curator's Cryptex...\n";
            // runCryptexBoss();
        } else if (choice == 7) {
            cout << "Logging off...\n";
            systemRunning = false;
        } else {
            cout << "\033[1;31m[ERROR] Invalid command.\033[0m\n"; 
        }
    }
    return 0;
}
