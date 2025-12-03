//
// Created by 
//

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <iostream>
#include <set>

#include <vector>
using namespace std;

class Dictionary {
  private:
    set<string> words;

  public:
    Dictionary();
    ~Dictionary();
    void clear();
    unsigned int size() const;
    bool empty() const;
    bool exists(const string &val);
    bool erase(const string &val);
    friend istream &operator>>(istream &is, Dictionary &dic);
    friend ostream &operator<<(ostream &os, const Dictionary &dic);
    int getOcurrences(const char c) const;
    int getTotalLetters() const;
    vector<string> getWordsLength(int length);

    class iterator
    {
      private:
        set<string>::iterator it;

      public:
        friend class Dictionary;
        iterator();
        bool operator==(const iterator &i) const;
        bool operator!=(const iterator &i) const;
        iterator &operator++();
        const string &operator*();
    };

    class const_iterator
    {
      private:
        set<string>::const_iterator it;

      public:
        friend class Dictionary;
        const_iterator();
        bool operator==(const const_iterator &i) const;
        bool operator!=(const const_iterator &i) const;
        const_iterator &operator++();
        const string &operator*() const;
    };

    iterator find(const string &w);
    pair<iterator, bool> insert(const string &val);
    pair<iterator, iterator> range_prefix(const string &val);
    iterator begin();
    const_iterator cbegin() const;
    iterator end();
    const_iterator cend() const;
};
#endif //DICTIONARY_H
