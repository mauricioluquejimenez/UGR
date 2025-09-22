#include "dictionary.h"

Dictionary::Dictionary(const Dictionary &other) {
    *this = other;
}
bool Dictionary::exists(const string &val) const {
    set<string>::const_iterator i = words.begin();
    bool exists = false;

    while(i != words.end() and !exists)
        exists = (*i == val);

    return exists;
}
bool Dictionary::insert(const std::string &val) {
    pair<set<string>::const_iterator,bool> i = words.insert(val);

    return i.second;
}
bool Dictionary::erase(const std::string &val) {
    return words.erase(val) == 1;
}
void Dictionary::clear() {
    words.clear();
}
bool Dictionary::empty() const {
    return words.empty();
}
unsigned int Dictionary::size() const {
    return words.size();
}
int Dictionary::getOcurrences(const char c) {
    int ocurrences = 0;
    set<string>::const_iterator i = words.begin();

    while(i != words.end()){
        string s = *i;
        for(int j = 0; j < s.size(); j++)
            if(s[j] == c) ocurrences++;
    }

    return ocurrences;
}
int Dictionary::getTotalLetters() {
    int letras = 0;
    set<string>::const_iterator i = words.begin();

    while(i != words.end())
        letras += i->size();

    return letras;
}
vector<string> Dictionary::wordsOfLength(int length) {
    vector<string> v;
    set<string>::const_iterator i = words.begin();

    while(i != words.end()){
        if(i->size() == length)
            v.push_back(*i);
        i++;
    }
}

ostream & operator << (ostream & os, const Dictionary & dic){
    set<string>::const_iterator i = dic.words.begin();
    
    while(i != dic.words.end())
        cout << *i << endl;

    return os;
}