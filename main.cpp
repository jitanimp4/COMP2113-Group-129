#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "cryptex_boss.h"
#include "celluloid_app.h"
#include "word_pyramid.h"
#include "game.h"

using namespace std;

int runWordleApp();

string toLowerMain(string text)
{
    for (int i = 0; i < text.length(); i++)
        text[i] = tolower((unsigned char)text[i]);
    return text;
}

bool isCryptexUnlocked(bool game1Done, bool game2Done, bool game3Done)
{
    return game1Done && game2Done && game3Done;
}

void pauseMenu()
{
    cout << "\nPress ENTER to return to menu...";
    string temp;
    getline(cin, temp);
}

int main()
{
    srand(time(0));

    bool wordPyramidDone = false;
    bool directorsArchiveDone = false;
    bool timeSlipDone = false;

    while (true)
    {
        cout << "\n====================================\n";
        cout << "        ODYSSEY OS MAIN MENU         \n";
        cout << "====================================\n";
        cout << "1. Word Pyramids of Giza\n";
        cout << "2. The Director's Archive\n";
        cout << "3. The Time-Slip Syndicate\n";
        cout << "4. Crossword\n";

        if (isCryptexUnlocked(wordPyramidDone, directorsArchiveDone, timeSlipDone))
            cout << "5. The Curator's Cryptex\n";
        else
            cout << "5. The Curator's Cryptex (LOCKED)\n";

        cout << "0. Exit\n";
        cout << "Select App > ";

        int choice;
        cin >> choice;
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
            cout << "Select difficulty: 1 Easy, 2 Medium, 3 Hard: ";
            cin >> difficulty;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
            CrosswordGame game("save_data.txt");
            game.run();
            pauseMenu();
        }
        else if (choice == 5)
        {
            if (!isCryptexUnlocked(wordPyramidDone, directorsArchiveDone, timeSlipDone))
            {
                cout << "\nCRYPTEX LOCKED.\n";
                cout << "Complete Word Pyramid, Director's Archive, and Time-Slip first.\n";
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
