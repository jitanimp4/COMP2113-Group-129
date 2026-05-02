#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

// [PROJECT REQUIREMENT: Program codes in multiple files]
// Including header files to link logic stored in different translation units.
#include "cryptex_boss.h"
#include "celluloid_app.h"
#include "word_pyramid.h"
#include "game.h"
#include "forgers_table.h"

using namespace std;

// Forward declaration
int runWordleApp();

/*
 * Function: toLowerMain
 * What it does: Converts a given string entirely to lowercase letters.
 * What the inputs are: string text (the original string to convert)
 * What the outputs are: string (the converted lowercase string)
 */
string toLowerMain(string text)
{
    for (size_t i = 0; i < text.length(); i++)
        text[i] = tolower((unsigned char)text[i]);
    return text;
}

/*
 * Function: isCryptexUnlocked
 * What it does: Checks if all prerequisite games have been completed to unlock the final boss.
 * What the inputs are: bool game1Done, bool game2Done, bool game3Done, bool game4Done, bool game5Done (completion status flags)
 * What the outputs are: bool (true if all games are completed, false otherwise)
 */
bool isCryptexUnlocked(bool game1Done, bool game2Done, bool game3Done, bool game4Done, bool game5Done)
{
    return game1Done && game2Done && game3Done && game4Done && game5Done;
}

/*
 * Function: pauseMenu
 * What it does: Pauses the application flow until the user presses the ENTER key, allowing them to read the screen.
 * What the inputs are: None
 * What the outputs are: None (Waits for standard input)
 */
void pauseMenu()
{
    // [PROJECT REQUIREMENT: File input/output] -> Console I/O
    cout << "\nPress ENTER to return to menu...";
    string temp;
    getline(cin, temp);
}

/*
 * Function: main
 * What it does: The entry point of the program. Manages the main menu UI and routes the user to different game modules.
 * What the inputs are: None
 * What the outputs are: int (0 indicating successful program termination)
 */
int main()
{
    // [PROJECT REQUIREMENT: Generation of random events]
    // Seeds the legacy random number generator with the current system time.
    srand(time(0));

    // State variables to track game progression
    bool wordPyramidDone = false;
    bool directorsArchiveDone = false;
    bool timeSlipDone = false;
    bool crosswordDone = false;
    bool forgersTableDone = false;

    // Main application loop
    while (true)
    {
        // Console I/O: Printing the menu
        cout << "\n====================================\n";
        cout << "        ODYSSEY OS MAIN MENU         \n";
        cout << "====================================\n";
        cout << "1. Word Pyramids of Giza\n";
        cout << "2. The Director's Archive\n";
        cout << "3. The Time-Slip Syndicate\n";
        cout << "4. Crossword\n";
        cout << "5. The Forger's Table\n";

        if (isCryptexUnlocked(wordPyramidDone, directorsArchiveDone, timeSlipDone, crosswordDone, forgersTableDone))
            cout << "6. The Curator's Cryptex\n";
        else
            cout << "6. The Curator's Cryptex (LOCKED)\n";

        cout << "0. Exit\n";
        cout << "Select App > ";

        int choice;
        cin >> choice;
        // Clears the input buffer to prevent infinite loops on bad input
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0)
        {
            cout << "Logging off Odyssey OS...\n";
            break;
        }
        else if (choice == 1)
        {
            runWordPyramidGame();
            wordPyramidDone = true;
            pauseMenu();
        }
       else if (choice == 2)
        {
            int difficulty;
            
            // Loop until the user provides valid input
            while (true) {
                cout << "Select difficulty: 1 Easy, 2 Medium, 3 Hard: ";
                cin >> difficulty;

                // Check if cin failed (they typed a letter) OR if the number is not 1, 2, or 3
                if (cin.fail() || difficulty < 1 || difficulty > 3) {
                    cout << "\033[1;31m [ERROR] Invalid input. Please enter 1, 2, or 3.\033[0m\n";
                    
                    // 1. Clear the error state so cin works again
                    cin.clear(); 
                    // 2. Throw away the bad input left in the buffer
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                } else {
                    // Valid input received! Clear the trailing newline and break the loop.
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break; 
                }
            }

            CelluloidApp app(difficulty);
            app.run();

            directorsArchiveDone = true;
            pauseMenu();
        }
        else if (choice == 3)
        {
            runWordleApp();
            timeSlipDone = true;
            pauseMenu();
        }
        else if (choice == 4)
        {
            // Note: Ensure "save_data.txt" handling meets the File I/O requirements in game.cpp.
            CrosswordGame game("save_data.txt");
            game.run();
            crosswordDone = true;
            pauseMenu();
        }
        else if (choice == 5)
        {
            runForgersTable();
            forgersTableDone = true;
            pauseMenu();
        }
        else if (choice == 6)
        {
            if (!isCryptexUnlocked(wordPyramidDone, directorsArchiveDone, timeSlipDone, crosswordDone, forgersTableDone))
            {
                cout << "\nCRYPTEX LOCKED.\n";
                cout << "Complete Word Pyramid, Director's Archive, Time-Slip Syndicate, Crossword, and Forger's Table first.\n";
                continue;
            }

            string mode;
            cout << "Select Cryptex mode (easy / medium / hard): ";
            getline(cin, mode);

            runCryptexBoss(toLowerMain(mode));
            pauseMenu();
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
