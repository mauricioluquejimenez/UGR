/**
 * @file maxqueue.h
 * @brief  Archivo de especificación del TDA MaxQueue
 * @author Mauricio Luque Jiménez
 */

#include <iostream>
#include <stack>

using namespace std;

struct element
{
    int e;
    int max;

    friend ostream & operator << (ostream & flujo, const element & n)
    {
        flujo << n.e << " (" << n.max << ")";
        return flujo;
    }
};

class MaxQueue
{
    private:
        stack<element> pila;
    
    public:

        /**
            * @brief Comprueba si la cola está vacía
            * @return true si la cola está vacía, false en caso contrario
         */
        bool empty() const;

        /**
         * @brief Añade un elemento a la cola
         * @param e Elemento a añadir
         */
        void push(const int & e);

        /**
         * @brief Elimina el último elemento de la cola
         * @pre La cola no está vacía
         * @post La cola queda con un elemento menos
         */
        void pop();

        /**
         * @brief Devuelve el primer elemento de la cola
         * @pre La cola no está vacía
         * @return Primer elemento de la cola
         */
        element front() const;

        /**
         * @brief Devuelve el tamaño de la cola
         * @return Tamaño de la cola 
         */
        int size() const;
};

