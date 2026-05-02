# COMP2113-Group-129
# Odyssey OS: The Heritage Terminal

Group Members: 
1. Menaka Kalpally Pulapra Menon
2. Anushka Jitani
3. Krislyn Mariah Mendonca
4. Anshika Mittal
5. Arpita Sharma
6. Miracle Ukazu Oluebube

Contributions: 

App 1: Menaka Kalpally Pulapra Menon
App 2: Anushka Jitani
App 3: Krislyn Mariah Mendonca
App 4: Anshika Mittal
App 5: Arpita Sharma
App 6: Miracle Ukazu Oluebube


Overview:

Odyssey OS: The Heritage Terminal is a vintage-style interactive terminal game that simulates an old-school computer system. The player logs into "Odyssey OS," a simulated classified database, to decrypt corrupted files related to world heritage sites, history, art, and architecture. The player will need to complete various mini-games, each focused on different themes like ancient civilizations, movies, art history, and more.

## Table of Contents
- [Game Concept](#game-concept)
- [Mini-Games Overview](#mini-games-overview)
  - [App 1: Word Pyramids of Giza](#app-1-word-pyramids-of-giza)
  - [App 2: The Director's Archive](#app-2-the-directors-archive)
  - [App 3: The Time-Slip Syndicate](#app-3-the-time-slip-syndicate)
  - [App 4: Crossword](#app-5-crossword)
  - [App 5: The Forger's Table](#app-6-the-forgers-table)
  - [App 6: The Curator's Cryptex](#app-4-the-curators-cryptex)]
- [Technologies Used](#technologies-used)
- [Installation](#installation)
  

## Game Concept

The player interacts with **Odyssey OS**, a system housing encrypted data of historical and cultural artifacts. Each **mini-game app** challenges the player’s knowledge of history, architecture, mythology through a series of immersive puzzles. Players must complete these games to eventually unlock the final trial, **The Curator's Cryptex**.

## Mini-Games Overview

## App 1: Word Pyramids of Giza
### Overview
A terminal-based word pyramid puzzle where letters are arranged in a triangular formation. Players must identify and guess all hidden words using only the letters shown in the pyramid. The game features multiple difficulty levels with different historical and mythological categories, a hint system, a clear/reset function, and file-based word banks.

### Coding Elements Implementation

| Requirement | Implementation Location |
|-------------|------------------------|
| **Generation of random events** | `WordPyramidGame()` constructor seeds `mt19937` with `random_device`; `generatePuzzlesFromWordBank()` randomly selects word subsets; `initialize()` randomly picks a puzzle from difficulty bank; `processHint()` randomly chooses an unguessed target word; `std::shuffle` randomises letter order in pyramid (all in `word_pyramid.cpp`) |
| **Data structures for storing data** | `struct PuzzleState` in `word_pyramid.h` (holds `vector<string> targetWords`, `vector<bool> correctlyGuessed`, `vector<string> guessedWords`, `vector<char>` letter pools, `int` metadata); `vector<vector<string>> easyPuzzles`, `mediumPuzzles`, `hardPuzzles` in `WordPyramidGame` class |
| **Dynamic memory management** | `activePuzzle = new PuzzleState()` in `initialize()` (`word_pyramid.cpp`); `delete activePuzzle` in `~WordPyramidGame()` destructor; `WordPyramidGame* game = new WordPyramidGame()` in `runWordPyramidGame()`; `delete game` after each round |
| **File input/output** | `loadWordBank()` reads `easy_words.txt`, `medium_words.txt`, `hard_words.txt` (input); `logGameResult()` appends game outcomes (WIN/EXIT, targets, guesses, hints left) to `game_log.txt` (output) – both in `word_pyramid.cpp` |
| **Program codes in multiple files** | `word_pyramid.h`, `word_pyramid.cpp`, `main.cpp`, `Makefile` |
| **Multiple Difficulty Levels** | `buildPuzzleSets()` creates three separate puzzle banks; user selects 1 (Easy), 2 (Medium), or 3 (Hard) in `runWordPyramidGame()`; `initialize(int difficultyChoice)` chooses the appropriate bank; `currentDifficulty` changes displayed category – all in `word_pyramid.cpp` |

### Difficulty Levels

| Difficulty | Word Category | Words per Puzzle | Total Letters | Hints |
|------------|---------------|------------------|---------------|-------|
| Easy (1) | Gods, Monsters, and Myths (Greek, Roman, Egyptian, Hindu, Nordic) | 4–7 | 40–60 | 3 |
| Medium (2) | Empires, Innovation, Monuments | 4–7 | 40–60 | 3 |
| Hard (3) | Rulers, Pharaohs, and Heroes (Greek, Roman, Egyptian, Nordic) | 4–7 | 40–60 | 3 |

### Commands

| Command | Description |
|---------|-------------|
| `[word]` | Guess a word (e.g., `FOX`). Must use only letters currently in the pyramid |
| `clear` | Resets the letter pool and guessed words – restarts the current puzzle |
| `hint` | Reveals one random unguessed target word (max 3 hints per puzzle) |
| `exit` | Returns to the main difficulty selection menu |

### How to Play

1. Run `./word_pyramid` and select a difficulty level (1=Easy, 2=Medium, 3=Hard)
2. Read the category and the number of hidden words
3. Look at the letter pyramid – all available letters are shown
4. Type a word that can be formed from those letters
5. If correct, letters are removed and the word appears in the “Guessed words” list
6. Use `clear` to restart the puzzle if stuck
7. Use `hint` for a random revealed word (max 3 hints)
8. Guess all hidden words to win and escape the pyramid
9. Type `exit` to go back and change difficulty or quit

### App 2: The Director's Archive
**Theme:** Movies and Heritage Sites  
**Game Mechanics:**
- A Hollywood-style Hangman game where the player guesses letters based on famous movie quotes or scenes.
- Each incorrect guess burns a frame of the film reel, decreasing the player’s lives.
- Dynamic hints are provided as lives are lost.
- Difficulty: Easy, Medium, Hard.

## App 3: The Time-Slip Syndicate

### Theme
Architecture & History - A Wordle-style game where players decode 5-letter historical and architectural terms in a race against time.

### Core Gameplay
- 6 attempts to guess a 5-letter historical/architectural word
- Color-coded feedback system
- 6x5 grid with persistent guess history
- Timer system that limits each guess based on difficulty
- Hint system available in Easy mode

### Difficulty Levels

| Difficulty | Time Limit | Word Bank Size | Hints |
|------------|------------|----------------|-------|
| **EASY** | 30 seconds | 10 common words | Yes (after 3 guesses) |
| **MEDIUM** | 20 seconds | 10 mixed words | No |
| **HARD** | 10 seconds | 10 obscure terms | No |

### Word Banks by Difficulty

**EASY Mode Words:** RUINS, MAYA, ARCH, DOME, WALL, TOWER, TEMPLE, STONE, BRICK, PILLAR

**MEDIUM Mode Words:** TOMBS, AZTEC, STOAS, PYLON, OBELI, MAYAN, INCA, FORUM, COLUMN, VAULT

**HARD Mode Words:** ZIGG, STELA, LINTL, OGIVE, TRABE, DORIC, IONIC, CORIN, CARYA, ROSET

### Controls

| Action | Input |
|--------|-------|
| Type your guess | Any 5 letters A-Z |
| Submit guess | `ENTER` |
| Exit game | Type `exit` or `quit` |

### Visual Feedback Guide

After each guess, the game shows visual indicators for each letter:

| Symbol | Meaning | What To Do |
|--------|---------|-------------|
| `[R]` | **GREEN** - Correct letter, right position | Keep this letter in this position |
| `(U)` | **YELLOW** - Correct letter, wrong position | Use this letter elsewhere in your next guess |
| `{N}` | **GRAY** - Letter not in the word | Don't use this letter again |
| `_` | Empty cell (not yet guessed) | Fill with your next guess |

### How to Play

1. **Start the Game:** Run the program and select difficulty from the main menu (1-3)
2. **View Instructions:** Press 4 from main menu to see detailed instructions
3. **Make a Guess:** Type any 5-letter word and press `ENTER`
4. **Analyze Feedback:** Look at the symbols to see which letters are correct:
   - `[Letter]` = Perfect! Keep it there
   - `(Letter)` = Good letter, wrong spot - try moving it
   - `{Letter}` = Wrong letter - avoid using it again
5. **Use Hints (Easy Mode Only):** After 3 wrong guesses, the game reveals all letters in the target word
6. **Beat the Timer:** Each guess has a time limit - if time expires, you lose that attempt
7. **Win or Lose:** 
   - **WIN** by guessing the word within 6 attempts
   - **LOSE** if you use all 6 attempts or run out of time



## App 4: Crossword
**Theme:** Wonders & Landmarks, Art Movements, Iconoclasts  

### Overview
A terminal-based crossword puzzle game where players solve clues to uncover words from a word bank. The game features multiple difficulty levels, time limits, hint systems, scoring mechanics, and save/load functionality.

### Coding Elements Implementation

| Requirement | Implementation Location |
|-------------|------------------------|
| **Generation of random events** | `revealOneLetter()` in `game.cpp` - randomly selects unsolved clues and random letter positions |
| **Data structures for storing data** | `struct GameState`, `struct Clue`, `struct DifficultyConfig` in `puzzle.h` - uses vectors, sets, maps |
| **Dynamic memory management** | `std::unique_ptr<GameState>` in `game.h` - automatic memory management |
| **File input/output** | `saveGame()` and `loadGame()` in `game.cpp` - saves/loads game state to `save_data.txt` |
| **Program codes in multiple files** | `main.cpp`, `game.cpp`, `game.h`, `puzzle.cpp`, `puzzle.h`, `art.h` |
| **Multiple Difficulty Levels** | `getConfig()` in `puzzle.cpp` - Easy, Medium, Hard with different settings |

### Difficulty Levels

| Difficulty | Grid Size | Time Limit | Max Hints | Base Points |
|------------|-----------|------------|-----------|-------------|
| Easy | 7x7 | None | Unlimited | 100 |
| Medium | 9x9 | 10 minutes | 3 | 150 |
| Hard | 11x11 | 7 minutes | 1 | 220 |

### How to Play

1. Run the program and select a difficulty level (1=Easy, 2=Medium, 3=Hard)
2. View the clues (Across and Down) and the word bank
3. Use the `solve` command to guess a word for a clue : Enter clue number, direction (A/D), and word from bank
4. Use `hint` to reveal a random letter (costs points on higher difficulties) : Reveals one random letter from an unsolved clue
5. Use `clues` to view clues again 
6. Use `save` to save progress
7. Use `quit` to save and exit

### Scoring System

| Action | Points |
|--------|--------|
| Correct word | +100 (Easy), +150 (Medium), +220 (Hard) |
| Wrong guess | 0 (Easy), -10 (Medium), -25 (Hard) |
| Streak bonus (Medium) | +2 points every 7 correct words |
| Time bonus (Medium/Hard) | Points per second remaining |
| Hint penalty (Medium) | 7% reduction per hint (max 90%) |
| Hint penalty (Hard) | -150 points per hint |


### App 5: The Forger's Table
**Theme:** Heritage Crimes Investigation  
**Game Mechanics:**
- The player identifies falsified fields in artifact dossiers by comparing two records of the same artifact.
- Difficulty: Intern (Easy), Archivist (Medium), Master Curator (Hard).
- Multiple fields may be falsified, and players need to spot them under time pressure.

### App 6: The Curator's Cryptex
**Theme:** Pop-Quiz Gauntlet (Rapid Fire Trivia)  
**Game Mechanics:**
- A rapid-fire trivia challenge that tests knowledge of history, monuments, architecture, and more.
- The player answers 10 questions with a time limit and only 3 lives.
- Each incorrect answer removes one life.
- Difficulty: Easy (Training Curator), Medium (Senior Curator), Hard (Master Curator).

## Technologies Used

- **C++ 11** for core game logic and terminal-based UI.
- **ASCII Art** for the retro terminal aesthetic.
- **ANSI Escape Codes** for color and text effects.
- **Standard libraries:** `<iostream>`, `<unistd.h>` for delays, and `usleep()` for animations.

## Installation

1. Clone this repository to your local machine:
   ```bash
   git clone https://github.com/your-username/odyssey-os.git
