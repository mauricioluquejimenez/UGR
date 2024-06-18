#include "letters_bag.h"

LettersBag::LettersBag() {}

LettersBag::LettersBag(const LettersSet & letterSet) {

    LettersSet l = letterSet;

    for(int letra = 'A'; letra <= 'Z'; letra++){
        for(int n  = 0; n < l[letra].repetitions; n++){
            letters.add(letra);
        }
    }
}

void LettersBag::insertLetter(const char &l) {
    letters.add(l);
}

char LettersBag::extractLetter() {
    return letters.get();
}

vector<char> LettersBag::extractLetters(int num) {
    vector<char> v;

    for(int n = 0; n < num; n++)
        v.push_back(extractLetter());

    return v;
}

void LettersBag::clear() {
    letters.clear();
}

unsigned int LettersBag::size() const {
    return letters.size();
}

LettersBag & LettersBag::operator = (const LettersBag & other){
    this->letters = other.letters;

    return *this;
}