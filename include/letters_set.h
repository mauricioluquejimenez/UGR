#ifndef __LETTER_SET_H__
#define __LETTER_SET_H__

#include <iostream>
#include <map>

using namespace std;

struct LetterInfo{
  unsigned int repetitions;
  unsigned int score;

  /**
   * @brief Constructor por defecto
   */
  LetterInfo(): repetitions(0), score(0){}

  /**
   * @brief Constructor con parámetros
   *
   * @param reps Número de repeticiones del carácter en la partida
   * @param score Puntuación del carácter
   */
  LetterInfo(unsigned int reps, unsigned int score): repetitions(reps), score(score){};
};


class LettersSet{
private:
  map<char, LetterInfo> letters;
  public:
    /**
     * @brief Constructor por defecto.
     * Crea un LettersSet.
    */
    LettersSet();
    /**
    * @brief Constructor por copia.
     * Crea un LettersSet a partir de otro.
    * @param other LettersSet que se va a copiar.
    */
    LettersSet(const LettersSet & other);
    /**
   * @brief Inserta el elemento @p val en el LettersSet.
   * @param val Pareja formada por un caracter y un LetterInfo.
   * @return Bool que indica si se ha podido insertar el caracter en el LettersSet (solo se inserta
   * si no estaba ya incluido).
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
    * Calcula la puntuación de @p word como la suma de las puntuaciones de cada letra.
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
    * @brief Sobrecarga del operador de consulta.
    * Para acceder a los elementos del map que hay en la clase.
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
     * Sobrecarga del operador de entrada.
     * @param is Flujo de entrada, del que se va a  leer el LettersSet.
     * @param cl LettersSet en el que se va a almacendar la información leí­da.
     * @return El flujo de entrada, @p is.
     */
    friend istream & operator >> (istream & is, LettersSet & cl);

    class iterator{
    private:
        map<char,LetterInfo>::iterator i;
    public:
        iterator(){}
        iterator(const map<char,LetterInfo>::iterator & other): i(other){}
        iterator(const iterator & other):i(other.i){}
        ~iterator(){}
        iterator & operator = (const map<char,LetterInfo>::iterator & other){i = other; return *this;}
        iterator & operator = (const iterator & other) {i = other.i; return *this;}
        const pair<char,LetterInfo> operator * () const{return *i;}
        iterator & operator ++ () {++i; return *this;}
        iterator & operator -- () {--i; return *this;}
        iterator & operator ++ (int) {i++; return *this;}
        iterator & operator -- (int) {i--; return *this;}
        bool operator != (const iterator & other) {return i != other.i;}
        bool operator == (const iterator & other) {return i == other.i;}
    };

    iterator begin() { iterator i = letters.begin(); return i; }
    iterator end() { iterator i = letters.end(); return i; }

    class const_iterator{
    private:
        map<char,LetterInfo>::const_iterator i;
    public:
        const_iterator(){}
        const_iterator(const map<char,LetterInfo>::const_iterator & other): i(other){}
        const_iterator(const const_iterator & other):i(other.i){}
        ~const_iterator(){}
        const_iterator & operator = (const map<char,LetterInfo>::const_iterator & other){i = other; return *this;}
        const_iterator & operator = (const const_iterator & other) {i = other.i; return *this;}
        const pair<char,LetterInfo> operator * () const{return *i;}
        const_iterator & operator ++ () {++i; return *this;}
        const_iterator & operator -- () {--i; return *this;}
        const_iterator & operator ++ (int) {i++; return *this;}
        const_iterator & operator -- (int) {i--; return *this;}
        bool operator != (const const_iterator & other) {return i != other.i;}
        bool operator == (const const_iterator & other) {return i == other.i;}
    };

    const_iterator cbegin() const { const_iterator i = letters.begin(); return i; }
    const_iterator cend() const { const_iterator i = letters.end(); return i; }


};

ostream & operator << (ostream & os, const LettersSet & cl);
istream & operator >> (istream & is, LettersSet & cl);

#endif
