#include "letters_bag.h"

LettersBag::LettersBag(const LettersSet & lettersSet) {
    *this = lettersSet;
}
void LettersBag::insertLetter(const char &l) {
    letters.add(l);
}
char LettersBag::extractLetter() {
    char letter = letters.get();

    return letter;
}
vector<char> LettersBag::extractLetters(int num) {
    int n = 0;
    vector<char> v;

    while(n < num){
        char letter = letters.get();
        v.push_back(letter);
        n++;
    }

    return v;
}
void LettersBag::clear() {
    letters.clear();
}
unsigned int LettersBag::size() const {
    return letters.size();
}
LettersBag & LettersBag::operator = (const LettersBag & other){
    letters = other.letters;

    return *this;
}