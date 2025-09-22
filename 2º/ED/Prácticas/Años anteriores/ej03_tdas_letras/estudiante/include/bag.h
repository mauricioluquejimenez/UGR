#ifndef __BAG_H__
#define __BAG_H__

/**
 *  \brief TDA abstracto Bolsa
 *
 *  Este TDA abstracto nos permite trabajar con una colección de elementos que
 *  permite la extracción de elementos de forma aleatoria sin reemplazamiento
 */

#include <iostream>
#include <vector>

using namespace std;

template <class T>

class Bag{
    private:
        vector<T> v;

    public:

        /**
         * @brief Constructor por defecto
         */
        Bag()=default;

        /**
         * @brief Constructor de copia. Crea una copia exacta de otro objeto de tipo Bag
         * @param other Objeto de tipo Bag<T> del que se va a realizar la copia
         */
        Bag(const Bag<T> & other){ *this = other; }

        /**
         * @brief Añade un elemento a la bolsa
         * @param element Elemento del tipo T a añadir a la bolsa
         */
        void add(const T & element){ v.push_back(element); }

        /**
         * @brief Extrae un elemento aleatorio de la bolsa. Devuelve un elemento aleatorio de la bolsa y lo elimina de la misma.
         * @return Elemento de tipo T extraído de la bolsa
         * @pre La bolsa no está vacía
         * @post El elemento devuelto se ha eliminado de la bolsa
         */
        T get(){
            int n;
            T element;

            n = rand() % v.size()-1;
            swap(v.at(n), v.back());
            element = v.back();
            v.pop_back();

            return element;
        }

        /**
         * @brief Elimina todos los elementos de la bolsa. Borra todos los elementos almacenados en la bolsa         * 
         */
        void clear(){ v.clear(); }

        /**
         * @brief Tamaño de la bolsa
         * @return Número de elementos que hay en la bolsa
         */
        unsigned int size() const { return v.size(); }

        /**
         * @brief Comprueba si la bolsa está vacía
         * @return true si la bolsa está vacía, false en caso contrario 
         */
        bool empty(){ return v.empty(); }

        /**
         * @brief Sobrecarga del operador de asignación
         * @param other Bag<t> a copiar
         * @return Referencia a this para poder encadenar el operador
         */
        const Bag<T> & operator=(const Bag<T> & other){ v = other.v; return *this; }
};

#endif
