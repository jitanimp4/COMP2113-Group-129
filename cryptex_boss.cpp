#include "cryptex_boss.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

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
        cout << "Error opening question file." << endl;
        return questions;
    }

    string line;
    while (getline(fin, line))
    {
        stringstream ss(line);
        TriviaQuestion q;

        getline(ss, q.difficulty, '|');
        getline(ss, q.question, '|');
        getline(ss, q.answer, '|');
        getline(ss, q.hint, '|');

        if (q.difficulty == mode)
        {
            questions.push_back(q);
        }
    }

    fin.close();
    return questions;
}

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
    for (int i = questions.size() - 1; i > 0; i--)
    {
        int r = rand() % (i + 1);

        TriviaQuestion temp = questions[i];
        questions[i] = questions[r];
        questions[r] = temp;
    }
}

void runCryptexBoss(string mode)
{
    vector<TriviaQuestion> questions = loadQuestions("cryptex_questions.txt", mode);

    if (questions.size() == 0)
    {
        cout << "No questions found for this mode." << endl;
        return;
    }
    srand(time(0));
    shuffleQuestions(questions);
    time_t startTime = time(0);
    int timeLimit = 60; // seconds

    int lives = 3;
    int score = 0;
    int totalQuestions = 10;

    if (mode == "easy")
    {
        lives = 5;
        timeLimit = 90;
    }
    else if (mode == "medium")
    {
        lives = 3;
        timeLimit = 60;
    }
    else
    {
        lives = 2;
        timeLimit = 30;
    }

    if (questions.size() < totalQuestions)
    {
        totalQuestions = questions.size();
    }

    cout << "===== THE CURATOR'S CRYPTEX =====" << endl;
    cout << "Mode: " << mode << endl;
    cout << "You have " << lives << " lives." << endl;
    cout << "Answer " << totalQuestions << " questions." << endl << endl;

    for (int i = 0; i < totalQuestions && lives > 0; i++)
    {
        time_t now = time(0);

        if (now - startTime >= timeLimit)
        {
            cout << "Time is up!" << endl;
            break;
        }

        int remaining = timeLimit - (now - startTime);

        cout << "Time left: " << remaining << " seconds" << endl;
        cout << "Question " << i + 1 << ": " << questions[i].question << endl;
        cout << "Your answer: ";

        string userAnswer;
        getline(cin, userAnswer);

        time_t afterAnswer = time(0);
        if (afterAnswer - startTime >= timeLimit)
        {
            cout << "Time is up!" << endl;
            break;
        }


        if (toLowerCase(userAnswer) == "hint")
        {
            cout << "Hint: " << questions[i].hint << endl;
            cout << "Now enter your answer: ";
            getline(cin, userAnswer);
        }

        if (toLowerCase(userAnswer) == toLowerCase(questions[i].answer))
        {
            cout << "Correct!" << endl;
            score++;
        }
        else
        {
            lives--;
            cout << "Wrong!" << endl;
            cout << "Correct answer: " << questions[i].answer << endl;
            cout << "Lives left: " << lives << endl;
        }


        cout << endl;
    }

    cout << "===== GAME OVER =====" << endl;
    cout << "Score: " << score << "/" << totalQuestions << endl;

    if (lives > 0)
    {
        cout << "You survived the Cryptex!" << endl;
    }
    else
    {
        cout << "The Cryptex defeated you." << endl;
    }
}
