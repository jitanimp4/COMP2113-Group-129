#ifndef CRYPTEX_BOSS_H
#define CRYPTEX_BOSS_H

#include <string>
#include <vector>
using namespace std;

struct TriviaQuestion
{
    string difficulty;
    string question;
    string answer;
    string hint;
};

void runCryptexBoss(string mode);

#endif
