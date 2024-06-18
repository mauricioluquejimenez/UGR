//
// Created by fluque1995 on 8/9/21.
//

#include <string>
#include <vector>
#include <utility>
#include <set>
#include "dictionary.h"
#include <cmath>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
//                             Private functions                             //
///////////////////////////////////////////////////////////////////////////////

Dictionary::node Dictionary::findLowerBoundChildNode(char character, Dictionary::node current) {
  node prev_child, curr_child = current.left_child();

  for (; !curr_child.is_null() && (*curr_child).character <= character; curr_child = curr_child.right_sibling()){
    prev_child = curr_child;
    if ((*curr_child).character == character)
      return curr_child;
  }
  if (!prev_child.is_null())
    return prev_child;

  return current;
}

Dictionary::node Dictionary::insertCharacter(char character, Dictionary::node current) {
  node insertion_position = findLowerBoundChildNode(character, current);
  if (insertion_position == current){
    this->words.insert_left_child(current, char_info(character));
    return insertion_position.left_child();
  } else if ((*insertion_position).character != character){
    this->words.insert_right_sibling(insertion_position, char_info(character));
    return insertion_position.right_sibling();
  } else
    return insertion_position;
}

int Dictionary::getOccurrences(node curr_node, char c){
    int occurrences = 0;

    if(!curr_node.left_child().is_null())
        occurrences += getOccurrences(curr_node.left_child(), c);
    if(!curr_node.right_sibling().is_null())
        occurrences += getOccurrences(curr_node.right_sibling(), c);
    if( (*curr_node).character == c)
        occurrences++;

    return occurrences;
}

std::pair<int, int> Dictionary::getTotalUsages(node curr_node, char c){
    pair<int,int> p = {0,0}, p_final;
    int usos_parciales_c = 0, validos_parciales_c = 0,
        usos_parciales_s = 0, validos_parciales_s = 0;

    if(!curr_node.left_child().is_null()){
        p = getTotalUsages(curr_node.left_child(), c);
        usos_parciales_c += p.first;
        validos_parciales_c += p.second;
    }

    if(!curr_node.right_sibling().is_null()){
        p = getTotalUsages(curr_node.right_sibling(), c);
        usos_parciales_s += p.first;
        validos_parciales_s += p.second;
    }

    int usos_totales = usos_parciales_c + usos_parciales_s;
    int validos_totales = validos_parciales_c + validos_parciales_s;

    if(*curr_node == c){
        usos_totales += validos_parciales_c;
        if((*curr_node).valid_word)
            usos_totales++;
            validos_totales++;
    }

    /*if((*curr_node).valid_word)
        validos_totales++;*/

    p_final = {usos_totales, validos_totales};

    return p_final;
}

///////////////////////////////////////////////////////////////////////////////
//                              Public functions                             //
///////////////////////////////////////////////////////////////////////////////

Dictionary::Dictionary() {
  this->words.set_root(char_info());
  this->total_words = 0;
}

Dictionary::Dictionary(const Dictionary &other) {
  this->words = other.words;
  this->total_words = other.total_words;
}

Dictionary::~Dictionary() {
  this->words.clear();
}

bool Dictionary::exists(const std::string & word) {
  node current = this->words.get_root();
  for (int i = 0; i < word.size(); ++i){
    current = this->findLowerBoundChildNode(word[i], current);
    if ((*current).character != word[i])
      return false;
  }

  return (*current).valid_word;
}

bool Dictionary::insert(const std::string &word) {
  node current = this->words.get_root();
  for (int i = 0; i < word.size(); ++i)
    current = this->insertCharacter(word[i], current);

  if (!(*current).valid_word) {
    (*current).valid_word = true;
    this->total_words++;
    return true;
  }

  return false;
}

bool Dictionary::erase(const std::string & val){
  node current = this->words.get_root();
  for (int i = 0; i < val.size(); ++i){
    current = this->findLowerBoundChildNode(val[i], current);
    if ((*current).character != val[i])
      return false;
  }
  if ((*current).valid_word){
    (*current).valid_word = false;
    this->total_words--;
    return true;
  }
  return false;
}

Dictionary &Dictionary::operator=(const Dictionary &dic){
  if (this != &dic){
    this->words.clear();
    this->words = dic.words;
    this->total_words = dic.total_words;
  }
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//                               I/O overload                                //
///////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream &os, const Dictionary &dict){
  for(Dictionary::iterator it = dict.begin();it != dict.end(); ++it)
    os << *it << std::endl;

  return os;
}

std::istream& operator>>(std::istream &is, Dictionary &dict){
  std::string curr_word;
  while (getline(is, curr_word))
    dict.insert(curr_word);

  return is;
}

vector<string> Dictionary::createWords(vector<char> & Characters, node curr_node, string word){
    vector<string> Words;

    if(!curr_node.is_null()){
        if((*curr_node).valid_word)
            Words.push_back(word);

        curr_node = curr_node.left_child();

        while(!curr_node.is_null()){
            vector<char>::iterator i = find(Characters.begin(), Characters.end(), (*curr_node).character);
            if(i != Characters.end()){
                Characters.erase(i);

                vector<string> next_word = createWords(Characters, curr_node, word + (*i));
                vector<string>::iterator nwi = next_word.begin();
                while(nwi != next_word.end()) Words.push_back(*nwi);

                Characters.push_back(*i);
            }
            curr_node = curr_node.right_sibling();
        }
    }

    return Words;
}


vector<string> Dictionary::getWords(const vector<char> & available_characters) {
    vector<char> Characters;

    vector<char>::const_iterator i = available_characters.cbegin();
    while(i != available_characters.cend()){
        char c = tolower(*i);
        Characters.push_back(c);
    }

    vector<string> Words = createWords(Characters, this->words.get_root(), "");

    return Words;
}

///////////////////////////////////////////////////////////////////////////////
//                            Recursive counters                             //
///////////////////////////////////////////////////////////////////////////////

int Dictionary::getOccurrences(const char c){
    return getOccurrences(this->words.get_root(),c);
}

int Dictionary::getTotalUsages(const char c){
    return getTotalUsages(this->words.get_root(),c).first;
}

///////////////////////////////////////////////////////////////////////////////
//                                 Iterator                                  //
///////////////////////////////////////////////////////////////////////////////

Dictionary::iterator::iterator() {
    this->curr_word = "";
    this->iter = tree<char_info>::const_preorder_iterator();
}

Dictionary::iterator::iterator(tree<char_info>::const_preorder_iterator iter) {
    this->curr_word = "";
    this->iter = iter;
}

Dictionary::iterator::iterator(iterator const & i){
    this->curr_word = i.curr_word;
    this->iter = i.iter;
}

std::string Dictionary::iterator::operator*() {
    return this->curr_word;
}

Dictionary::iterator &Dictionary::iterator::operator++() {
    int nivel;

    while((!iter.get_level() == 0) and (!(*iter).valid_word)){
        nivel = iter.get_level();

        if(nivel != 0){
            if(iter.get_level() > nivel)
                curr_word += (*iter).character;

            else if(iter.get_level() == nivel){
                curr_word.pop_back();
                curr_word += (*iter).character;
            }
            else
                for(int i = 0; i < (nivel - iter.get_level()); i++)
                    curr_word.pop_back();
        }
    }
    return *this;
}

bool Dictionary::iterator::operator==(const iterator &other) {
    return (this->iter == other.iter);
}

bool Dictionary::iterator::operator!=(const iterator &other) {
    return (this->iter != other.iter);
}

Dictionary::iterator Dictionary::begin() const {
    return words.cbegin_preorder();
}

Dictionary::iterator Dictionary::end() const {
    return words.cend_preorder();
}

///////////////////////////////////////////////////////////////////////////////
//                            Letters Iterator                               //
///////////////////////////////////////////////////////////////////////////////


Dictionary::possible_words_iterator Dictionary::possible_words_begin(vector<char> available_characters) const {
    possible_words_iterator it(this->words.get_root(), available_characters);
    ++it;
    return it;
}

Dictionary::possible_words_iterator Dictionary::possible_words_end() const {
    return possible_words_iterator();
}

Dictionary::possible_words_iterator::possible_words_iterator() {
    node n;

    current_word = "";
    current_node = n;     //Nodo vacío
}

Dictionary::possible_words_iterator::possible_words_iterator(node current_node, vector<char> available_letters){
    this->current_node = current_node;

    vector<char>::const_iterator i = available_letters.begin();
    while(i != available_letters.end()){ this->available_letters.insert(*i); i++; } //inserta el vector en el multiset
}

Dictionary::possible_words_iterator::possible_words_iterator(const possible_words_iterator &other){
    this->available_letters = other.available_letters;
    this->level = other.level;
    this->current_node = other.current_node;
    this->current_word = other.current_word;
}

Dictionary::possible_words_iterator &Dictionary::possible_words_iterator::operator=(const Dictionary::possible_words_iterator &other) {
    this->available_letters = other.available_letters;
    this->level = other.level;
    this->current_node = other.current_node;
    this->current_word = other.current_word;
}

bool Dictionary::possible_words_iterator::operator==(const Dictionary::possible_words_iterator &other) const {
    return ((this->available_letters == other.available_letters)
        and (this->level == other.level)
        and (this->current_node == other.current_node)
        and (this->current_word == other.current_word));
}

bool Dictionary::possible_words_iterator::operator!=(const Dictionary::possible_words_iterator &other) const {
    return ((this->available_letters != other.available_letters)
        and (this->level != other.level)
        and (this->current_node != other.current_node)
        and (this->current_word != other.current_word));
}

Dictionary::possible_words_iterator &Dictionary::possible_words_iterator::operator++() {
//SEGURAMENTE MAL PERO HE IDO PROBANDO COSAS POR SI COLABA
    // Verifica si la rama actual puede generar una palabra válida
    /*
    while ((*current_node).valid_word ) {

    if (!current_node.left_child().is_null()) {
        current_node = current_node.left_child();
    } else if (!current_node.right_sibling().is_null()) {
        current_node = current_node.right_sibling();
    }
    return *this;
    }*/
}

std::string Dictionary::possible_words_iterator::operator*() const {
    return this->current_word;
}