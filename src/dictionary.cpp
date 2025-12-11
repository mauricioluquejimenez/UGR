#include "dictionary.h"

void Dictionary::clear()
{
    this->words.clear();
}

unsigned int Dictionary::size() const
{
    return this->words.size();
}

bool Dictionary::exists(const string &val)
{
    return this->words.find(val) != this->words.end();
}

bool Dictionary::erase(const string &val)
{
    return this->words.erase(val) > 0;
}

istream &operator>>(istream &is, Dictionary &dic)
{
    string word;
    
    while(is >> word) dic.insert(word);
    
    return is;
}

ostream &operator<<(ostream &os, const Dictionary &dic)
{
    for(Dictionary::const_iterator it = dic.cbegin(); it != dic.cend(); ++it)
        os << *it << endl;
    
    return os;
}

int Dictionary::getOcurrences(const char c) const
{
    int ocurrences = 0;
    string word;

    for(Dictionary::const_iterator it = cbegin(); it != cend(); ++it)
    {
        word = *it;
        for(int i = 0; i < word.size(); i++ )
            if(tolower(word[i]) == tolower(c)) ocurrences++;
    }

    return ocurrences;
}

int Dictionary::getTotalLetters() const
{
    int total = 0;
    string word;

    for(Dictionary::const_iterator it = cbegin(); it != cend(); ++it)
    {
        word = *it;
        int word_size = word.size();
        total += word_size;
    }

    return total;
}

vector<string> Dictionary::getWordsLength(int longitud)
{
    vector<string> words_length;
    string word;
    int word_length;

    for(Dictionary::const_iterator it = cbegin(); it != cend(); ++it)
    {
        word = *it;
        word_length = word.size();
        if(word_length == longitud) words_length.push_back(word);
    }

    return words_length;
}

Dictionary::iterator::iterator(){};

bool Dictionary::iterator::operator==(const iterator &i) const
{
    return this->it == i.it;
}

bool Dictionary::iterator::operator!=(const iterator &i) const
{
    return this->it != i.it;
}

Dictionary::iterator &Dictionary::iterator::operator++()
{
    ++this->it;
    return *this;
}

const string &Dictionary::iterator::operator*()
{
    return *this->it;
}

Dictionary::const_iterator::const_iterator(){};

bool Dictionary::const_iterator::operator==(const const_iterator &i) const
{
    return this->it == i.it;
}

bool Dictionary::const_iterator::operator!=(const const_iterator &i) const
{
    return this->it != i.it;
}

Dictionary::const_iterator &Dictionary::const_iterator::operator++()
{
    ++this->it;
    return *this;
}

const string &Dictionary::const_iterator::operator*() const
{
    return *this->it;
}

Dictionary::iterator Dictionary::find(const string &w)
{
    Dictionary::iterator it;
    it.it = this->words.find(w);
    return it;
}

pair<Dictionary::iterator, bool> Dictionary::insert(const string &val)
{
    pair<set<string>::iterator, bool> insercion = words.insert(val);

    Dictionary::iterator it;
    it.it = insercion.first;
    
    return make_pair(it, insercion.second);
}

pair<Dictionary::iterator, Dictionary::iterator> Dictionary::range_prefix(const string &prefix)
{
    Dictionary::iterator it1, it2;

    set<string>::iterator inf = words.lower_bound(prefix);

    if(inf == words.end() || inf->compare(0, prefix.size(), prefix) != 0)
    {
        it1.it = words.end();
        it2.it = words.end();
    }
    else
    {
        string sup = prefix;
        sup.push_back(char(127));
    
        set<string>::iterator sup_it = words.upper_bound(sup);
        it1.it = inf;
        it2.it = sup_it;
    }

    return make_pair(it1, it2);
}

Dictionary::iterator Dictionary::begin()
{
    Dictionary::iterator it;
    it.it = this->words.begin();
    return it;
}

Dictionary::const_iterator Dictionary::cbegin() const
{
    Dictionary::const_iterator it;
    it.it = this->words.begin();
    return it;
}

Dictionary::iterator Dictionary::end()
{
    Dictionary::iterator it;
    it.it = this->words.end();
    return it;
}

Dictionary::const_iterator Dictionary::cend() const
{
    Dictionary::const_iterator it;
    it.it = this->words.end();
    return it;
}