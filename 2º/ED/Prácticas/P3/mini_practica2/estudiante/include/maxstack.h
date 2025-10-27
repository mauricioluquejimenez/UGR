/**
 * @file maxstack.h
 * @brief  Archivo de especificación del TDA MaxStack
 * @author Mauricio Luque Jiménez
 */

#include <iostream>
#include <queue>

using namespace std;

/**
 * @brief Estructura para representar un elemento del MaxStack. Requiere sobrecarga del operador << para mostrar correctamente su contenido
 * @param e Elemento a almacenar en la pila
 * @param max Elemento máximo almacenado en la pila
 */
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

/**
 * @brief TDA MaxStack. Implementa una pila con capacidad de obtener el elemento máximo en tiempo constante
 * @param cola cola que almacena los elementos del TDA
 * @author Mauricio Luque Jiménez
 */
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
         * @brief Añade un elemento a la pila. Crea un nuevo elemento auxiliar al que le asigna el argumento e. Si la cola está vacía, también asigna e como el valor máximo. En caso contrario, compara e con el máximo del elemento que está al final de la cola para decidir si al máximo del valor auxiliar le asigna el parámetro e o el máximo del elemento final de la cola. Finalmente, vuelva toda la cola auxiliar en original en la cola auxiliar, para acabar copiando el valor actual de la cola auxiliar en la cola original.
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