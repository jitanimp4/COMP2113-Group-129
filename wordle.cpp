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

/**
 * @enum Difficulty
 * @brief Game difficulty levels
 * 
 * EASY   - 30 seconds per guess, includes hints
 * MEDIUM - 20 seconds per guess, standard challenge
 * HARD   - 10 seconds per guess, expert mode, no hints
 */
enum Difficulty { EASY, MEDIUM, HARD };

/**
 * @enum CellState
 * @brief State of a cell in the Wordle grid
 * 
 * EMPTY      - No letter entered yet
 * CORRECT    - Letter is correct and in correct position (Green)
 * MISPLACED  - Letter is correct but wrong position (Yellow)
 * WRONG      - Letter is not in the target word (Gray)
 */
enum CellState { EMPTY, CORRECT, MISPLACED, WRONG };

/**
 * @struct Cell
 * @brief Represents a single cell in the game grid
 * 
 * Contains a character letter and its current state in the game
 */
struct Cell {
    char letter;      ///< The character entered by the player
    CellState state;  ///< Current state of this cell
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
    "ZIGG", "STELA", "LINTL", "OGIVE", "TRABE",
    "DORIC", "IONIC", "CORIN", "CARYA", "ROSET"
};

// ================================
// Console Color Class (Cross-platform)
// ================================

/**
 * @class ConsoleColor
 * @brief Handles colored console output across Windows and Linux platforms
 * 
 * Uses ANSI escape codes on Linux and SetConsoleTextAttribute on Windows
 */
class ConsoleColor {
private:
#ifdef _WIN32
    HANDLE hConsole;  ///< Windows console handle
#endif
public:
    /**
     * @brief Constructor - Initializes console handle on Windows
     * @return None
     */
    ConsoleColor() {
#ifdef _WIN32
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
    }
    
    /**
     * @brief Sets the text color for subsequent console output
     * @param color Integer color code (ANSI color codes or Windows color values)
     * @return None
     */
    void setColor(int color) {
#ifdef _WIN32
        SetConsoleTextAttribute(hConsole, color);
#else
        cout << "\033[" << color << "m";
#endif
    }
    
    /**
     * @brief Resets console text color to default (white/gray)
     * @return None
     */
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

/**
 * @class Timer
 * @brief Timer class for tracking time-limited guesses
 * 
 * Uses high-resolution chrono for accurate timing
 */
class Timer {
private:
    steady_clock::time_point startTime;  ///< Time when timer was started
    int limit;                           ///< Time limit in seconds
    bool running;                        ///< Whether timer is active
    
public:
    /**
     * @brief Constructor - initializes timer to stopped state
     * @return None
     */
    Timer() : limit(0), running(false) {}
    
    /**
     * @brief Starts the timer with specified time limit
     * @param seconds Time limit in seconds
     * @return None
     */
    void start(int seconds) {
        limit = seconds;
        startTime = steady_clock::now();
        running = true;
    }
    
    /**
     * @brief Gets remaining time on the timer
     * @return int Number of seconds remaining (0 if expired)
     */
    int getRemaining() {
        if (!running) return limit;
        auto now = steady_clock::now();
        int elapsed = duration_cast<seconds>(now - startTime).count();
        int remaining = max(0, limit - elapsed);
        if (remaining <= 0) running = false;
        return remaining;
    }
    
    /**
     * @brief Checks if timer has expired
     * @return bool True if time is up, false otherwise
     */
    bool isExpired() {
        return getRemaining() <= 0;
    }
    
    /**
     * @brief Stops the timer (remains stopped until start() called again)
     * @return None
     */
    void stop() {
        running = false;
    }
};

// ================================
// Game Class
// ================================

/**
 * @class TimeSlipSyndicate
 * @brief Main game class for The Time-Slip Syndicate
 * 
 * Handles game logic, UI rendering, and user input for the Wordle-style game
 */
class TimeSlipSyndicate {
private:
    string targetWord;                    ///< The correct word to guess
    vector<vector<Cell>> grid;            ///< 6x5 Wordle game grid
    int currentRow;                       ///< Current row being filled (0-5)
    bool gameWon;                         ///< Whether player has won
    bool gameActive;                      ///< Whether game is still active
    Difficulty difficulty;                ///< Selected difficulty level
    string currentGuess;                  ///< Current guess being typed
    Timer guessTimer;                     ///< Timer for current guess
    int timeLimit;                        ///< Time limit based on difficulty
    vector<string> wordBank;              ///< Words for selected difficulty
    ConsoleColor console;                 ///< Console color handler
    
    // Immersive messages for feedback
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
    /**
     * @brief Constructor - Initializes game with selected difficulty
     * @param diff Difficulty level (EASY/MEDIUM/HARD)
     * @return None
     * 
     * Sets time limit, selects random target word from appropriate word bank,
     * initializes empty grid, and starts the timer.
     */
    TimeSlipSyndicate(Difficulty diff) : 
        currentRow(0), 
        gameWon(false), 
        gameActive(true),
        difficulty(diff) {
        
        // Set time limit and word bank based on difficulty
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
        
        // Seed random number generator
        srand(time(nullptr));
        
        // Select random target word
        targetWord = wordBank[rand() % wordBank.size()];
        
        // Initialize grid (6 rows x 5 columns)
        grid.resize(6, vector<Cell>(5));
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 5; ++j) {
                grid[i][j].letter = '_';
                grid[i][j].state = EMPTY;
            }
        }
        
        // Start the timer
        guessTimer.start(timeLimit);
    }
    
    /**
     * @brief Clears the console screen (cross-platform)
     * @return None
     * Uses ANSI escape codes on Linux, alternative on Windows
     */
    void clearScreen() {
        cout << "\033[2J\033[1;1H";
    }
    
    /**
     * @brief Draws the top border of the game interface
     * @return None
     */
    void drawTopBorder() {
        console.setColor(36);
        cout << "╔════════════════════════════════════════════════════════════════════════════════╗\n";
        console.reset();
    }
    
    /**
     * @brief Draws the bottom border of the game interface
     * @return None
     */
    void drawBottomBorder() {
        console.setColor(36);
        cout << "╚════════════════════════════════════════════════════════════════════════════════╝\n";
        console.reset();
    }
    
    /**
     * @brief Draws the header with system status indicators
     * @return None
     * Displays [SCANNING], [ONLINE], [SECURE] with flickering scan line effect
     */
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
        
        // Flickering scan line effect
        console.setColor(90);
        cout << "║  ⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡⌠⌡  ║\n";
        console.reset();
    }
    
    /**
     * @brief Draws side panels with era indicators (Classical/Medieval/Modern)
     * @return None
     */
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
    
    /**
     * @brief Draws the timer and attempt counter in the interface
     * @return None
     * Displays countdown timer with visual progress bar (█ for remaining, ░ for elapsed)
     */
    void drawTimer() {
        int remaining = guessTimer.getRemaining();
        
        cout << "║  ";
        console.setColor(remaining < 5 ? 31 : 33);
        cout << "[TIME: " << setw(2) << remaining << "s ";
        
        // Progress bar
        int progress = (remaining * 10) / timeLimit;
        for (int i = 0; i < progress; ++i) cout << "█";
        for (int i = progress; i < 10; ++i) cout << "░";
        cout << "]";
        
        console.reset();
        
        // Show attempt counter
        console.setColor(36);
        cout << "  [ATTEMPT: " << currentRow + 1 << "/6]";
        console.reset();
        
        // Adjust spacing to align with right border
        int spaces = 69 - (15 + (remaining < 5 ? 0 : 0));
        for (int i = 0; i < spaces; ++i) cout << " ";
        cout << "║\n";
    }
    
    /**
     * @brief Draws the 6x5 Wordle game grid with color-coded cells
     * @return None
     * Grid layout changes based on difficulty (HARD uses compact layout)
     */
    void drawWordleGrid() {
        for (int row = 0; row < 6; ++row) {
            cout << "║  ";
            
            // Row highlighting for current attempt
            if (row == currentRow && gameActive && !gameWon) {
                console.setColor(36);
                cout << "▶";
                console.reset();
                cout << " ";
            } else {
                cout << "  ";
            }
            
            for (int col = 0; col < 5; ++col) {
                // Set color based on cell state
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
                
                // Draw cell based on difficulty
                if (difficulty == HARD) {
                    cout << " ";
                    if (grid[row][col].state == EMPTY) {
                        console.setColor(90);
                        cout << "_";
                    } else {
                        cout << grid[row][col].letter;
                    }
                    cout << "  ";
                } else if (difficulty == MEDIUM) {
                    if (grid[row][col].state == EMPTY) {
                        cout << "┌──┐ ";
                    } else {
                        cout << "┌──┐ ";
                    }
                } else {
                    if (grid[row][col].state == EMPTY) {
                        cout << "┌───┐ ";
                    } else {
                        cout << "┌───┐ ";
                    }
                }
            }
            cout << "║\n";
            
            // Second line of cells
            if (difficulty != HARD) {
                cout << "║    ";
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
                    
                    if (difficulty == MEDIUM) {
                        if (grid[row][col].state == EMPTY) {
                            cout << "│__│ ";
                        } else {
                            cout << "│" << grid[row][col].letter 
                                 << grid[row][col].letter << "│ ";
                        }
                    } else {
                        if (grid[row][col].state == EMPTY) {
                            cout << "│ " << grid[row][col].letter << " │ ";
                        } else {
                            cout << "│ " << grid[row][col].letter << " │ ";
                        }
                    }
                }
                cout << "║\n";
                
                // Third line of cells
                cout << "║    ";
                for (int col = 0; col < 5; ++col) {
                    console.setColor(37);
                    if (difficulty == MEDIUM) {
                        cout << "└──┘ ";
                    } else {
                        cout << "└───┘ ";
                    }
                }
                cout << "║\n";
            }
            
            console.reset();
        }
    }
    
    /**
     * @brief Draws the input area where player types their guess
     * @return None
     * Shows current typed letters, hints based on difficulty level
     */
    void drawInputArea() {
        console.setColor(36);
        cout << "║  ┌────────────────────────────────────────────────────────────────────┐  ║\n";
        cout << "║  │ [GUESS: ";
        
        // Show current typing
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
        
        // Show hint based on difficulty and progress
        if (difficulty == EASY && currentRow == 2 && !gameWon) {
            console.setColor(33);
            cout << "  [HINT: Common letters: R,U,I,N,S]";
        } else if (difficulty == MEDIUM && currentRow == 3 && !gameWon) {
            console.setColor(33);
            cout << "  [HINT: Position 2 contains consonant]";
        } else {
            console.setColor(90);
            cout << "  [Enter 5-letter word and press ENTER]";
        }
        
        // Fill remaining space
        int spaces = 70 - (15 + (int)currentGuess.length() * 2);
        for (int i = 0; i < spaces; ++i) cout << " ";
        
        console.setColor(36);
        cout << "┘  ║\n";
        cout << "║  └────────────────────────────────────────────────────────────────────┘  ║\n";
        console.reset();
    }
    
    /**
     * @brief Draws a status message in the game interface
     * @param message The text to display
     * @param color The color code for the message
     * @return None
     */
    void drawStatusMessage(const string& message, int color) {
        console.setColor(color);
        cout << "║  ";
        cout << message;
        // Fill remaining space
        int spaces = 76 - (int)message.length();
        for (int i = 0; i < spaces; ++i) cout << " ";
        cout << "║\n";
        console.reset();
    }
    
    /**
     * @brief Draws the complete game interface
     * @param statusMsg Optional status message to display (default: empty)
     * @param statusColor Color for the status message (default: 37 - white)
     * @return None
     */
    void drawInterface(const string& statusMsg = "", int statusColor = 37) {
        clearScreen();
        drawTopBorder();
        drawHeader();
        drawSidePanels();
        drawTimer();
        
        // Draw separator
        console.setColor(36);
        cout << "║  ┌────────────────────────────────────────────────────────────────────┐  ║\n";
        console.reset();
        
        drawWordleGrid();
        
        // Draw separator
        console.setColor(36);
        cout << "║  └────────────────────────────────────────────────────────────────────┘  ║\n";
        console.reset();
        
        drawInputArea();
        
        if (!statusMsg.empty()) {
            drawStatusMessage(statusMsg, statusColor);
        }
        
        drawBottomBorder();
    }
    
    /**
     * @brief Evaluates the player's guess against the target word
     * @return bool True if the guess was correct (win condition), false otherwise
     * 
     * Implements Wordle logic:
     * - Green: Correct letter, correct position
     * - Yellow: Correct letter, wrong position
     * - Gray: Letter not in word
     */
    bool evaluateGuess() {
        if (currentGuess.length() != 5) {
            drawInterface("INCOMPLETE TEMPORAL DATA - Need 5 letters!", 33);
            this_thread::sleep_for(chrono::milliseconds(1000));
            return false;
        }
        
        // Convert to uppercase
        for (char& c : currentGuess) {
            c = toupper(c);
        }
        
        // Check win condition
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
    
    /**
     * @brief Gets a single character from keyboard input without requiring Enter
     * @return char The key pressed by the user
     * 
     * Cross-platform implementation using _getch() on Windows and termios on Linux
     */
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
    
    /**
     * @brief Checks if a key is available to read without blocking
     * @return bool True if a key is in the input buffer, false otherwise
     */
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
    
    /**
     * @brief Main game loop that runs the game until win/loss
     * @return None
     * 
     * Handles:
     * - Timer checking and expiration
     * - Keyboard input processing
     * - Game state updates
     * - Display updates
     */
    void run() {
        drawInterface("Welcome, Time Agent. Decode the historical fragment.", 36);
        this_thread::sleep_for(chrono::milliseconds(2000));
        
        // Main game loop
        while (gameActive && !gameWon && currentRow < 6) {
            drawInterface();
            
            // Check timer expiration
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
            
            // Handle input
            if (keyAvailable() && gameActive && !gameWon) {
                char key = getKey();
                
                if (key == '\n' || key == '\r') {
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
        
        // Game over message
        drawInterface();
        if (gameWon) {
            drawStatusMessage("★ TEMPORAL ARCHIVE COMPLETE! ★", 32);
            drawStatusMessage("You have successfully restored the timeline!", 36);
        } else {
            drawStatusMessage("✗ TEMPORAL ARCHIVE FAILED ✗", 31);
            drawStatusMessage("Another agent must attempt to restore history.", 33);
        }
        
        drawStatusMessage("Press any key to exit...", 37);
        getKey();
    }
};

// ================================
// Main Function
// ================================

/**
 * @brief Main entry point for the Time-Slip Syndicate game
 * @return int Returns 0 on successful execution
 * 
 * Displays welcome screen, gets difficulty choice, and starts the game.
 * This function is called runWordleApp() to integrate with the main menu.
 */
int runWordleApp() {
    // Setup console
    cout << "\033[2J\033[1;1H";
    cout << "\n\n\n";
    
    // Welcome screen
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
