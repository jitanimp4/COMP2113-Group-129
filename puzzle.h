#ifndef PUZZLE_H
#define PUZZLE_H

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

enum class Direction {
    Across,
    Down
};

struct Clue {
    int number;
    Direction direction;
    std::string answer;
    std::string clueText;
    int row;
    int col;
};

struct DifficultyConfig {
    std::string name;
    std::string theme;
    int gridSize;
    int timeLimitSec;
    int maxHints;
    int wordBankSize;
    int basePointsPerWord;
    int wrongGuessPenalty;
    bool unlimitedHints;
    bool hintPenaltyPercent;
    int hintPenaltyValue;
    int finalMinuteBell;
    bool streakBonus;
    int streakThreshold;
    int streakBonusValue;
    int finishTimeBonusFactor;
};

struct GameState {
    std::vector<std::string> solutionGrid;
    std::vector<std::vector<int>> numbering;
    std::vector<Clue> clues;
    std::vector<std::string> wordBank;
    std::set<std::string> remainingWords;
    std::map<std::pair<int, Direction>, std::string> solvedByKey;
    int score = 0;
    int hintsUsed = 0;
    int currentStreak = 0;
};

struct RawItem {
    std::string answer;
    std::string clue;
};

DifficultyConfig getConfig(int choice);
std::vector<RawItem> getRawByDifficulty(int choice);
bool buildCrossword(
    int n, const std::vector<RawItem> &items, std::vector<std::string> &solutionGrid,
    std::vector<Clue> &placedClues);
std::vector<std::vector<int>> buildNumbering(const std::vector<std::string> &grid);

#endif
