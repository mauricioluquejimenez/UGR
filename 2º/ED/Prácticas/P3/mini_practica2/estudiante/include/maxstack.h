/**
 * @file maxstack.h
 * @brief  Archivo de especificación del TDA MaxStack
 * @author Mauricio Luque Jiménez
 */

#include <iostream>
#include <queue>

using namespace std;

struct element
{
    int e;
    int max;

    friend ostream & operator << (ostream & flujo, const element & n)
    {
        flujo << n.e << "," << n.max;
        return flujo;
    }
};

class MaxStack
{
    private:
        queue<element> cola;
    
    public:

        /**
            * @brief Comprueba si la pila está vacía
            * @return true si la pila está vacía, false en caso contrario
         */
        bool empty() const;

        /**
         * @brief Añade un elemento a la pila
         * @param e Elemento a añadir
         */
        void push(const int & e);

        /**
         * @brief Elimina el último elemento de la pila
         * @pre La pila no está vacía
         * @post La pila queda con un elemento menos
         */
        void pop();

        /**
         * @brief Devuelve el último elemento de la pila
         * @pre La pila no está vacía
         * @return Último elemento de la pila
         */
        element top() const;

        /**
         * @brief Devuelve el tamaño de la pila
         * @return Tamaño de la pila 
         */
        int size() const;
};