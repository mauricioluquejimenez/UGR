#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

/**
 * @brief TDA Dictionary
 * @details Almacena las palabras de un fichero de texto y permite iterar sobre ellas
 *
 */

#include <iostream>
#include <string>
#include <set>
#include <vector>

using namespace std;

class Dictionary{
    private:
        set <string> words;

    public:

        /**
         * @brief Constructor por defecto
         */
        Dictionary()=default;

        /**
         * @brief Constructor de copia. Crea un Dictionary con el mismo contenido que el que se va a pasar como argumento
         * @param other Dictionary que se quiere copiar
         */
        Dictionary(const Dictionary & other);

        /**
         * @brief Indica si una palabra está en el diccionario o no. Este método comprueba si una determinada palabra se encuentra o no en el diccionario
         * @param val La palabra que se quiere buscar
         * @return Booleano indicando si la palabra existe o no en el diccionario 
         */
        bool exists(const string & val) const;

        /**
         * @brief Inserta una palabra en el diccionario
         * @param val Palabra a insertar en el diccionario
         * @return Booleano que indica si la inserción ha tenido éxito. Una palabra se inserta con éxito si no existía previamente en el diccionario. 
         */
        bool insert(const string & val);

        /**
         * @brief Elimina una palabra del diccionario
         * @param val Palabra a borrar en el diccionario
         * @return Booleano que indica si la palabra se ha borrado del diccionario 
         */
        bool erase(const string & val);

        /**
         * @brief Limpia el Dictionary. Elimina todas las palabras contenidas en el conjunto 
         */
        void clear();

        /**
         * @brief Comprueba si el diccionario está vacío
         * @return true si el diccionario está vacío, false en caso contrario 
         */
        bool empty() const ;

        /**
         * @brief Tamaño del diccionario
         * @return Número de palabras guardadas en el diccionario
         */
        unsigned int size() const;

        /**
         * @brief Indica el número de apariciones de una letra
         * @param c Letra a buscar
         * @return Un entero indicando el número de apariciones 
         */
        int getOcurrences(const char c);

        /**
         * @brief Cuenta el total de letras de un diccionario
         * @return Entero con el total de letras 
         */
        int getTotalLetters();

        /**
         * @brief Devuelve las palabras en el diccionario con una longitud dada
         * @param length Longitud de las palabras buscadas
         * @return Vector de palabras con la longitud deseada
         */
        vector<string> wordsOfLength(int length);

        /**
         * @brief Sobrecarga del operador de salida. Permite imprimir el diccionario completo a un flujo de salida
         * @param os Flujo de salida, donde imprimir el diccionario
         * @param dic Diccionario a imprimir
         * @return Flujo de salida, para poder encadenar el operador
         */
        friend ostream & operator << (ostream & os, const Dictionary & dic);

        /**
         * @brief Sobrecarga del operador de entrada. Permite leer las palabras de un fichero de texto e introducirlas en el diccionario
         * @param is Flujo de entrada
         * @param dic Diccionario a rellenar
         * @return Flujo de entrada para poder encadenar el operador
         */
        friend istream & operator >> (istream & is, Dictionary & dic);
};

ostream & operator << (ostream & os, const Dictionary & dic);
istream & operator >> (istream & is, Dictionary & dic);

#endif
