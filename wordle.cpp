// TimeSlipSyndicate.cpp
// The Time-Slip Syndicate - An Architectural Wordle Game
// COMP2113 Project
//
// Team Members: Krislyn Mariah Mendonca, Menaka Menon, Anshika Mittal, Arpita Sharma, Anushka Jitani and Miracle.
// Date: 02/05/2026

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cctype>
#include <iomanip>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

using namespace std;
using namespace chrono;

enum Difficulty { EASY, MEDIUM, HARD };
enum CellState { EMPTY, CORRECT, MISPLACED, WRONG };

struct Cell {
    char letter;
    CellState state;
};

// Word banks
const vector<string> EASY_WORDS = {
    "RUINS", "MAYA", "ARCH", "DOME", "WALL", 
    "TOWER", "TEMPLE", "STONE", "BRICK", "PILLAR"
};

const vector<string> MEDIUM_WORDS = {
    "TOMBS", "AZTEC", "STOAS", "PYLON", "OBELI",
    "MAYAN", "INCA", "FORUM", "COLUMN", "VAULT"
};

const vector<string> HARD_WORDS = {
    "ZIGG", "STELA", "LINTL", "OGIVE", "TRABE",
    "DORIC", "IONIC", "CORIN", "CARYA", "ROSET"
};

class ConsoleColor {
private:
#ifdef _WIN32
    HANDLE hConsole;
#endif
public:
    ConsoleColor() {
#ifdef _WIN32
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
    }
    
    void setColor(int color) {
#ifdef _WIN32
        SetConsoleTextAttribute(hConsole, color);
#else
        cout << "\033[" << color << "m";
#endif
    }
    
    void reset() {
#ifdef _WIN32
        SetConsoleTextAttribute(hConsole, 7);
#else
        cout << "\033[0m";
#endif
    }
};

class Timer {
private:
    time_t startTime;
    int limit;
    bool running;
    
public:
    Timer() : limit(0), running(false) {}
    
    void start(int seconds) {
        limit = seconds;
        startTime = time(nullptr);
        running = true;
    }
    
    int getRemaining() {
        if (!running) return limit;
        time_t now = time(nullptr);
        int elapsed = difftime(now, startTime);
        int remaining = max(0, limit - elapsed);
        if (remaining <= 0) running = false;
        return remaining;
    }
    
    bool isExpired() {
        return getRemaining() <= 0;
    }
    
    void stop() {
        running = false;
    }
};

class TimeSlipSyndicate {
private:
    string targetWord;
    vector<vector<Cell>> grid;
    int currentRow;
    bool gameWon;
    bool gameActive;
    Difficulty difficulty;
    string currentGuess;
    Timer guessTimer;
    int timeLimit;
    vector<string> wordBank;
    ConsoleColor console;
    bool exitRequested;
    
public:
    TimeSlipSyndicate(Difficulty diff) : 
        currentRow(0), 
        gameWon(false), 
        gameActive(true),
        difficulty(diff),
        exitRequested(false) {
        
        switch(difficulty) {
            case EASY:
                timeLimit = 30;
                wordBank = EASY_WORDS;
                break;
            case MEDIUM:
                timeLimit = 20;
                wordBank = MEDIUM_WORDS;
                break;
            case HARD:
                timeLimit = 10;
                wordBank = HARD_WORDS;
                break;
        }
        
        srand(time(nullptr));
        targetWord = wordBank[rand() % wordBank.size()];
        
        grid.resize(6, vector<Cell>(5));
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 5; ++j) {
                grid[i][j].letter = ' ';
                grid[i][j].state = EMPTY;
            }
        }
        
        guessTimer.start(timeLimit);
    }
    
    void clearScreen() {
        // Simple clear screen that works everywhere
        for (int i = 0; i < 50; i++) {
            cout << endl;
        }
    }
    
    void displayInstructions() {
        clearScreen();
        console.setColor(36);
        cout << "\n========================================\n";
        cout << "   TIME-SLIP SYNDICATE - INSTRUCTIONS\n";
        cout << "========================================\n\n";
        console.reset();
        
        console.setColor(37);
        cout << "OBJECTIVE: Guess the 5-letter historical/architectural word in 6 tries!\n\n";
        
        console.setColor(32);
        cout << "COLOR MEANING:\n";
        cout << "  GREEN  - Letter is CORRECT and in RIGHT position\n";
        console.setColor(33);
        cout << "  YELLOW - Letter is CORRECT but in WRONG position\n";
        console.setColor(90);
        cout << "  GRAY   - Letter is NOT in the word\n\n";
        console.reset();
        
        console.setColor(36);
        cout << "CONTROLS:\n";
        console.reset();
        cout << "  • Type letters A-Z to form your 5-letter guess\n";
        cout << "  • Press ENTER to submit your guess\n";
        cout << "  • Press BACKSPACE to delete letters\n";
        cout << "  • Press ESC to exit the game at any time\n\n";
        
        console.setColor(33);
        cout << "DIFFICULTY LEVELS:\n";
        console.reset();
        cout << "  • EASY   - 30 seconds per guess, hints available\n";
        cout << "  • MEDIUM - 20 seconds per guess, standard challenge\n";
        cout << "  • HARD   - 10 seconds per guess, no hints\n\n";
        
        console.setColor(36);
        cout << "EXAMPLE:\n";
        console.reset();
        cout << "  If target word is 'RUINS' and you guess 'HOUSE':\n";
        cout << "  Letters: H O U S E\n";
        cout << "  Result:  GRAY GRAY YELLOW YELLOW GRAY\n";
        cout << "  This means U and S are in the word, but in wrong positions!\n\n";
        
        console.setColor(33);
        cout << "TIP: Start with words containing common vowels (A, E, I, O, U)\n";
        console.reset();
        
        cout << "\n========================================\n";
        cout << "\nPress ENTER to continue...";
        cin.get();
    }
    
    void drawGameUI() {
        clearScreen();
        
        console.setColor(36);
        cout << "\n========== THE TIME-SLIP SYNDICATE ==========\n";
        console.reset();
        
        string diffStr;
        switch(difficulty) {
            case EASY: diffStr = "EASY"; break;
            case MEDIUM: diffStr = "MEDIUM"; break;
            case HARD: diffStr = "HARD"; break;
        }
        
        int remaining = guessTimer.getRemaining();
        if (remaining < 5) {
            console.setColor(31);
        } else {
            console.setColor(33);
        }
        cout << "\n[" << diffStr << " MODE]  [TIME: " << remaining << "s]  [ATTEMPT: " << currentRow + 1 << "/6]";
        console.setColor(31);
        cout << "  [ESC to EXIT]\n";
        console.reset();
        
        cout << "\n---------------------------------------------\n\n";
        
        // Draw grid
        for (int row = 0; row < 6; ++row) {
            cout << "  ";
            
            for (int col = 0; col < 5; ++col) {
                switch(grid[row][col].state) {
                    case CORRECT:
                        console.setColor(32);
                        break;
                    case MISPLACED:
                        console.setColor(33);
                        break;
                    case WRONG:
                        console.setColor(90);
                        break;
                    default:
                        console.setColor(37);
                }
                
                char displayChar = (grid[row][col].letter == ' ') ? '_' : grid[row][col].letter;
                cout << "[" << displayChar << "] ";
                console.reset();
            }
            cout << "\n";
        }
        
        cout << "\n---------------------------------------------\n";
        
        console.setColor(36);
        cout << "\nGUESS: ";
        console.reset();
        
        for (size_t i = 0; i < currentGuess.length(); ++i) {
            console.setColor(37);
            cout << currentGuess[i];
            if (i < currentGuess.length() - 1) cout << " ";
        }
        for (size_t i = currentGuess.length(); i < 5; ++i) {
            console.setColor(90);
            cout << "_ ";
        }
        
        console.reset();
        cout << "\n\n";
        cout << "Enter a 5-letter word (or press ESC to exit): ";
    }
    
    void displayMessage(const string& message, int color) {
        console.setColor(color);
        cout << "\n*** " << message << " ***\n";
        console.reset();
        this_thread::sleep_for(chrono::milliseconds(1500));
    }
    
    bool evaluateGuess() {
        if (currentGuess.length() != 5) {
            displayMessage("Need 5 letters!", 33);
            return false;
        }
        
        // Convert to uppercase
        for (char& c : currentGuess) {
            c = toupper(c);
        }
        
        // Check win
        if (currentGuess == targetWord) {
            for (int i = 0; i < 5; ++i) {
                grid[currentRow][i].letter = currentGuess[i];
                grid[currentRow][i].state = CORRECT;
            }
            gameWon = true;
            gameActive = false;
            guessTimer.stop();
            return true;
        }
        
        // Count letters in target
        map<char, int> targetCount;
        for (char c : targetWord) {
            targetCount[c]++;
        }
        
        // First pass: mark correct positions
        for (int i = 0; i < 5; ++i) {
            if (currentGuess[i] == targetWord[i]) {
                grid[currentRow][i].letter = currentGuess[i];
                grid[currentRow][i].state = CORRECT;
                targetCount[currentGuess[i]]--;
            }
        }
        
        // Second pass: mark misplaced letters
        for (int i = 0; i < 5; ++i) {
            if (grid[currentRow][i].state != CORRECT) {
                if (targetCount[currentGuess[i]] > 0) {
                    grid[currentRow][i].letter = currentGuess[i];
                    grid[currentRow][i].state = MISPLACED;
                    targetCount[currentGuess[i]]--;
                } else {
                    grid[currentRow][i].letter = currentGuess[i];
                    grid[currentRow][i].state = WRONG;
                }
            }
        }
        
        displayMessage("Historical fragment mismatch!", 33);
        
        currentRow++;
        currentGuess = "";
        guessTimer.start(timeLimit);
        
        if (currentRow >= 6) {
            gameActive = false;
            return false;
        }
        
        return false;
    }
    
    char getKey() {
#ifdef _WIN32
        return _getch();
#else
        struct termios oldt, newt;
        char ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
#endif
    }
    
    void showHint() {
        if (difficulty == EASY && currentRow == 3 && !gameWon && !exitRequested) {
            console.setColor(33);
            cout << "\n[HINT: The word contains: ";
            for (char c : targetWord) {
                cout << c << " ";
            }
            cout << "]\n";
            console.reset();
            this_thread::sleep_for(chrono::milliseconds(2000));
        }
    }
    
    void run() {
        displayInstructions();
        
        while (gameActive && !gameWon && currentRow < 6 && !exitRequested) {
            drawGameUI();
            showHint();
            
            // Check timer
            if (guessTimer.isExpired()) {
                displayMessage("TIME PARADOX! Guess expired!", 31);
                currentRow++;
                currentGuess = "";
                
                if (currentRow < 6) {
                    guessTimer.start(timeLimit);
                } else {
                    gameActive = false;
                    break;
                }
                continue;
            }
            
            // Get input
            if (keyAvailable()) {
                char key = getKey();
                
                if (key == 27) { // ESC
                    exitRequested = true;
                    displayMessage("Exiting game...", 31);
                    break;
                }
                else if (key == '\n' || key == '\r') {
                    evaluateGuess();
                } 
                else if (key == '\b' || key == 127) {
                    if (!currentGuess.empty()) {
                        currentGuess.pop_back();
                    }
                }
                else if (isalpha(key)) {
                    if (currentGuess.length() < 5) {
                        currentGuess += toupper(key);
                    }
                }
            }
            
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        
        // Game over screen
        drawGameUI();
        cout << "\n";
        
        if (gameWon && !exitRequested) {
            console.setColor(32);
            cout << "=============================================\n";
            cout << "  ★ TEMPORAL ARCHIVE COMPLETE! ★\n";
            cout << "  You successfully restored the timeline!\n";
            cout << "=============================================\n";
            console.reset();
        } 
        else if (!exitRequested && !gameWon) {
            console.setColor(31);
            cout << "=============================================\n";
            cout << "  ✗ TEMPORAL ARCHIVE FAILED ✗\n";
            cout << "  The word was: " << targetWord << "\n";
            cout << "=============================================\n";
            console.reset();
        } 
        else if (exitRequested) {
            console.setColor(33);
            cout << "=============================================\n";
            cout << "  Game exited.\n";
            cout << "=============================================\n";
            console.reset();
        }
        
        cout << "\nPress ENTER to continue...";
        cin.get();
    }
};

int runWordleApp() {
    ConsoleColor console;
    
    while (true) {
        // Clear screen using newlines
        for (int i = 0; i < 30; i++) cout << endl;
        
        console.setColor(36);
        cout << "=============================================\n";
        cout << "      THE TIME-SLIP SYNDICATE\n";
        cout << "      Architectural Wordle Game\n";
        cout << "=============================================\n";
        console.reset();
        
        console.setColor(33);
        cout << "\n          MAIN MENU\n";
        console.reset();
        cout << "\n    1. EASY     - 30s per guess, with hints";
        cout << "\n    2. MEDIUM   - 20s per guess, standard challenge";
        cout << "\n    3. HARD     - 10s per guess, expert mode";
        cout << "\n    4. INSTRUCTIONS";
        cout << "\n    5. EXIT     - Quit the game\n";
        
        console.setColor(36);
        cout << "\n    Enter your choice (1-5): ";
        console.reset();
        
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        
        cin.ignore(1000, '\n');
        
        if (choice == 5) {
            console.setColor(32);
            cout << "\nThank you for playing!\n";
            console.reset();
            return 0;
        }
        
        if (choice == 4) {
            // Show instructions
            for (int i = 0; i < 30; i++) cout << endl;
            console.setColor(36);
            cout << "\n========================================\n";
            cout << "   TIME-SLIP SYNDICATE - INSTRUCTIONS\n";
            cout << "========================================\n\n";
            console.reset();
            
            console.setColor(37);
            cout << "OBJECTIVE: Guess the 5-letter word in 6 tries!\n\n";
            
            console.setColor(32);
            cout << "COLOR MEANING:\n";
            cout << "  GREEN  - Correct letter, right position\n";
            console.setColor(33);
            cout << "  YELLOW - Correct letter, wrong position\n";
            console.setColor(90);
            cout << "  GRAY   - Letter not in word\n\n";
            console.reset();
            
            console.setColor(36);
            cout << "CONTROLS:\n";
            console.reset();
            cout << "  • Type letters to guess\n";
            cout << "  • Press ENTER to submit\n";
            cout << "  • Press BACKSPACE to delete\n";
            cout << "  • Press ESC to exit\n\n";
            
            console.setColor(33);
            cout << "DIFFICULTY LEVELS:\n";
            console.reset();
            cout << "  • EASY   - 30 seconds, with hints\n";
            cout << "  • MEDIUM - 20 seconds\n";
            cout << "  • HARD   - 10 seconds, no hints\n\n";
            
            console.setColor(36);
            cout << "TIP: Start with words containing vowels (A, E, I, O, U)\n";
            console.reset();
            
            cout << "\n========================================\n";
            cout << "\nPress ENTER to return to menu...";
            cin.get();
            continue;
        }
        
        Difficulty diff;
        switch(choice) {
            case 1: diff = EASY; break;
            case 2: diff = MEDIUM; break;
            case 3: diff = HARD; break;
            default: 
                continue;
        }
        
        TimeSlipSyndicate game(diff);
        game.run();
    }
    
    return 0;
}

int main() {
    return runWordleApp();
}
