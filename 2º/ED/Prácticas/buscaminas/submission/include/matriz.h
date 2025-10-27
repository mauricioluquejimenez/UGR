//Representación de Matriz fichero matriz.h
#ifndef __MATRIZ__H
#define __MATRIZ__H

#include <iostream>

using namespace std;

/**
 * @brief Clase plantilla para representar una matriz de elementos de tipo T
 * @tparam T Tipo de dato plantilla para almacenar en la matriz
 */
template <class T>
class Matriz
{
    private:
        T** datos; 
        int nf,nc;
    
    public:

        /**
         * @brief Copia el contenido de una matriz M a una nueva matriz
         * @param M Matriz de la que se copia el contenido
         */
        void Copiar(const Matriz<T> &M);

        /**
         * @brief Libera espacio de una matriz existente
         * @post La matriz debe quedar vacía
         */
        void Libera();

        /**
         * @brief Constructor por defecto
         */
        Matriz();

        /**
         * @brief Constructor de una matriz con f filas y c columnas
         * @param f Número de filas de la nueva matriz
         * @param c Número de oclumnas de la nueva matriz
         */
        Matriz(int f,int c);

        /**
         * @brief Constructor de copia
         * @param M Matriz de refererencia
         */
        Matriz(const Matriz<T> & M);

        /**
         * @brief Destructor de la matriz. Libera el espacio de la matriz y la elimina
         * @post La matriz deja de existir
         */
        ~Matriz();

        /**
         * @brief Sobrecarga del operador de asignación
         * @param M Matriz de referencia que se le asigna a la matriz actual
         * @return Matriz de tipo T con el contenido de M
         */
        Matriz<T> & operator=(const Matriz<T> &M);

        /**
         * @brief Devuelve el elemento de la fila f y columna c
         * 
         * @param f Fila de la matriz
         * @param c Columna de la matriz
         * @return Elemento de la matriz en la posición (f,c), de tipo t
         */
        T get(int f, int c)const;

        /**
         * @brief Asigna el valor v en la posición (f,c) de la matriz
         * 
         * @param f Fila de la matriz
         * @param c Columna de la matriz
         * @param v Valor a asignar en la posición (f,c)
         */
        void set(int f,int c, const T &v);

        /**
         * @brief Devuelve el número de filas de la matriz
         * @return Entero con el número de filas 
         */
        int getNumFilas()const;

        /**
         * @brief Devuelve el número de columnas de la matriz
         * @return Entero con el número de columnas 
         */
        int getNumCols()const;
        
        /**
         * @brief Sobrecarga del operador de salida para mostrar el contenido de la matriz M
         * @tparam U 
         * @param os Flujo de salida
         * @param M Matriz que se va a mostrar por pantalla
         * @return ostream& 
         */
        template <class U> friend ostream & operator<<(ostream &os, const Matriz<U> &M);
};

#include <src/matriz.cpp>
#endif