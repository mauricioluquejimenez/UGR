#include <iostream>
#include <set>
#include <map>

using namespace std;

bool complementarios(const set<int> & a, const set<int> & b)
{

    // Como cada elemento del segundo conjunto tiene que ir entre dos del primero
    // éste tiene que tener exactamente un elemento más
    if(b.size() != a.size() - 1) return false;
    
    set<int>::const_iterator it1, itsig, itfinal, it2;

    // Iteradores para recorrer el primer conjunto: el valor actual y el siguiente
    it1 = a.cbegin();

    // Hay que evitar que el iterador siguiente se salga del conjunto: el valor "actual" no puede ser el último
    itfinal = a.cend(); itfinal--;

    // Iterador para recorrer el segundo conjunto
    it2 = b.cbegin();

    // Mientras el iterador actual no llegue al PENÚLTIMO elemento del primer conjunto...
    while(it1 != itfinal)
    {
        // Se actualiza el iterador siguiente
        itsig = it1; ++itsig;

        // Si el valor del segundo conjunto no está entre los dos del primero, se devuelve false
        if(!(*it1 <= *it2 && *it2 < *itsig)) return false;

        // Si no se cumple la condición (negativa) anterior, se avanza en ambos conjuntos
        ++it1;
        ++it2;
    }

    // Si se ejecuta esta línea es porque no se ha devuelto false previamente, ni en la comprobación del tamaño ni al entrar en el bucle
    // En ese caso, los conjuntos son complementarios
    return true;
}

int main(){
    set<int> c1({1,5,7}), c2({2,6}),c3({2,3});
    if (complementarios(c1,c3))
            cout<<"Son complmentarios" <<endl;
    else
            cout<<"No son complmentarios" <<endl;
}