//
// Created by 
//

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <cctype>

using namespace std;

/**
 * @brief TDA Dictionary. Almacena un conjunto de palabras y permite operaciones de inserción, borrado y búsqueda.
 */
class Dictionary {
  private:
    set<string> words;

  public:

    /**
    * @brief Constructor por defecto. Crea un Dictionary vacío
    */
    Dictionary(){};

    /**
    * @brief Destructor
    */
    ~Dictionary(){};

    /**
    * @brief Limpia el Dictionary
    * @post el diccionario queda con 0 palabras
    */
    void clear();

    /**
    * @brief Tamaño del diccionario
    * @return Número de palabras guardadas en el diccionario
    */
    unsigned int size() const;

    /**
    * @brief Comprueba si el diccionario está vacio.
    * @return true si el diccionario está vacío, false en caso contrario
    */
    bool empty() const { return words.size() == 0; }

    /**
    * @brief Indica si una palabra esta en el diccionario o no. Este método comprueba si una determinada palabra se encuentra o no en el dicccionario
    * @param val: la palabra que se quiere buscar.
    * @return Booleano indicando si la palabra existe o no en el diccionario
    */
    bool exists(const string &val);

    /**
    * @brief Elimina una palabra del diccionario
    * @param val Palabra a borrar del diccionario
    * @return Booleano que indica si la palabra se ha borrado del diccionario
    */
    bool erase(const string &val);

    /**
    * @brief Sobrecarga del operador de entrada. Permite leer las palabras de un fichero de texto e introducirlas en el diccionario
    * @param is Flujo de entrada
    * @param dic Diccionario a rellenar
    * @return Flujo de entrada para poder encadenar el operador
    */
    friend istream &operator>>(istream &is, Dictionary &dic);

    /**
    * @brief Sobrecarga del operador de salida. Permite imprimir el diccionario completo a un flujo de salida
    * @param os Flujo de salida, donde imprimir el diccionario
    * @param dic Diccionario a imprimir
    * @return Flujo de salida, para poder encadenar el operador
    */
    friend ostream &operator<<(ostream &os, const Dictionary &dic);
    /**
    * @brief Indica el numero de apariciones de una letra.
    * @param c letra a buscar.
    * @return Un entero indicando el numero de apariciones.
    */
    int getOcurrences(const char c) const;

    /**
    * @brief Indica el numero totales de letras
    * @return Un entero indicando el numero totales de letras
    */
    int getTotalLetters() const;

    /**
    * @brief Obtiene todas las palabras de una longitud
    * @param longitud: valor de longitud de la palabras a devolver
    * @return un vector con palabra de la longitud dada
    */
    vector<string> getWordsLength(int longitud);

    /**
    * @brief Iterador del diccionario. Obtiene las palabras ordenadas alfabéticamente
    */
    class iterator
    {
      private:
        set<string>::iterator it;

      public:
        /**
         * @brief Constructor por defecto del iterador. No hace nada
         */
        iterator();

        /**
         * @brief Sobrecarga del operador de igualdad
         * @param i Iterador con el que se va a comparar
         */
        bool operator==(const iterator &i) const;

        /**
         * @brief Sobrecarga del operador de desigualdad
         * @param i Iterador con el que se va a comparar
         */
        bool operator!=(const iterator &i) const;

        /**
         * @brief Sobrecarga del operador de incremento
         */
        iterator &operator++();

        /**
         * @brief Sobrecarga del operador de acceso al valor al que apunta el iterador
         */
        const string &operator*();
        friend class Dictionary;
    };

    /**
    * @brief Iterador constante del diccionario. Obtiene las palabras ordenadas alfabéticamente
    */
    class const_iterator
    {
      private:
        set<string>::const_iterator it;

      public:

        /**
         * @brief Constructor por defecto del iterador. No hace nada
         */
        const_iterator();

        /**
         * @brief Sobrecarga del operador de igualdad
         * @param i Iterador constante con el que se va a comparar
         */
        bool operator==(const const_iterator &i) const;

        /**
         * @brief Sobrecarga del operador de desigualdad
         * @param i Iterador constante con el que se va a comparar
         */
        bool operator!=(const const_iterator &i) const;

        /**
         * @brief Sobrecarga del operador de incremento
         */
        const_iterator &operator++();

        /**
         * @brief Sobrecarga del operador de acceso al valor al que apunta el iterador
         */
        const string &operator*() const;
        friend class Dictionary;
    };

    /**
    * @brief Indica si una palabra esta en el diccionario
    * @return iterador apuntando a la palabra si esta o end si no esta
    */
    iterator find(const string &w);

    /**
    * @brief Inserta una palabra en el diccionario
    * @param val palabra que se quiere insertar en el diccionario
    * @return un pair con un iterator apuntando a la palabra insertada y un booleano que indica si la palabra se ha insertado o no
    */
    pair<iterator, bool> insert(const string &val);

    /**
    * @brief Busca un las palabras con un prefijo
    * @param val prefijo a buscar
    * @return un pair con dos iteradores el primero apuntando a la primera palaba con el prefijo y el segundo donde ya no contiene el prefijo. Si no existe el prefijo se devuelve los dos iteradores apuntando a end()
    */
    pair<iterator, iterator> range_prefix(const string &val);

    /**
    * @brief Obtiene el iterador apuntando a la primera palabra del diccionario
    */
    iterator begin();

    /**
    * @brief Obtiene el iterador constante apuntando a la primera palabra del diccionario
    */
    const_iterator cbegin() const;

    /**
    * @brief Obtiene el iterador apuntando al final del diccionario.
    */
    iterator end();

    /**
    * @brief Obtiene el iterador constante apuntando al final del diccionario.
    */
    const_iterator cend() const;
};
#endif //DICTIONARY_H
