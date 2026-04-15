#include "cryptex_boss.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

//Color scheme

const string RESET  = "\033[0m";
const string PINK = "\033[95m";
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE   = "\033[34m";
const string MAGENTA= "\033[35m";
const string CYAN   = "\033[36m";
const string WHITE  = "\033[37m";
const string BRIGHT_RED   = "\033[91m";
const string BRIGHT_GREEN = "\033[92m";
const string BRIGHT_YELLOW= "\033[93m";
const string BRIGHT_CYAN  = "\033[96m";

const string BOLD   = "\033[1m";

string toLowerCase(string text)
{
    for (int i = 0; i < text.length(); i++)
    {
        text[i] = tolower(text[i]);
    }
    return text;
}

vector<TriviaQuestion> loadQuestions(string filename, string mode)
{
    vector<TriviaQuestion> questions;
    ifstream fin(filename);

    if (!fin)
    {
        cout << "Error opening file: " << filename << endl;
        return questions;
    }

    string line;
    while (getline(fin, line))
    {
        stringstream ss(line);
        TriviaQuestion q;
        q.bonusLife = false;


        getline(ss, q.difficulty, '|');
        getline(ss, q.question, '|');
        getline(ss, q.answer, '|');
        getline(ss, q.hint, '|');

        if (q.question == "What famous island prison once held Al Capone?" || q.question == "Which kingdom in present-day Nigeria was known for its advanced bronze sculptures?"
                || q.question == "What fortress in India is known as the Red Fort?")
        {
            q.bonusLife = true;
        }

        if (q.difficulty == mode)
        {
            questions.push_back(q);
        }
    }

    fin.close();
    return questions;
}

//Changed Shuffle logic, this was too simple and not efficient
//void shuffleQuestions(vector<TriviaQuestion>& questions)
//{
//for (int i = 0; i < questions.size(); i++)
//{
//int r = rand() % questions.size();
//TriviaQuestion temp = questions[i];
//questions[i] = questions[r];
//questions[r] = temp;
//}
//}

void shuffleQuestions(vector<TriviaQuestion>& questions)
{
    if (questions.empty())
        return;

    for (int i = questions.size() - 1; i > 0; i--)
    {
        int r = rand() % (i + 1);

        TriviaQuestion temp = questions[i];
        questions[i] = questions[r];
        questions[r] = temp;
    }
}
//Mascot
void showMascotNormal()
{
    cout << PINK;
    cout << "   /\\_/\\\\ " << endl;
    cout << "  ( o.o )" << endl;
    cout << "   > ^ <   Curio is watching..." << endl;
    cout << RESET;
}

void showMascotAngry()
{
    cout << "   /\\_/\\\\ " << endl;
    cout << "  ( >.< )" << endl;
    cout << "   > ^ <   CURIO IS NOT PLEASED." << endl;
}

void showMascotSad()
{
    cout << "============================================" << endl;
    cout << BRIGHT_YELLOW;
    cout << "   /\\_/\\ " << endl;
    cout << "  ( T_T )" << endl;
    cout << "   > ^ <   "<< RED <<"YOU FAILED CURIO *TSK *TSK..." << RESET << endl;
    cout << "============================================" << endl;
}

void showMascotHappy()
{
    cout << "============================================" << endl;
    cout << PINK;
    cout << "   /\\_/\\ " << endl;
    cout << "  ( ^w^ )" << endl;
    cout << "   > ^ <  " << GREEN << "ARCHIVE RESTORED!" << RESET << endl;
    cout << "Curio is very happy! UwU" << endl;
    cout << "============================================" << endl;
}

void showMascotSleeping()
{
    cout << "   /\\_/\\\\ " << endl;
    cout << "  ( -.- ) zZ" << endl;
    cout << "   > ^ <   Curio is waiting..." << endl;
}

void showMascotThinking()
{
    cout << BRIGHT_CYAN;
    cout << "   /\\_/\\\\ " << endl;
    cout << "  ( o~o )" << endl;
    cout << "   > ^ <   "<< RESET << "Curio is searching the archives..." << endl;
}

void showMascotGlitch()
{
    cout << "============================================" << endl;
    cout << RED;
    cout << "   /\\\\_/\\\\ " << endl;
    cout << "  ( x.x )" << endl;
    cout << "   > ^ <   CURI0_//GL1TCH DETECTED" << endl;
    cout << RESET;
    cout << "============================================" << endl;
}

void runCryptexBoss(string mode)
{
    vector<TriviaQuestion> questions = loadQuestions("data/cryptex_questions.txt", mode); //is cryptex_questions in my codeblocks

    if (questions.size() == 0)
    {
        cout << "No questions found for this mode." << endl;
        return;
    }
    srand(time(0)); //put this in main
    shuffleQuestions(questions);
    time_t startTime = time(0);
    int timeLimit = 60; // seconds

    int lives;
    int score = 0;
    int totalQuestions = 10;
    int hintsLeft;

    if (mode == "easy")
    {
        lives = 5;
        timeLimit = 90;
        hintsLeft = 3;
    }
    else if (mode == "medium")
    {
        lives = 3;
        timeLimit = 60;
        hintsLeft = 2;
    }
    else
    {
        lives = 2;
        timeLimit = 40;
        hintsLeft = 1;
    }

    if (questions.size() < totalQuestions)
    {
        totalQuestions = questions.size();
    }

    showMascotSleeping();
    cout << BRIGHT_CYAN << "Waking Curio..." << RESET << endl;
    cout<<endl;
    cout << "==================================" << endl;
    cout << "     THE CURATOR'S CRYPTEX        " << endl;
    cout << "==================================" << endl;
    cout<<endl;
    showMascotNormal();
    cout << CYAN << "[ARCHIVE STATUS] Final archive access initiated..." << RESET << endl;
    //Color Change Modes
    if (mode == "easy")
    {
        cout << "Mode: " << GREEN << mode << RESET << endl;
    }
    else if (mode == "medium")
    {
        cout << "Mode: " << YELLOW << mode << RESET << endl;
    }
    else
    {
        cout << "Mode: " << RED << mode << RESET << endl;
    }
    if (lives == 1)
    {
        cout << "You have " << RED << lives << RESET << " life." << endl;
    }
    else
    {
        cout << "You have " << YELLOW << lives << RESET << " lives." << endl;
    }
    cout << "Answer " << totalQuestions << " questions." << endl << endl;
    cout << BLUE << "Need help? Type 'hint' for an easy cheat." << RESET << endl;

    if (mode == "hard")
    {
        cout << BRIGHT_RED << BOLD;
        cout << "==================================" << endl;
        cout << "        SUDDEN DEATH MODE         " << endl;
        cout << "==================================" << RESET << endl;

        cout << RED << "[SECURITY NOTICE]" << RESET
             << " Errors will not be forgiven." << endl;

        showMascotGlitch();
    }

    for (int i = 0; i < totalQuestions && lives > 0; i++)
    {
        time_t now = time(0);

        if (now - startTime >= timeLimit)
        {
            cout << "[SYSTEM FAILURE] Time is UP!" << endl;
            showMascotSad();
            break;
        }

        int remaining = timeLimit - (now - startTime);

        if( remaining > 10 )
        {
            if (remaining > 20)
            {
                cout << BRIGHT_GREEN << "Time left: " << remaining << " seconds" << RESET << endl;
            }
            else
            {
                cout << YELLOW << "Time left: " << remaining << " seconds" << RESET << endl;
            }
        }
        else
        {
            cout << RED << "Time left: " << remaining << " seconds" << RESET << endl;
        }

        cout << BOLD << "> Question " << i + 1 << ": " << questions[i].question << RESET << endl;
        cout << "Your answer: ";

        string userAnswer;
        getline(cin, userAnswer);

        time_t afterAnswer = time(0);
        if (afterAnswer - startTime >= timeLimit)
        {
            cout << RED << "[SYSTEM FAILURE] Time is UP!" << RESET << endl;
            break;
        }

        if (userAnswer == "")
        {
            showMascotSleeping();
        }

        if (toLowerCase(userAnswer) == "hint")
        {
            showMascotThinking();
            if (hintsLeft > 0)
            {
                cout << CYAN << "Hint: " << questions[i].hint << RESET << endl;
                hintsLeft--;
                cout << BRIGHT_CYAN << "Hints left: " << hintsLeft << RESET << endl;
                cout << "Now enter your answer: ";
                getline(cin, userAnswer);
            }
            else
            {
                cout << RED << "No hints left." << RESET << endl;
                cout << "Enter your answer: ";
                getline(cin, userAnswer);
            }
        }

        if (toLowerCase(userAnswer) == toLowerCase(questions[i].answer))
        {
            cout << endl;
            cout << GREEN << "[ACCESS STABLE] Correct!" << RESET << endl;
            score++;

            if (questions[i].bonusLife)
            {
                lives++;
                cout << endl;
                cout << MAGENTA << "[RELIC BONUS] Extra life granted." << RESET << endl;
            }
        }
        else
        {
            lives--;
            if (lives == 1)
            {
                cout << RED << BOLD << "============================================" << endl;
                cout << "       WARNING: Final life remaining!       " << endl;
                cout << "============================================" << RESET << endl;
                showMascotAngry();
            }
            cout << RED << endl;
            cout << "[ARCHIVE BREACH!!!] Incorrect response." << endl;
            cout << RESET << endl;
            cout << "Correct answer: " << questions[i].answer << endl;
            if (lives == 1)
            {
                cout << RED << "Lives left: " << lives << RESET << endl;
            }
            else
            {
                cout << YELLOW << "Lives left: " << lives << RESET << endl;
            }
        }


        cout << endl;
    }

    cout << "===== GAME OVER =====" << endl;
    cout << "Score: " << score << "/" << totalQuestions << endl;

    if (lives > 0)
    {
        showMascotHappy();
        cout << GREEN << "You survived the Cryptex!" << endl;
    }
    else
    {
        showMascotSad();
        cout << "The Cryptex defeated you." << endl;
    }
}
