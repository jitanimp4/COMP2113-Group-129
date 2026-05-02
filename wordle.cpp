// TimeSlipSyndicate.cpp
// The Time-Slip Syndicate - An Architectural Wordle Game
// COMP2113 Project
//
// Team Members: Krislyn Mariah Mendonca, Menaka Menon, Anshika Mittal, Arpita Sharma, Anushka Jitani and Miracle.
// Date: 01/05/2026

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
#include <fstream>

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

// ================================
// Constants and Enumerations
// ================================

enum Difficulty { EASY, MEDIUM, HARD };
enum CellState { EMPTY, CORRECT, MISPLACED, WRONG };

struct Cell {
    char letter;
    CellState state;
};

// Word banks for different difficulty levels
const vector<string> EASY_WORDS = {
    "RUINS", "MAYA", "ARCH", "DOME", "WALL", 
    "TOWER", "TEMPLE", "STONE", "BRICK", "PILLAR"
};

const vector<string> MEDIUM_WORDS = {
    "TOMBS", "AZTEC", "STOAS", "PYLON", "OBELI",
    "MAYAN", "INCA", "FORUM", "COLUMN", "VAULT"
};

const vector<string> HARD_WORDS = {
    "ZIGGURAT", "STELA", "LINTEL", "OGIVE", "TRABEATION",
    "DORIC", "IONIC", "CORINTHIAN", "CARYATID", "ROSETTA"
};

// ================================
// Console Color Class
// ================================

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

// ================================
// Timer Class
// ================================

class Timer {
private:
    steady_clock::time_point startTime;
    int limit;
    bool running;
    
public:
    Timer() : limit(0), running(false) {}
    
    void start(int seconds) {
        limit = seconds;
        startTime = steady_clock::now();
        running = true;
    }
    
    int getRemaining() {
        if (!running) return limit;
        auto now = steady_clock::now();
        int elapsed = duration_cast<seconds>(now - startTime).count();
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

// ================================
// Game Class
// ================================

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
    
    const vector<string> successMessages = {
        "Temporal fragment recovered!",
        "Historical resonance detected!",
        "Timeline stabilized!",
        "Artifact successfully decoded!"
    };
    
    const vector<string> failureMessages = {
        "Temporal anomaly detected!",
        "Historical fragment corrupted!",
        "Timeline divergence imminent!",
        "Artifact mismatch confirmed!"
    };
    
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
        
        // Make sure target word is exactly 5 letters for easy/medium
        if (difficulty == HARD) {
            // Filter HARD_WORDS to only 5-letter words
            for (const auto& word : wordBank) {
                if (word.length() == 5) {
                    targetWord = word;
                    break;
                }
            }
        }
        
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
        cout << "\033[2J\033[1;1H";
    }
    
    void drawTopBorder() {
        console.setColor(36);
        cout << "╔════════════════════════════════════════════════════════════════════════════════╗\n";
        console.reset();
    }
    
    void drawBottomBorder() {
        console.setColor(36);
        cout << "╚════════════════════════════════════════════════════════════════════════════════╝\n";
        console.reset();
    }
    
    void drawHeader() {
        console.setColor(36);
        cout << "║  [TEMPORAL ARCHIVE v2.47]";
        
        console.setColor(32);
        cout << "  [SCANNING]";
        
        console.setColor(33);
        cout << " [ONLINE]";
        
        console.setColor(32);
        cout << " [SECURE]";
        
        console.setColor(36);
        cout << "                                      ║\n";
        console.reset();
        
        console.setColor(90);
        cout << "║  ⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡  ║\n";
        console.reset();
    }
    
    void drawSidePanels() {
        console.setColor(33);
        cout << "║  ┌─────────────┐                                                ║\n";
        cout << "║  │ [CLASSICAL] │                                                ║\n";
        cout << "║  │    🏛       │                                                ║\n";
        cout << "║  ├─────────────┤                                                ║\n";
        cout << "║  │ [MEDIEVAL]  │                                                ║\n";
        cout << "║  │    🏰       │                                                ║\n";
        cout << "║  ├─────────────┤                                                ║\n";
        cout << "║  │  [MODERN]   │                                                ║\n";
        cout << "║  │    🏭       │                                                ║\n";
        cout << "║  └─────────────┘                                                ║\n";
        console.reset();
    }
    
    void drawTimer() {
        int remaining = guessTimer.getRemaining();
        
        cout << "║  ";
        console.setColor(remaining < 5 ? 31 : 33);
        cout << "[TIME: " << setw(2) << remaining << "s ";
        
        int progress = (remaining * 10) / timeLimit;
        for (int i = 0; i < progress; ++i) cout << "█";
        for (int i = progress; i < 10; ++i) cout << "░";
        cout << "]";
        
        console.reset();
        
        console.setColor(36);
        cout << "  [ATTEMPT: " << currentRow + 1 << "/6]";
        console.reset();
        
        console.setColor(31);
        cout << "  [ESC to EXIT]";
        console.reset();
        
        int spaces = 69 - (15 + (remaining < 5 ? 0 : 0));
        for (int i = 0; i < spaces; ++i) cout << " ";
        cout << "║\n";
    }
    
    void drawWordleGrid() {
        for (int row = 0; row < 6; ++row) {
            cout << "║  ";
            
            if (row == currentRow && gameActive && !gameWon && !exitRequested) {
                console.setColor(36);
                cout << "▶";
                console.reset();
                cout << " ";
            } else {
                cout << "  ";
            }
            
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
                
                cout << " " << (grid[row][col].letter == ' ' ? '_' : grid[row][col].letter) << "  ";
            }
            cout << "║\n";
            console.reset();
        }
    }
    
    void drawInputArea() {
        console.setColor(36);
        cout << "║  ┌────────────────────────────────────────────────────────────────────┐  ║\n";
        cout << "║  │ [GUESS: ";
        
        for (size_t i = 0; i < currentGuess.length(); ++i) {
            console.setColor(37);
            cout << currentGuess[i];
            if (i < currentGuess.length() - 1) cout << " ";
        }
        for (size_t i = currentGuess.length(); i < 5; ++i) {
            console.setColor(90);
            cout << "_ ";
        }
        
        console.setColor(36);
        cout << "]";
        
        if (difficulty == EASY && currentRow == 2 && !gameWon) {
            console.setColor(33);
            cout << "  [HINT: Common letters in historical terms]";
        } else {
            console.setColor(90);
            cout << "  [Enter 5-letter word, ENTER to submit]";
        }
        
        int spaces = 70 - (15 + (int)currentGuess.length() * 2);
        for (int i = 0; i < spaces; ++i) cout << " ";
        
        console.setColor(36);
        cout << "┘  ║\n";
        cout << "║  └────────────────────────────────────────────────────────────────────┘  ║\n";
        console.reset();
    }
    
    void drawStatusMessage(const string& message, int color) {
        console.setColor(color);
        cout << "║  ";
        cout << message;
        int spaces = 76 - (int)message.length();
        for (int i = 0; i < spaces; ++i) cout << " ";
        cout << "║\n";
        console.reset();
    }
    
    void drawInterface(const string& statusMsg = "", int statusColor = 37) {
        clearScreen();
        drawTopBorder();
        drawHeader();
        drawSidePanels();
        drawTimer();
        
        console.setColor(36);
        cout << "║  ┌────────────────────────────────────────────────────────────────────┐  ║\n";
        console.reset();
        
        drawWordleGrid();
        
        console.setColor(36);
        cout << "║  └────────────────────────────────────────────────────────────────────┘  ║\n";
        console.reset();
        
        drawInputArea();
        
        if (!statusMsg.empty()) {
            drawStatusMessage(statusMsg, statusColor);
        }
        
        drawBottomBorder();
    }
    
    bool evaluateGuess() {
        if (currentGuess.length() != 5) {
            drawInterface("INCOMPLETE TEMPORAL DATA - Need 5 letters!", 33);
            this_thread::sleep_for(chrono::milliseconds(1000));
            return false;
        }
        
        for (char& c : currentGuess) {
            c = toupper(c);
        }
        
        if (currentGuess == targetWord) {
            for (int i = 0; i < 5; ++i) {
                grid[currentRow][i].letter = currentGuess[i];
                grid[currentRow][i].state = CORRECT;
            }
            gameWon = true;
            gameActive = false;
            guessTimer.stop();
            drawInterface();
            drawStatusMessage(successMessages[rand() % successMessages.size()], 32);
            drawStatusMessage("★ TIMELINE RESTORED! ★", 36);
            return true;
        }
        
        map<char, int> targetCount;
        for (char c : targetWord) {
            targetCount[c]++;
        }
        
        for (int i = 0; i < 5; ++i) {
            if (currentGuess[i] == targetWord[i]) {
                grid[currentRow][i].letter = currentGuess[i];
                grid[currentRow][i].state = CORRECT;
                targetCount[currentGuess[i]]--;
            }
        }
        
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
        
        drawInterface(failureMessages[rand() % failureMessages.size()], 36);
        this_thread::sleep_for(chrono::milliseconds(800));
        
        currentRow++;
        currentGuess = "";
        guessTimer.start(timeLimit);
        
        if (currentRow >= 6) {
            gameActive = false;
            drawInterface();
            drawStatusMessage("✗ TIMELINE CORRUPTED!", 31);
            drawStatusMessage("The historical fragment was: " + targetWord, 33);
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
    
    bool keyAvailable() {
#ifdef _WIN32
        return _kbhit();
#else
        struct termios oldt, newt;
        int oldf;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        int ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        if (ch != EOF) {
            ungetc(ch, stdin);
            return true;
        }
        return false;
#endif
    }
    
    void run() {
        drawInterface("Welcome, Time Agent. Decode the historical fragment.", 36);
        this_thread::sleep_for(chrono::milliseconds(2000));
        
        while (gameActive && !gameWon && currentRow < 6 && !exitRequested) {
            drawInterface();
            
            if (guessTimer.isExpired()) {
                drawInterface("⚠ TIME PARADOX! Guess expired! ⚠", 31);
                this_thread::sleep_for(chrono::milliseconds(1500));
                
                currentRow++;
                currentGuess = "";
                
                if (currentRow < 6) {
                    guessTimer.start(timeLimit);
                }
                
                if (currentRow >= 6) {
                    gameActive = false;
                    drawInterface();
                    drawStatusMessage("✗ TIMELINE CORRUPTED!", 31);
                    drawStatusMessage("The historical fragment was: " + targetWord, 33);
                    break;
                }
                continue;
            }
            
            if (keyAvailable() && gameActive && !gameWon) {
                char key = getKey();
                
                if (key == 27) { // ESC key
                    exitRequested = true;
                    drawInterface("EXITING GAME...", 31);
                    this_thread::sleep_for(chrono::milliseconds(1000));
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
        
        drawInterface();
        if (gameWon && !exitRequested) {
            drawStatusMessage("★ TEMPORAL ARCHIVE COMPLETE! ★", 32);
            drawStatusMessage("You have successfully restored the timeline!", 36);
        } else if (!exitRequested) {
            drawStatusMessage("✗ TEMPORAL ARCHIVE FAILED ✗", 31);
            drawStatusMessage("The historical fragment was: " + targetWord, 33);
        } else {
            drawStatusMessage("✗ GAME EXITED ✗", 31);
            drawStatusMessage("Timeline preservation aborted.", 33);
        }
        
        drawStatusMessage("Press any key to exit...", 37);
        getKey();
    }
};

int runWordleApp() {
    cout << "\033[2J\033[1;1H";
    cout << "\n\n\n";
    
    ConsoleColor console;
    console.setColor(36);
    cout << "   ╔══════════════════════════════════════════════════════════════════════╗\n";
    cout << "   ║                                                                      ║\n";
    cout << "   ║     ████████╗██╗███╗   ███╗███████╗                                 ║\n";
    cout << "   ║     ╚══██╔══╝██║████╗ ████║██╔════╝                                 ║\n";
    cout << "   ║        ██║   ██║██╔████╔██║█████╗                                   ║\n";
    cout << "   ║        ██║   ██║██║╚██╔╝██║██╔══╝                                   ║\n";
    cout << "   ║        ██║   ██║██║ ╚═╝ ██║███████╗                                 ║\n";
    cout << "   ║        ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝                                 ║\n";
    cout << "   ║                                                                      ║\n";
    cout << "   ║              THE TIME-SLIP SYNDICATE                                 ║\n";
    cout << "   ║            ARCHITECTURAL WORDLE v1.0                                 ║\n";
    cout << "   ║                                                                      ║\n";
    cout << "   ╚══════════════════════════════════════════════════════════════════════╝\n";
    console.reset();
    
    cout << "\n\n";
    console.setColor(33);
    cout << "   ╔══════════════════════════════════════════════════════════════════════╗\n";
    cout << "   ║                         SELECT DIFFICULTY                           ║\n";
    cout << "   ╠══════════════════════════════════════════════════════════════════════╣\n";
    cout << "   ║                                                                      ║\n";
    cout << "   ║    1. EASY     - 30s per guess, visual aids, hints                  ║\n";
    cout << "   ║    2. MEDIUM   - 20s per guess, standard challenge                  ║\n";
    cout << "   ║    3. HARD     - 10s per guess, expert time trial, no hints         ║\n";
    cout << "   ║                                                                      ║\n";
    cout << "   ╚══════════════════════════════════════════════════════════════════════╝\n";
    console.reset();
    
    cout << "\n   Choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    cin.clear();
    
    Difficulty diff;
    switch(choice) {
        case 1: diff = EASY; break;
        case 2: diff = MEDIUM; break;
        case 3: diff = HARD; break;
        default: diff = MEDIUM;
    }
    
    TimeSlipSyndicate game(diff);
    game.run();
    
    return 0;
}

int main() {
    return runWordleApp();
}
