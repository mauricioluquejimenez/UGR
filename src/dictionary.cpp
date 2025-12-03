#include "dictionary.h"

Dictionary::Dictionary()
{
    this->words = set<string>();
}

Dictionary::~Dictionary()
{
    clear();
}

void Dictionary::clear()
{
    this->words.clear();
}

unsigned int Dictionary::size() const
{
    return this->words.size();
}

bool Dictionary::empty() const
{
    return this->words.empty();
}

bool Dictionary::exists(const string &val)
{
    return this->words.find(val) != this->words.end();
}

bool Dictionary::erase(const string &val)
{
    return this->words.erase(val);
}

int Dictionary::getOcurrences(const char c) const
{

}

int Dictionary::getTotalLetters() const
{

}

vector<string> Dictionary::getWordsLength(int length)
{

}

Dictionary::iterator::iterator() = default;

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

Dictionary::const_iterator::const_iterator() = default;

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

}

pair<Dictionary::iterator, bool> Dictionary::insert(const string &val)
{

}

pair<Dictionary::iterator, Dictionary::iterator> Dictionary::range_prefix(const string &prefix)
{

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