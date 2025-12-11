#ifndef __LETTER_SET_H__
#define __LETTER_SET_H__

#include <iostream>
#include <map>

using namespace std;

/**
 * @brief TDA LetterInfo. Estructura que almacena la información de cada carácter en el LettersSet.
 */
struct LetterInfo{

    /**
     * @brief Número de veces que un caracter puede repetirse en la partida
     */
    unsigned int repetitions;

    /**
     * @brief Puntuación asociada al caracter
     */
    unsigned int score;

  /**
   * @brief Constructor por defecto
   */
  LetterInfo(): repetitions(0), score(0){}

  /**
   * @brief Constructor con parámetros
   * @param reps Número de repeticiones del carácter en la partida
   * @param score Puntuación del carácter
   */
  LetterInfo(unsigned int reps, unsigned int score): repetitions(reps), score(score){};
};

/**
 * @brief TDA LettersSet. Almacena el conjunto de letras disponibles en el juego de las letras, junto con su puntuación y número de repeticiones.
 */
class LettersSet{
private:
  map<char, LetterInfo> letters;
  public:
    /**
     * @brief Constructor por defecto. Crea un LettersSet.
    */
    LettersSet();
    /**
    * @brief Constructor por copia. Crea un LettersSet a partir de otro.
    * @param other LettersSet que se va a copiar.
    */
    LettersSet(const LettersSet & other);
    /**
   * @brief Inserta el elemento @p val en el LettersSet.
   * @param val Pareja formada por un caracter y un LetterInfo.
   * @return Bool que indica si se ha podido insertar el caracter en el LettersSet (solo se inserta si no estaba ya incluido).
   */
    bool insert(const pair<char,LetterInfo> & val);
    /**
     * @brief Elimina el caracter @p key.
     * @param key Caracter que se quiere eliminar.
     * @return Bool que indica si se ha podido eliminar el caracter.
    */
    bool erase(const char & key);
    /**
     * @brief Comprueba si está vací­o el lettersSet.
     * @return true si está vací­o, false en caso contrario.
    */
    bool empty() const;
    /**
     * @brief Borra el contenido del lettersSet.
    */
    void clear();
    /**
    * @brief Indica el número de elementos que hay en el lettersSet.
    * @return Tamaño del lettersSet.
    */
    unsigned int size();
    /**
    * @brief Calcula la puntuación de @p word como la suma de las puntuaciones de cada letra.
    * @param word String del que queremos calcular la puntuación.
    * @return Puntuación de la palabra.
   */
    int getScore(string word);
    /**
   * @brief Sobrecarga del operador de asignación.
   * @param cl LettersSet a copiar.
   * @return Referencia al objeto @p  para poder encadenar el operador.
   */
    LettersSet & operator = (const LettersSet & cl);
    /**
    * @brief Sobrecarga del operador de consulta. Para acceder a los elementos del map que hay en la clase.
    * @param val Carácter que queremos consultar.
    * @return Estructura de tipo LetterInfo con la información del caracter: puntuación y número de repeticiones .
    */
    LetterInfo & operator [] (const char & val);
    /**
     * @brief Sobrecarga del operador de salida.
     * @param os Flujo de salida, donde se  va a escribir el LettersSet.
     * @param cl LettersSet que se escribe.
     * @return El flujo de salida, @p os.
     */
    friend ostream & operator << (ostream & os, const LettersSet & cl);
    /**
     * @brief Sobrecarga del operador de entrada.
     * @param is Flujo de entrada, del que se va a  leer el LettersSet.
     * @param cl LettersSet en el que se va a almacendar la información leí­da.
     * @return El flujo de entrada, @p is.
     */
    friend istream & operator >> (istream & is, LettersSet & cl);

    /**
    * @brief Iterador del LettersSet. Permite recorrer los elementos del LettersSet
    */
    class iterator{
    private:
        map<char,LetterInfo>::iterator i;
    public:
        /**
         * @brief Constructor por defecto del iterador. No hace nada
         */
        iterator(){}

        /**
         * @brief Constructor con parámetros
         * @param other Iterador del map que se va a asignar internamente al iterador
         */
        iterator(const map<char,LetterInfo>::iterator & other): i(other){}

        /**
         * @brief Constructor de copia sobre otro iterador de LettersSet
         * @param other Iterador de LettersSet que se va a copiar
         */
        iterator(const iterator & other):i(other.i){}

        /**
         * @brief Destructor del iterador. No hace nada
         */
        ~iterator(){}

        /**
         * @brief Sobrecarga del operador de asignación sobre un iterador de map
         */
        iterator & operator = (const map<char,LetterInfo>::iterator & other){i = other; return *this;}

        /**
         * @brief Sobrecarga del operador de asignación sobre otro iterador de LettersSet
         */
        iterator & operator = (const iterator & other) {i = other.i; return *this;}

        /**
         * @brief Sobrecarga del operador de acceso al valor al que apunta el iterador
         */
        const pair<char,LetterInfo> operator * () const{return *i;}

        /**
         * @brief Sobrecarga del operador de incremento
         */
        iterator & operator ++ () {++i; return *this;}

        /**
         * @brief Sobrecarga del operador de decremento
         */
        iterator & operator -- () {--i; return *this;}

        /**
         * @brief Sobrecarga del operador de incremento específico para valores enteros
         */
        iterator & operator ++ (int) {i++; return *this;}

        /**
         * @brief Sobrecarga del operador de decremento específico para valores enteros
         */
        iterator & operator -- (int) {i--; return *this;}

        /**
         * @brief Sobrecarga del operador de desigualdad
         * @param other Iterador con el que se va a comparar
         */
        bool operator != (const iterator & other) {return i != other.i;}

        /**
         * @brief Sobrecarga del operador de igualdad
         * @param other Iterador con el que se va a comparar
         */
        bool operator == (const iterator & other) {return i == other.i;}
    };

    /**
     * @brief Obtiene el iterador al inicio del LettersSet
     * @return Iterador al inicio del LettersSet 
     */
    iterator begin() { iterator i = letters.begin(); return i; }

    /**
     * @brief Obtiene el iterador al final del LettersSet
     * @return Iterador al final del LettersSet 
     */
    iterator end() { iterator i = letters.end(); return i; }

    /**
    * @brief Iterador constante del LettersSet. Permite recorrer los elementos del LettersSet sin modificarlos
    */
    class const_iterator{
    private:
        map<char,LetterInfo>::const_iterator i;
    public:
        /**
         * @brief Constructor por defecto del iterador. No hace nada
         */
        const_iterator(){}

        /**
         * @brief Constructor con parámetros
         * @param other Iterador constante del map que se va a asignar internamente al iterador
         */
        const_iterator(const map<char,LetterInfo>::const_iterator & other): i(other){}

        /**
         * @brief Constructor de copia sobre otro iterador constante de LettersSet
         * @param other Iterador constante de LettersSet que se va a copiar
         */
        const_iterator(const const_iterator & other):i(other.i){}

        /**
         * @brief Destructor del iterador. No hace nada
         */
        ~const_iterator(){}

        /**
         * @brief Sobrecarga del operador de asignación sobre un iterador constante de map
         */
        const_iterator & operator = (const map<char,LetterInfo>::const_iterator & other){i = other; return *this;}

        /**
         * @brief Sobrecarga del operador de asignación sobre otro iterador constante de LettersSet
         */
        const_iterator & operator = (const const_iterator & other) {i = other.i; return *this;}

        /**
         * @brief Sobrecarga del operador de acceso al valor al que apunta el iterador
         */
        const pair<char,LetterInfo> operator * () const{return *i;}

        /**
         * @brief Sobrecarga del operador de incremento
         */
        const_iterator & operator ++ () {++i; return *this;}

        /**
         * @brief Sobrecarga del operador de decremento
         */
        const_iterator & operator -- () {--i; return *this;}

        /**
         * @brief Sobrecarga del operador de incremento específico para valores enteros
         */
        const_iterator & operator ++ (int) {i++; return *this;}

        /**
         * @brief Sobrecarga del operador de decremento específico para valores enteros
         */
        const_iterator & operator -- (int) {i--; return *this;}

        /**
         * @brief Sobrecarga del operador de desigualdad
         * @param other Iterador constante con el que se va a comparar
         */
        bool operator != (const const_iterator & other) {return i != other.i;}

        /**
         * @brief Sobrecarga del operador de igualdad
         * @param other Iterador constante con el que se va a comparar
         */
        bool operator == (const const_iterator & other) {return i == other.i;}
    };

    /**
     * @brief Obtiene el iterador constante al inicio del LettersSet
     * @return Iterador constante al inicio del LettersSet 
     */
    const_iterator cbegin() const { const_iterator i = letters.begin(); return i; }

    /**
     * @brief Obtiene el iterador constante al final del LettersSet
     * @return Iterador constante al final del LettersSet 
     */
    const_iterator cend() const { const_iterator i = letters.end(); return i; }
};

/**
 * @brief Sobrecarga del operador de salida.
 * @param os Flujo de salida, donde se  va a escribir el LettersSet.
 * @param cl LettersSet que se escribe.
 * @return El flujo de salida, @p os.
 */
ostream & operator << (ostream & os, const LettersSet & cl);

/**
 * @brief Sobrecarga del operador de entrada.
 * @param is Flujo de entrada, del que se va a leer el LettersSet.
 * @param cl LettersSet en el que se va a almacendar la información leí­da.
 * @return El flujo de entrada, @p is.
 */
istream & operator >> (istream & is, LettersSet & cl);

#endif