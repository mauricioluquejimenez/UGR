/**
 * @file maxqueue.h
 * @brief  Archivo de especificación del TDA MaxQueue
 * @author Mauricio Luque Jiménez
 */

#include <iostream>
#include <stack>

using namespace std;

/**
 * @brief Estructura para representar un elemento del MaxQueue. Requiere sobrecarga del operador << para mostrar correctamente su contenido
 * @param e Elemento a almacenar en la cola
 * @param max Elemento máximo almacenado en la cola
 */
struct element
{
    int e;
    int max;

    friend ostream & operator <<(ostream & flujo, const element & n)
    {
        flujo << n.e << "," << n.max;
        return flujo;
    }
};

/**
 * @brief TDA MaxQueue. Implementa una cola con capacidad de obtener el elemento máximo en tiempo constante
 * @param pila pila que almacena los elementos del TDA
 * @author Mauricio Luque Jiménez
 */
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
         * @brief Añade un elemento a la cola. Crea un nuevo elemento auxiliar al que le asigna el argumento e, y mientras que la pila no esté vacía, lo compara con el máximo del elemento que está en la cima. Si el máximo actual es menor que e, actualiza dicho valor y elimina el elemento de la cima, además de guardarlo en la pila auxiliar. Finalmente, va rellenando la pila original con los elementos guardados en la pila auxiliar.
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