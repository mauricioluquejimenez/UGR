#include "letters_set.h"
#include <algorithm>

LettersSet::LettersSet(const LettersSet & other) {
    *this=other;
}
bool LettersSet::insert(const pair<char,LetterInfo> & val){
    pair<map<char,LetterInfo>::iterator,bool> i = letters.insert(val);

    return i.second;
}
bool LettersSet::erase(const char & key){
    return letters.erase(key) == 1;
}
bool LettersSet::empty() const {
    return letters.empty();
}
void LettersSet::clear() {
    letters.clear();
}
unsigned int LettersSet::size() const {
    return letters.size();
}
int LettersSet::getScore(std::string word) {
    map<char,LetterInfo>::iterator it;
    int sum_score = 0;

    transform(word.begin(),word.end(),word.begin(),::tolower);

    for(int i = 0; i < word.length(); i++){
        it = letters.find(word[i]);

        if(it->first == word[i])
            sum_score += it->second.score;
    }
    return sum_score;
}
LettersSet & LettersSet::operator = (const LettersSet & cl){
    letters = cl.letters;

    return *this;
}
LetterInfo & LettersSet::operator [] (const char & val){
    return letters[val];
}
ostream & operator << (ostream & os, const LettersSet & cl){
   map<char, LetterInfo>::const_iterator i = cl.letters.begin();

   while(i != cl.letters.end())
        cout << i->first << ": "
             << i->second.repetitions << " repeticiones, "
             << i->second.score << " puntos" << endl;

    return os;
}
istream & operator >> (istream & is, LettersSet & cl){

}