#include "solver.h"

Solver::Solver(const Dictionary &dict, const LettersSet &letters_set)
{
    this->dictionary = dict;
    this->letters_set = letters_set;
}

pair<vector<string>, int> Solver::getSolutions(const vector<char> &available_letters, bool score_game)
{
    vector<string> solutions;
    int score = -1, curr_score = 0;

    for (Dictionary::const_iterator it = dictionary.cbegin(); it != dictionary.cend(); ++it)
    {
        const string &word = *it;

        if(score_game) curr_score = letters_set.getScore(word);
        else curr_score = (word).size();

        if(curr_score == score) solutions.push_back(word);
        else if(curr_score > score)
        {
            solutions.clear();
            solutions.push_back(word);
            score = curr_score;
        }
    }    
    return make_pair(solutions, score);
}