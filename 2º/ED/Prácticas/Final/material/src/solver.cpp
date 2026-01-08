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

    int frec_letters[26] = {0};

    for(int i = 0; i < available_letters.size(); i++)
    {
        char c = tolower(available_letters[i]);

        if(c >= 'a' && c <= 'z') frec_letters[c - 'a']++;
    }

    for (Dictionary::const_iterator it = dictionary.cbegin(); it != dictionary.cend(); ++it)
    {
        const string &word = *it;
        int frec_word[26];
        bool puede_formar = true;

        for(int i = 0; i < 26; i++) frec_word[i] = frec_letters[i];

        for(int i = 0; i < word.size(); i++)
        {
            char c = tolower(word[i]);

            if(c >= 'a' && c <= 'z')
            {
                frec_word[c - 'a']--;
                if(frec_word[c - 'a'] < 0)
                {
                    puede_formar = false;
                    break;
                }
            }
            else
            {
                puede_formar = false;
                break;
            }
        }

        if(!puede_formar) continue;
        
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