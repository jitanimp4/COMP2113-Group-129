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
  - [App 4: The Curator's Cryptex](#app-4-the-curators-cryptex)
  - [App 5: Crossword](#app-5-crossword)
  - [App 6: The Forger's Table](#app-6-the-forgers-table)
- [Technologies Used](#technologies-used)
- [Installation](#installation)
  

## Game Concept

The player interacts with **Odyssey OS**, a system housing encrypted data of historical and cultural artifacts. Each **mini-game app** challenges the player’s knowledge of history, architecture, mythology through a series of immersive puzzles. Players must complete these games to eventually unlock the final trial, **The Curator's Cryptex**.

## Mini-Games Overview

### App 1: Word Pyramids of Giza
**Theme:** Ancient Civilizations, Gods, Mythology
**Game Mechanics:**
- A Word Pyramid puzzle with letters arranged in a triangular formation.
- Categories include Egyptian pharaohs, Greek and Roman gods, ancient civilizations like the Maya and Inca, etc.
- Players must find words from a letter pool to complete the pyramid.
- Difficulty: Easy, Medium, Hard with a timer for added pressure.

### App 2: The Director's Archive
**Theme:** Movies and Heritage Sites  
**Game Mechanics:**
- A Hollywood-style Hangman game where the player guesses letters based on famous movie quotes or scenes.
- Each incorrect guess burns a frame of the film reel, decreasing the player’s lives.
- Dynamic hints are provided as lives are lost.
- Difficulty: Easy, Medium, Hard.

### App 3: The Time-Slip Syndicate
**Theme:** Architecture & History  
**Game Mechanics:**
- A Wordle-style game set in a terminal, where the player guesses 5-letter historical or architectural words and terminology.
- Players must deduce the correct term through color-coded feedback (green, yellow, gray) to identify correct and incorrect letters.
- Difficulty: Easy, Medium, Hard with a timer and era indicators.
  

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
