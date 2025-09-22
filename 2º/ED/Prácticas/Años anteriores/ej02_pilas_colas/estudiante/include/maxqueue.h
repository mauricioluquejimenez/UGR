/**
 * @file maxqueue.h
 * @brief  Archivo de especificación del TDA MaxQueue
 * @author Mauricio Luque JImenez, Salma Ezzamarty
 */

#include<iostream>
#include<stack>
using namespace std;

struct element{
    int elem;
    int maximum;
    friend std::ostream & operator <<(std::ostream &os, const element &el){
        os << el.elem << ","<< el.maximum;
        return os;
    }
};

class MaxQueue{
private:
    stack<element> pila;
public:

    /**
    * @brief Comprueba si la cola está vacía
    * @return Devuelve true si está vacía y false si no
    */
    bool isEmpty() const;

    /**
     * @brief Añade un elemento a la cola en su ultima posicion
     * @param elemento Valor que se añade a la cola
     */
    void push(const int &elemento);

    /**
     *  @brief Elimina el último valor de la cola
     *  @pre La cola debe tener como mínimo n > 0 elementos
     *  @post se elimina el elemento
     */
    void pop();

    /**
     * @brief Devuelve el primer elemento de la cola
     * @return Primer elemento de la cima
     */
    element front();

    /**
    * @brief Devuelve el numeor de elementos que se encuentran en  la pila.
    * @return Tamaño de la pila
    */
    int size() const;
};

/**
 * @brief Sobrecarga del operador de salida
 * @param flujo Flujo de salida
 * @param n Elemento de la cola mostrado
 * @return Referencia al flujo de salida
 */
ostream & operator << (ostream & flujo, const element & n);