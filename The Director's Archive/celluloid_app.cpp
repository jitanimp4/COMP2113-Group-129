#include "celluloid_app.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype> 
#include <algorithm> 
#include <random>

using namespace std;

CelluloidApp::CelluloidApp(int difficulty) {
    difficultyLevel = difficulty;
    currentSiteIndex = 0; 

    if (difficulty == 1) {
        lives = 4;
        hintsAllowed = 3;
    } else if (difficulty == 2) {
        lives = 3;
        hintsAllowed = 2;
    } else {
        lives = 2;
        hintsAllowed = 1;
    }
}

bool CelluloidApp::loadFiles() {
    ifstream file("movies.txt");
    if (!file.is_open()) {
        cout << "\033[1;31m [ERROR] Missing 'movies.txt' database file.\033[0m\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // Skip blank lines
        
        stringstream ss(line);
        string item;
        HeritageSite site;

        // Parse the pipe-separated file correctly
        getline(ss, site.movieClue, '|');
        getline(ss, site.answer, '|');
        
        // Push remaining segments as hints
        while (getline(ss, item, '|')) {
            site.hints.push_back(item);
        }
        
        siteDatabase.push_back(site);
    }
    
    file.close();
    
    if (siteDatabase.empty()) {
        cout << "\033[1;31m [ERROR] movies.txt is empty.\033[0m\n";
        return false;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(siteDatabase.begin(), siteDatabase.end(), g);
    
    return true;
}

void CelluloidApp::printFilmReel(int livesRemaining) {
    cout << "\n[FILM REEL: " << livesRemaining << " FRAMES LEFT]\n";
    for(int i = 0; i < livesRemaining; i++) {
        cout << "[O] ";
    }
    cout << "\n\n";
}

void CelluloidApp::playRound() {
    
    if (currentSiteIndex >= siteDatabase.size()) {
        cout << "\n\033[1;36m[SYSTEM] All records decrypted. Reshuffling the archive...\033[0m\n";
        
        // NEW: Modern C++ way to reshuffle
        random_device rd;
        mt19937 g(rd());
        shuffle(siteDatabase.begin(), siteDatabase.end(), g);
        
        currentSiteIndex = 0; 
    }

   
    HeritageSite targetSite = siteDatabase[currentSiteIndex];
    currentSiteIndex++; // Move down the deck for the next round

    string hiddenAnswer = targetSite.answer;
    string guessedState = "";

    for (char c : hiddenAnswer) {
        if (c == ' ') {
            guessedState += " ";
        } else {
            guessedState += "_";
        }
    }

    int currentLives = lives;
    int hintsUsed = 0;

    cout << "\n\033[1;33m" << targetSite.movieClue << "\033[0m\n";

    while (currentLives > 0 && guessedState != hiddenAnswer) {
        printFilmReel(currentLives);

        cout << "Target: " << guessedState << "\n";
        cout << "Enter a letter to guess: ";

       
        string input;
        char guess;
        while (true) {
            cin >> input;
            // Check if they typed exactly 1 character AND it is an alphabet letter
            if (input.length() == 1 && isalpha(input[0])) {
                guess = toupper(input[0]);
                break; // Valid input, break out of the checking loop
            } else {
                cout << "\033[1;31m [ERROR] Invalid input. Please enter a single letter (A-Z).\033[0m\n";
                cout << "Target: " << guessedState << "\n";
                cout << "Enter a letter to guess: ";
            }
        }

        bool found = false;
        for (size_t i = 0; i < hiddenAnswer.length(); i++) {
            if (hiddenAnswer[i] == guess) {
                guessedState[i] = guess;
                found = true;
            }
        }

        if (!found) {
            currentLives--;
            cout << "\a"; 
            cout << "\033[1;31m [FRAME BURNED] Incorrect guess.\033[0m\n";

            if (hintsUsed < hintsAllowed && hintsUsed < targetSite.hints.size()) {
                cout << "\033[1;36m [HINT]: " << targetSite.hints[hintsUsed] << "\033[0m\n";
                hintsUsed++;
            }
        } else {
            cout << "\033[1;32mMatch found!\033[0m\n";
        }
    }

    if (guessedState == hiddenAnswer) {
        cout << "\n\033[1;32m [DECRYPTED] The location is: " << hiddenAnswer << "\033[0m\n";
    } else {
        cout << "\n\033[1;31m [DATA LOST] The film reel burned up. Answer: " << hiddenAnswer << "\033[0m\n";
    }
}

void CelluloidApp::run() {
    cout << "\nLoading The Director's Archive...\n";
    if (!loadFiles()) return;

    char playAgain = 'Y';
    while (playAgain == 'Y' || playAgain == 'y') {
        playRound();
        cout << "\nLoad another reel? (Y/N): ";
        cin >> playAgain;
    }
}
