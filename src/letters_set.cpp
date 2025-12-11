#include "letters_set.h"
#include <algorithm>

LettersSet::LettersSet() {}

LettersSet::LettersSet(const LettersSet &other) {
    *this = other;
}

bool LettersSet::insert(const pair<char, LetterInfo> &val) {
    pair<map<char,LetterInfo>::iterator, bool> i = letters.insert(val);

    return i.second;
}

bool LettersSet::erase(const char & key){
    return letters.erase(key) == 1;
}

bool LettersSet::empty() const {
    return letters.empty();
}

void LettersSet::clear(){
    letters.clear();
}

unsigned int LettersSet::size() {
    return letters.size();
}

int LettersSet::getScore(string word){
    int puntos = 0;

    transform(word.begin(),word.end(),word.begin(),[](unsigned char c){
        return tolower(c);
    });

    for(int i = 0; i < word.length(); i++)
        puntos += letters.find(word[i])->second.score;

    return puntos;
}

LettersSet & LettersSet::operator = (const LettersSet & cl){
    this->letters = cl.letters;

    return *this;
}

LetterInfo & LettersSet::operator[](const char &val) {
    return letters[val];
}

ostream & operator << (ostream & os, const LettersSet & cl){
    map<char,LetterInfo>::const_iterator i = cl.letters.begin();

    while(i != cl.letters.end()){
        cout << i->first << ": "
             << i->second.repetitions << " repeticiones, "
             << i->second.score << " puntos" << endl;
        i++;
    }

    return os;
}

istream & operator >> (istream & is, LettersSet & cl){
    char letra;
    LetterInfo info;
    pair<char,LetterInfo> par;

    string cad;
    getline(is, cad);

    while(!is.eof()){
        is >> letra >> info.repetitions >> info.score;
        par.first = letra;
        par.second = info;
        cl.letters.insert(par);
    }

    return is;
}
