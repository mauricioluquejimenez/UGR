#include "letters_bag.h"

LettersBag::LettersBag() {
     srand(time(NULL));        //inicializo semilla para que cada ejecución
                               //del programa dé distintos resultados
}

LettersBag::LettersBag(const LettersSet & letterSet) {
    srand(time(NULL));

    LettersSet l = letterSet;

    for(LettersSet::iterator it = l.begin(); it != l.end(); ++it)
    {
      char letra = (*it).first;
      int repeticiones = (*it).second.repetitions;

      for(int i = 0; i < repeticiones; i++)
        insertLetter(letra);
    }
}

void LettersBag::insertLetter(const char &l) {
    letters.push_back(l);
}

char LettersBag::extractLetter() {
    if (letters.empty())
        return '\0';

    int pos = rand() % letters.size();     

    char c = letters[pos];

    letters[pos] = letters.back();  //para eleminar un elemento en medio de un vector, lo que hago es copiar
    letters.pop_back();             //el ultimo elemento del vector en la posicion que queremos eliminar y eliminamos la ultima posicion.

    return c;
}

vector<char> LettersBag::extractLetters(int num) {
    vector<char> v;

    for(int n = 0; n < num && !letters.empty(); n++)
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
