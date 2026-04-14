#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>
#include <conio.h>  // For _getch() on Windows
#include <windows.h> // For console manipulation

using namespace std;
using namespace chrono;

// Console colors
enum Color {
    BLACK = 0,
    DARK_GREEN = 2,
    DARK_YELLOW = 6,
    DARK_GRAY = 8,
    GRAY = 7,
    WHITE = 15,
    GREEN = 10,
    YELLOW = 14,
    RED = 12,
    CYAN = 11
};

// Difficulty levels
enum Difficulty {
    EASY,
    MEDIUM,
    HARD
};

// Cell state for Wordle grid
enum CellState {
    EMPTY,
    CORRECT,    // Green
    MISPLACED,  // Yellow
    WRONG       // Gray
};

struct Cell {
    char letter;
    CellState state;
};

// Word banks
const vector<string> EASY_WORDS = {"RUINS", "MAYA", "ARCH", "DOME", "WALL", "TOWER", "TEMPLE", "STONE", "BRICK", "PILLAR"};
const vector<string> MEDIUM_WORDS = {"TOMBS", "AZTEC", "STOAS", "PYLON", "OBELI", "MAYAN", "INCA", "FORUM", "COLUMN", "VAULT"};
const vector<string> HARD_WORDS = {"ZIGG", "STELA", "LINTL", "OGIVE", "TRABE", "DORIC", "IONIC", "CORIN", "CARYA", "ROSET"};

class TimeSlipSyndicate {
private:
    string targetWord;
    vector<vector<Cell>> grid;
    int currentRow;
    int currentCol;
    int attempts;
    bool gameWon;
    Difficulty difficulty;
    string currentGuess;
    time_point<steady_clock> guessStartTime;
    int timeLimit;
    bool gameActive;
    
    // Word bank based on difficulty
    vector<string> wordBank;
    
public:
    TimeSlipSyndicate(Diff d) : difficulty(d), currentRow(0), currentCol(0), attempts(6), gameWon(false), gameActive(true) {
        // Set time limit based on difficulty
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
        
        // Select random target word
        srand(time(0));
        targetWord = wordBank[rand() % wordBank.size()];
        
        // Initialize grid
        grid.resize(6, vector<Cell>(5));
        for(int i = 0; i < 6; i++) {
            for(int j = 0; j < 5; j++) {
                grid[i][j].letter = '_';
                grid[i][j].state = EMPTY;
            }
        }
        
        // Start timer for first guess
        guessStartTime = steady_clock::now();
    }
    
    void setColor(Color color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
    
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
    
    void clearScreen() {
        system("cls");
    }
    
    void drawBorder() {
        setColor(CYAN);
        
        // Top border
        gotoxy(0, 0);
        cout << "╔══════════════════════════════════════════════════════════════════╗";
        
        // Side borders
        for(int i = 1; i <= 25; i++) {
            gotoxy(0, i);
            cout << "║";
            gotoxy(70, i);
            cout << "║";
        }
        
        // Bottom border
        gotoxy(0, 26);
        cout << "╚══════════════════════════════════════════════════════════════════╝";
        
        setColor(WHITE);
    }
    
    void drawHeader() {
        setColor(CYAN);
        gotoxy(5, 1);
        cout << "[TEMPORAL ARCHIVE v2.47]";
        
        setColor(GREEN);
        gotoxy(35, 1);
        cout << "[SCANNING]";
        
        setColor(YELLOW);
        gotoxy(46, 1);
        cout << "[ONLINE]";
        
        setColor(GREEN);
        gotoxy(55, 1);
        cout << "[SECURE]";
        
        setColor(WHITE);
        
        // Flickering scan line effect
        gotoxy(2, 2);
        setColor(DARK_GRAY);
        cout << "⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡";
        setColor(WHITE);
    }
    
    void drawSidePanels() {
        // Left panel - Era indicators
        setColor(DARK_YELLOW);
        gotoxy(2, 4);
        cout << "[CLASSICAL]";
        gotoxy(2, 6);
        cout << "[MEDIEVAL]";
        gotoxy(2, 8);
        cout << "[MODERN]";
        
        // Era-specific symbols
        setColor(GRAY);
        gotoxy(2, 5);
        cout << "🏛️";
        gotoxy(2, 7);
        cout << "🏰";
        gotoxy(2, 9);
        cout << "🏭";
        
        setColor(WHITE);
    }
    
    void drawTimer() {
        auto now = steady_clock::now();
        int elapsed = duration_cast<seconds>(now - guessStartTime).count();
        int remaining = max(0, timeLimit - elapsed);
        
        gotoxy(55, 4);
        setColor(remaining < 5 ? RED : YELLOW);
        cout << "[TIME: " << remaining << "s";
        
        // Progress bar
        int progress = (remaining * 10) / timeLimit;
        cout << " ";
        for(int i = 0; i < progress; i++) cout << "█";
        for(int i = progress; i < 10; i++) cout << "░";
        cout << "]";
        
        if(remaining <= 0 && gameActive && !gameWon) {
            setColor(RED);
            gotoxy(55, 5);
            cout << "[!TEMPORAL OVERFLOW!]";
            gameActive = false;
        }
        
        setColor(WHITE);
    }
    
    void drawWordleGrid() {
        int startX = 20;
        int startY = 4;
        
        for(int row = 0; row < 6; row++) {
            int y = startY + (row * (difficulty == HARD ? 1 : 3));
            
            // Row highlighting for current attempt
            if(row == currentRow && gameActive && !gameWon) {
                setColor(CYAN);
                gotoxy(startX - 2, y);
                cout << "▶";
            }
            
            for(int col = 0; col < 5; col++) {
                int x = startX + (col * (difficulty == HARD ? 2 : 4));
                
                // Set background color based on cell state
                switch(grid[row][col].state) {
                    case CORRECT:
                        setColor(GREEN);
                        break;
                    case MISPLACED:
                        setColor(YELLOW);
                        break;
                    case WRONG:
                        setColor(DARK_GRAY);
                        break;
                    default:
                        setColor(GRAY);
                }
                
                // Draw cell background
                if(difficulty == HARD) {
                    gotoxy(x, y);
                    cout << (grid[row][col].state == EMPTY ? " _ " : 
                            (grid[row][col].state == CORRECT ? "█" + string(1, grid[row][col].letter) + "█" :
                             (grid[row][col].state == MISPLACED ? "▒" + string(1, grid[row][col].letter) + "▒" :
                              "░" + string(1, grid[row][col].letter) + "░")));
                } else if(difficulty == MEDIUM) {
                    gotoxy(x, y);
                    cout << "┌──┐";
                    gotoxy(x, y+1);
                    if(grid[row][col].state == EMPTY) cout << "│__│";
                    else if(grid[row][col].state == CORRECT) cout << "│" << grid[row][col].letter << grid[row][col].letter << "│";
                    else if(grid[row][col].state == MISPLACED) cout << "│" << grid[row][col].letter << grid[row][col].letter << "│";
                    else cout << "│" << grid[row][col].letter << grid[row][col].letter << "│";
                    gotoxy(x, y+2);
                    cout << "└──┘";
                } else {
                    gotoxy(x, y);
                    cout << "┌───┐";
                    gotoxy(x, y+1);
                    if(grid[row][col].state == EMPTY) cout << "│ " << grid[row][col].letter << " │";
                    else if(grid[row][col].state == CORRECT) cout << "│ " << grid[row][col].letter << " │";
                    else if(grid[row][col].state == MISPLACED) cout << "│ " << grid[row][col].letter << " │";
                    else cout << "│ " << grid[row][col].letter << " │";
                    gotoxy(x, y+2);
                    cout << "└───┘";
                }
            }
            setColor(WHITE);
        }
    }
    
    void drawInputArea() {
        gotoxy(15, difficulty == HARD ? 20 : 22);
        setColor(CYAN);
        cout << "[GUESS: ";
        
        // Show current typing
        for(int i = 0; i < currentGuess.length(); i++) {
            setColor(WHITE);
            cout << currentGuess[i];
            if(i < currentGuess.length() - 1) cout << " ";
        }
        for(int i = currentGuess.length(); i < 5; i++) {
            setColor(DARK_GRAY);
            cout << "_ ";
        }
        
        setColor(CYAN);
        cout << "]";
        
        // Show hint based on difficulty
        gotoxy(15, difficulty == HARD ? 22 : 24);
        setColor(DARK_YELLOW);
        if(difficulty == EASY && currentRow == 2 && !gameWon) {
            cout << "[HINT: Common letters in era: R,U,I,N,S]";
        } else if(difficulty == MEDIUM && currentRow == 3 && !gameWon) {
            cout << "[HINT: Position 2 contains consonant]";
        } else {
            cout << "[Enter word and press ENTER]";
        }
        
        setColor(WHITE);
    }
    
    void drawStatusMessage(const string& message, Color color) {
        gotoxy(15, difficulty == HARD ? 24 : 26);
        setColor(color);
        cout << message;
        this_thread::sleep_for(milliseconds(1500));
        setColor(WHITE);
    }
    
    void evaluateGuess() {
        if(currentGuess.length() != 5) return;
        
        // Convert to uppercase
        for(char& c : currentGuess) c = toupper(c);
        
        // Check win condition
        if(currentGuess == targetWord) {
            for(int i = 0; i < 5; i++) {
                grid[currentRow][i].letter = currentGuess[i];
                grid[currentRow][i].state = CORRECT;
            }
            gameWon = true;
            drawWordleGrid();
            drawStatusMessage("✓ TEMPORAL FRAGMENT RECOVERED! ✓", GREEN);
            drawStatusMessage("ARCHIVE COMPLETE - TIMELINE RESTORED", CYAN);
            return;
        }
        
        // Count letters in target
        map<char, int> targetCount;
        for(char c : targetWord) targetCount[c]++;
        
        // First pass: mark correct positions
        for(int i = 0; i < 5; i++) {
            if(currentGuess[i] == targetWord[i]) {
                grid[currentRow][i].letter = currentGuess[i];
                grid[currentRow][i].state = CORRECT;
                targetCount[currentGuess[i]]--;
            }
        }
        
        // Second pass: mark misplaced letters
        for(int i = 0; i < 5; i++) {
            if(grid[currentRow][i].state != CORRECT) {
                if(targetCount[currentGuess[i]] > 0) {
                    grid[currentRow][i].letter = currentGuess[i];
                    grid[currentRow][i].state = MISPLACED;
                    targetCount[currentGuess[i]]--;
                } else {
                    grid[currentRow][i].letter = currentGuess[i];
                    grid[currentRow][i].state = WRONG;
                }
            }
        }
        
        drawWordleGrid();
        
        // Generate immersive message
        vector<string> messages = {
            "Temporal scan complete",
            "Historical fragment detected",
            "Artifact mismatch",
            "Anomaly in timeline",
            "Resonance pattern found"
        };
        
        drawStatusMessage(messages[rand() % messages.size()], CYAN);
        
        currentRow++;
        currentGuess = "";
        guessStartTime = steady_clock::now();
        
        if(currentRow >= 6 && !gameWon) {
            gameActive = false;
            drawStatusMessage("✗ TIMELINE CORRUPTED - ANSWER: " + targetWord, RED);
        }
    }
    
    void run() {
        clearScreen();
        drawBorder();
        drawHeader();
        drawSidePanels();
        drawWordleGrid();
        
        char key;
        while(gameActive && !gameWon && currentRow < 6) {
            drawTimer();
            drawInputArea();
            
            if(kbhit()) {
                key = _getch();
                
                if(key == 13) { // Enter key
                    if(currentGuess.length() == 5) {
                        evaluateGuess();
                        drawWordleGrid();
                    } else {
                        drawStatusMessage("INCOMPLETE TEMPORAL DATA - Need 5 letters", YELLOW);
                    }
                } 
                else if(key == 8) { // Backspace
                    if(!currentGuess.empty()) {
                        currentGuess.pop_back();
                        drawInputArea();
                    }
                }
                else if(isalpha(key)) {
                    if(currentGuess.length() < 5) {
                        currentGuess += toupper(key);
                        drawInputArea();
                    }
                }
            }
            
            // Check timer
            auto now = steady_clock::now();
            if(duration_cast<seconds>(now - guessStartTime).count() >= timeLimit) {
                drawStatusMessage("TIME PARADOX - Guess expired!", RED);
                if(difficulty == HARD) {
                    // Screen shake effect
                    for(int i = 0; i < 3; i++) {
                        gotoxy(0, 0);
                        this_thread::sleep_for(milliseconds(50));
                    }
                }
                currentRow++;
                currentGuess = "";
                guessStartTime = steady_clock::now();
                if(currentRow >= 6) {
                    gameActive = false;
                    drawStatusMessage("✗ TIMELINE CORRUPTED - ANSWER: " + targetWord, RED);
                }
                drawWordleGrid();
            }
            
            this_thread::sleep_for(milliseconds(50));
        }
        
        // Game over message
        gotoxy(25, 28);
        if(gameWon) {
            setColor(GREEN);
            cout << "★ TEMPORAL ARCHIVE COMPLETE - YOU SAVED HISTORY! ★";
        } else {
            setColor(RED);
            cout << "✗ TIMELINE FRACTURED - ANOTHER SLIP MUST RESTORE IT ✗";
        }
        setColor(WHITE);
        gotoxy(30, 30);
        cout << "Press any key to exit...";
        _getch();
    }
};

int main() {
    // Setup console
    system("mode con: cols=80 lines=32");
    system("title The Time-Slip Syndicate");
    
    // Hide cursor
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(console, &cursorInfo);
    
    // Welcome screen
    cout << "\n\n\n";
    cout << "   ╔══════════════════════════════════════════════════════════╗\n";
    cout << "   ║                                                          ║\n";
    cout << "   ║     ████████╗██╗███╗   ███╗███████╗                     ║\n";
    cout << "   ║     ╚══██╔══╝██║████╗ ████║██╔════╝                     ║\n";
    cout << "   ║        ██║   ██║██╔████╔██║█████╗                       ║\n";
    cout << "   ║        ██║   ██║██║╚██╔╝██║██╔══╝                       ║\n";
    cout << "   ║        ██║   ██║██║ ╚═╝ ██║███████╗                     ║\n";
    cout << "   ║        ╚═╝   ╚═╝╚═╝     ╚═╝╚══════╝                     ║\n";
    cout << "   ║                                                          ║\n";
    cout << "   ║           THE TIME-SLIP SYNDICATE                        ║\n";
    cout << "   ║         ARCHITECTURAL WORDLE v1.0                        ║\n";
    cout << "   ║                                                          ║\n";
    cout << "   ╚══════════════════════════════════════════════════════════╝\n\n\n";
    
    cout << "   Select Difficulty:\n";
    cout << "   1. EASY   - 30s per guess, visual aids\n";
    cout << "   2. MEDIUM - 20s per guess, standard challenge\n";
    cout << "   3. HARD   - 10s per guess, expert time trial\n\n";
    cout << "   Choice: ";
    
    int choice;
    cin >> choice;
    
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
