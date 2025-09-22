/**
 * @file maxstack.h
 * @brief  Archivo de especificación del TDA MaxStack
 * @author
 */
#include <ostream>
#include <iostream>
#include <cstdlib>
#include <queue>
using namespace std;

struct element{
    int elem;
    int maximum;
    friend std::ostream & operator <<(std::ostream &os, const element &el){
        os << el.elem << ","<< el.maximum;
        return os;
    }
};
/**
 * @file maxstack.h
 * @brief  Archivo de especificación del TDA MaxStack
 * @author
 */

class MaxStack{
private:
    queue<element> cola;
public:

    /**
     * @brief Comprueba si la pila está vacía
     * @return Devuelve true si está vacía y false si no
     */
    bool isEmpty() const;

    /**
     * @brief Añade un elemento a la pila
     * @param elemento Valor que se añade a la pila
     */
    void push(const int &elemento);

    /**
    *  @brief Elimina el  valor de la cima  de la pila
    *  @pre La pila no debe estar vacía
     *  @post se elimina el elemento
    */
    void pop();

    /**
    * @brief Devuelve el elemento situado en la cima de la pila
     * @return Elemento de la cima
    */
    element top();

    /**
     * @brief Devuelve el numeor de elementos que se encuentran en  la pila.
     * @return Tamaño de la pila
     */
    int size() const;
};

/**
 * @brief Sobrecarga del operador de salida
 * @param flujo Flujo de salida
 * @param n Elemento de la pila mostrado
 * @return Referencia al flujo de salida
 */
ostream & operator<<(ostream & flujo, const element & n);