#include "matriz.h"

template <class T>
void Matriz<T>::Copiar(const Matriz<T> &M)
{
    nf = M.nf;
    nc = M.nc;
    datos = new T*[nf];

    for (int i = 0; i < nf; i++) 
    {
        datos[i] = new T[nc];
        for (int j = 0; j < nc; j++) datos[i][j] = M.datos[i][j];
    }
}

template <class T>
void Matriz<T>::Libera()
{
    for (int i = 0; i < nf; i++) delete [] datos[i];
    delete [] datos;
}

template <class T>
Matriz<T>::Matriz()
{
    nf = 0;
    nc = 0;
    datos = nullptr;
}

template <class T>
Matriz<T>::Matriz(int f,int c)
{
    nf = f;
    nc = c;
    datos = new T*[nf];

    for (int i = 0; i < nf; i++) datos[i] = new T[nc];
}

template <class T>
Matriz<T>::Matriz(const Matriz<T> & M)
{
    Copiar(M);
}

template <class T>  
Matriz<T>::~Matriz()
{
    Libera();
}

template <class T>
Matriz<T> & Matriz<T>::operator=(const Matriz<T> &M)
{
    if (this != &M)
    {
        Libera();
        Copiar(M);
    }
    return *this;
}

template <class T>
T Matriz<T>::get(int f ,int c) const
{ 
    return datos[f][c];
}

template <class T>
void Matriz<T>::set(int f,int c, const T &v)
{
    datos[f][c] = v;    
}

template <class T>
int Matriz<T>::getNumFilas()const
{
    return nf;
}

template <class T>
int Matriz<T>::getNumCols()const
{
    return nc;
}

template <class U>
ostream & operator<<(ostream &os, const Matriz<U> &M)
{
    for (int i = 0; i < M.nf; i++)
    {
        for (int j = 0; j < M.nc; j++) os << M.datos[i][j] << " ";
        os << endl;
    }
    return os;
}