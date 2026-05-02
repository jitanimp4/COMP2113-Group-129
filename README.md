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

### App 1: Word Pyramids of Giza
**Theme:** Ancient Ruins & Myths
**Game Mechanics:**
- Word Pyramids of Giza is a puzzle mini‑game set in ancient Egypt. The player sees a triangular pyramid of scrambled letters hiding 4–7 historical or mythological words (e.g., gods, empires, rulers). By typing valid words from the letter pool, the letters are removed from the pyramid. The goal is to uncover all hidden words before the timer runs out (optional). The game includes a clear reset and a hint system (max 3 hints).
- Categories include Easy) Gods, Monsters, and Myths, (Medium) Empires, Innovation, Monuments, (Hard) Rulers, Pharaohs, and Heros.
- Players must find words from a letter pool to complete the pyramid.
- Difficulty: Easy, Medium, Hard.

### App 2: The Director's Archive
**Theme:** Movies and Heritage Sites  
**Game Mechanics:**
- A Hollywood-style Hangman game where the player guesses letters based on famous movie quotes or scenes.
- Each incorrect guess burns a frame of the film reel, decreasing the player’s lives.
- Dynamic hints are provided as lives are lost.
- Difficulty: Easy, Medium, Hard.

### App 3: The Time-Slip Syndicate
**Developer:** Krislyn Mariah Mendonca
### Theme
Architecture & History - A Wordle-style game where players decode 5-letter historical and architectural terms.
### Core Gameplay
- 6 attempts to guess a 5-letter historical/architectural word
- Color-coded feedback: Green (correct position), Yellow (wrong position), Gray (not in word)
- 6x5 grid with persistent guess history
### Difficulty Levels
| Difficulty | Time Limit | Word Bank | Hints |
|------------|------------|-----------|-------|
| EASY | 30s | 10 common words (RUINS, MAYA, ARCH, etc.) | Yes |
| MEDIUM | 20s | 10 mixed words (TOMBS, AZTEC, PYLON, etc.) | Limited |
| HARD | 10s | 10 obscure terms (ZIGG, STELA, OGIVE, etc.) | None |
### Special Features
- Immersive terminal UI with era indicators (Classical/Medieval/Modern)
- Real-time countdown timer with visual progress bar
- ASCII art borders and flickering scan line effects
- Thematic success/failure messages
- ESC key to exit at any time
### Controls
- Type 5-letter word → Press `ENTER` to submit
- `BACKSPACE` to delete → `ESC` to exit
## Technologies Used
- **C++11** for core game logic
- **ASCII Art** for retro terminal aesthetic
- **ANSI Escape Codes** for color (Linux/macOS)
- **Windows Console API** for color (Windows)
### Standard Libraries Used
`<iostream>`, `<vector>`, `<map>`, `<chrono>`, `<thread>`, `<ctime>`, `<cctype>`, `<iomanip>`, `<termios.h>`, `<unistd.h>`, `<conio.h>`, `<windows.h>`
### Non-Standard Libraries
**None.** All libraries are standard C++ libraries requiring no additional installation.

### App 4: Crossword
**Theme:** Wonders & Landmarks, Art Movements, Iconoclasts  
**Game Mechanics:**
- A crossword puzzle with a word bank where players match clues to words from the bank.
- Difficulty: Easy (Famous landmarks), Medium (Art movements), Hard (Iconoclasts and architects).
- Time limits and hints are available, with the challenge scaling in difficulty.

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
