//Representación de Matriz fichero matriz.h #ifndef __MATRIZ__H
#define __MATRIZ__H template <class T>
class Matriz{
    private:
        T** datos; int nf,nc;
    
    public:
        void Copiar(const Matriz<T> &M);//copia una matriz
        void Libera(); //libera la memoria
        Matriz();
        Matriz(int f,int c);
        Matriz(const Matriz<T> & M);
        ~Matriz();
        Matriz<T> & operator=(const Matriz<T> &M);
        T get(int f,in c)const;
        void set(int f,int c, const T &v);
        int getNumFilas()const;
        int getNumCols()const;
        
        template <class U> friend ostream & operator<<(ostream &os, const Matriz<U> &M);
};

#include "matriz.cpp"
#endif