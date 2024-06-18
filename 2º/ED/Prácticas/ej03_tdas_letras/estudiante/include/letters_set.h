#ifndef __LETTER_SET_H__
#define __LETTER_SET_H__

/**
 * @brief TDA LetterInfo
 *
 * Contiene información sobre un determinado carácter del juego de las
 * letras. En concreto, almacena información sobre el número de repeticiones de
 * la letra en la partida y de la puntuación que otorga al utilizarse en una
 * palabra
 */

struct LetterInfo{
    int repetitions, score;
};

/**
 * @brief TDA LettersSet
 *
 * Este TDA representa un conjunto de letras, con la información necesaria para
 * jugar una partida al juego de las letras, es decir, el número de repeticiones
 * que tenemos de la letra y la puntuación que dicha letra otorga cuando se
 * utiliza en una palabra
 */

#include <iostream>
#include <map>

using namespace std;

class LettersSet{
    private:
        map<char, LetterInfo> letters;

    public:

        /**
         * @brief Constructor por defecto. Crea un LetterSet vacío
         */
        LettersSet()= default;

        /**
         * @brief Constructor de copia
         * @param other LetterSet a copiar
         */
        LettersSet(const LettersSet & other);

        /**
         * @brief Inserta un elemento en el LetterSet
         * @param val Pareja de letra y LetterInfo asociada a insertar
         * @return Booleano que marca si se ha podido insertar la letra en el LettersSet. La letra sólo se inserta correctamente si no estaba aún incluida en la colección 
         */
        bool insert(const pair<char,LetterInfo> & val);

        /**
         * @brief Elimina un carácter del LetterSet
         * @param key Carácter a eliminar
         * @return Booleano que indica si se ha podido eliminar correctamente la letra del LettersSet 
         */
        bool erase(const char & key);

        /**
         * @brief Consulta si el LettersSet es vacío
         * @return true si el LetterSet está vacío, falso en caso contrario 
         */
        bool empty() const;

        /**
         * @brief Limpia el contenido del LettersSet. Elimina el contenido del LettersSet
         */
        void clear();

        /**
         * @brief Tamaño del LetterSet
         * @return Número de elementos en el LetterSet 
         */
        unsigned int size() const;

        /**
         * @brief Calcula la puntuación dada una palabra
         * @param word String con la palabra cuya puntuación queremos calcular
         * @return Puntuación de la palabra, calculada como la suma de las puntuaciones de cada una de sus letras 
         */
        int getScore(string word);

        /**
         * @brief Sobrecarga del operador de asignación
         * @param cl LetterSet a copiar
         * @return Referencia al objeto this para poder encadenar el operador 
         */
        LettersSet & operator = (const LettersSet & cl);

        /**
         * @brief Sobrecarga del operador de consulta. Permite acceder a los elementos del map que hay en nuestra clase
         * @param val Carácter a consultar
         * @return Estructura de tipo LetterInfo con la información del carácter consultado. Número de repeticiones y puntuación 
         */
        LetterInfo & operator [] (const char & val);


        /**
         * @brief Sobrecarga del operador de salida
         * @param os Flujo de salida, donde escribir el LettersSet
         * @param cl LettersSet que se escribe
         * @return Flujo de salida
         */
        friend ostream & operator << (ostream & os, const LettersSet & cl);

        /**
         * @brief Sobrecarga del operador de entrada
         * @param is Flujo de entrada, del que leer el LettersSet
         * @param cl LettersSet en el que almacenar la información leída
         * @return istream& 
         */
        friend istream & operator >> (istream & is, LettersSet cl);

        class iterator{
        private:
            map<char,LetterInfo>::iterator i;
        public:
            iterator(){}
            iterator(const map<char,LetterInfo>::iterator & other): i(other){}
            iterator(const iterator & other):i(other.i){}
            ~iterator(){}
            iterator & operator = (const map<char,LetterInfo>::iterator & other){i=other; return *this;}
            iterator & operator = (const iterator & other) {i = other.i; return *this;}
            const pair<char,LetterInfo> operator * () const{return *i;}
            iterator & operator ++ () {++i; return *this;}
            iterator & operator -- () {--i; return *this;}
            iterator & operator ++ (int) {i++; return *this;}
            iterator & operator -- (int) {i--; return *this;}
            bool operator != (const iterator & other) {return i != other.i;}
            bool operator == (const iterator & other) {return i == other.i;}
        };

    class const_iterator{
    private:
        map<char,LetterInfo>::const_iterator i;
    public:
        const_iterator(){}
        const_iterator(const map<char,LetterInfo>::iterator & other): i(other){}
        const_iterator(const const_iterator & other):i(other.i){}
        ~const_iterator(){}
        const_iterator & operator = (const map<char,LetterInfo>::iterator & other){i=other; return *this;}
        const_iterator & operator = (const const_iterator & other) {i = other.i; return *this;}
        const pair<char,LetterInfo> operator * () const{return *i;}
        const_iterator & operator ++ () {++i; return *this;}
        const_iterator & operator -- () {--i; return *this;}
        const_iterator & operator ++ (int) {i++; return *this;}
        const_iterator & operator -- (int) {i--; return *this;}
        bool operator != (const const_iterator & other) {return i != other.i;}
        bool operator == (const const_iterator & other) {return i == other.i;}
    };
};

ostream & operator << (ostream & os, const LettersSet & cl);
istream & operator >> (istream & is, LettersSet cl);
#endif