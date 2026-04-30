#ifndef GAME_H
#define GAME_H

#include "puzzle.h"

#include <chrono>
#include <memory>
#include <string>

struct SaveMeta {
    int difficultyChoice = 1;
    int elapsedSeconds = 0;
    bool hardBellPlayed = false;
};

class CrosswordGame {
public:
    explicit CrosswordGame(const std::string &savePath);
    void run();

private:
    bool tryLoadGame();
    bool startNewGame();
    void gameLoop();
    void processSolve();
    void processHint();
    void applyFinalScoring();
    int secondsRemaining() const;
    bool isSolved() const;
    const Clue *findClue(int number, Direction d) const;
    bool revealOneLetter();
    void saveCurrentGame() const;

    std::string savePath_;
    int difficultyChoice_;
    DifficultyConfig cfg_;
    std::unique_ptr<GameState> state_;
    std::chrono::steady_clock::time_point startTime_;
    int loadedElapsedSeconds_;
    bool hardBellPlayed_;
};

#endif
