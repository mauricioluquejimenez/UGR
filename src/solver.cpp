#include "solver.h"

Solver::Solver(const Dictionary &dict, const LettersSet &letters_set)
{
    this->dictionary = dict;
    this->letters_set = letters_set;
}

pair<vector<string>, int> Solver::getSolutions(const vector<char> &available_letters, bool score_game)
{
    int score = 0, curr_score;
    vector<string> solutions;
    vector<string>::const_iterator i = (dictionary.getWords(available_letters)).cbegin();

    while(i != (dictionary.getWords(available_letters)).cend())
    {
        if(score_game) curr_score = letters_set.getScore(*i);
        else curr_score = (*i).size();

        if(curr_score == score) solutions.push_back(*i);
        else if(curr_score > score)
        {
            solutions.clear();
            solutions.push_back(*i);
            score = curr_score;
        }
        i++;
    }

    pair<vector<string>, int> solution = {solutions, score};
    return solution;
}

pair<vector<string>, int> Solver::getSolutionsE(const vector<char> &available_letters, bool score_game)
{
    int score = 0;
    vector<string> mejores_resultados;
    vector<string> results = this->dictionary.getWords(available_letters);

    if (score_game)
    {
        for (vector<string>::iterator it = results.begin(); it != results.end(); ++it)
        {
            string word = *it;
            int puntuacion_actual = this->letters_set.getScore(word);
            if (puntuacion_actual == score) mejores_resultados.push_back(word);
            else if (puntuacion_actual > score)
            {
                mejores_resultados.clear();
                score = puntuacion_actual;
                mejores_resultados.push_back(word);
            }
        }
    }
    else
    {
        for (vector<string>::iterator it = results.begin(); it != results.end(); ++it)
        {
            string word = *it;
            int puntuacion_actual = word.size();
            if (puntuacion_actual == score)  mejores_resultados.push_back(word);
            else if (puntuacion_actual > score)
            {
                mejores_resultados.clear();
                score = puntuacion_actual;
                mejores_resultados.push_back(word);
            }
        }
    }

    return pair<vector<string>, int> (mejores_resultados, score);
}