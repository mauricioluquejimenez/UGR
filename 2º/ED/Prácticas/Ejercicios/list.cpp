#include <iostream>
#include <list>

using namespace std;

// FUNCIÓN A IMPLEMENTAR
void max_sublist(list<int> &L, list<int> &subl)
{
    list<int>::iterator it = L.begin();
    list<int> aux;

    // subl.clear();

    while(it != L.end())
    {
        if(*it % 2 == 0) aux.push_back(*it);
        else
        {
            if(aux.size() > subl.size()) subl = aux;
            aux.clear();
        }
        ++it;
    }

    if(aux.size() > subl.size()) subl = aux;
}

void dividir_lista(list<int> &L, int k)
{
    int contador = 0;
    list<int>::iterator it = L.begin();

    while (contador != L.size())
    {
        contador++;
        if((*it) >= k)
        {
            L.push_back(*it);
            it = L.erase(it);
        }
        else ++it;
    }

    /*
    
    OTRA MANERA DE HACERLO
    
        EN VEZ DE MOVER ELEMENTOS AL FINAL, SE MUEVEN AL PRINCIPIO
        DE ESTA MANERA, EL ITERATOR SE QUEDA SIN ELEMENTOS QUE CONSULTAR Y LLEGA AL FINAL DE LA LISTA

    list<int>::iterator it = L.begin();

    while(it != L.end())
    {
        if((*it) < k)
        {
            L.push_front(*it);
            it = L.erase(it);
        }
        else ++it;
    }
    
    */
}

void agrupar_elemento(list<int> & entrada, int k)
{
    list<int>::iterator it = entrada.begin();
    list<int>::iterator it_k = entrada.end();

    while(it != entrada.end())
    {
        if((*it) == k)
        {
            it_k = it;
            break;
        }
        it++;
    }

    if(it_k == entrada.end()) return;

    while(it != entrada.end())
    {
        if((*it) == k) entrada.splice(it_k, entrada, it);
        ++it;
    }
        
}

void duplicar(const list<int> & lista_inicial, list<int> & lista_final)
{
    lista_final.clear();

    list<int>::const_iterator it = lista_inicial.cbegin();
    list<int>::const_reverse_iterator rit = lista_inicial.crbegin();

    while(it != lista_inicial.cend() && rit != lista_inicial.crend())
    {
        lista_final.push_back(*it);
        lista_final.push_back(*rit);

        ++it;
        ++rit;
    }
}

void subsecuencia(list<int> & L)
{
    list<int>::iterator it = L.begin();

    while(it != L.end())
    {
        list<int>::iterator next = it;
        ++next;

        if(next != L.end() && *it >= *next) it = L.erase(next);
        else ++it;
    }
}

void intercambia(list<int> & L)
{
    list<int>::iterator impar, par = L.begin();
    ++par;

    while(impar != L.end() && par != L.end())
    {
        swap(*impar, *par);
        ++impar; ++impar;
        ++par; ++par;
    }
}

list<list<int>> construye_lista(list<int> & L)
{
    list<int> aux;
    list<list<int>> resultado;
    list<int>::iterator it = L.begin();

    L.sort();
    int actual = *it;
    aux.push_back(actual); ++it;

    while (it != L.end())
    {
        if (*it == actual) aux.push_back(*it);
        else
        {
            resultado.push_back(aux);
            aux.clear();
            actual = *it;
            aux.push_back(actual);
        }

        ++it;
    }

    resultado.push_back(aux);
    return resultado;
}

ostream &operator<<(ostream & os, const list<int> &L){
    for (auto it=L.cbegin(); it!=L.cend();++it)
            os<<*it<<" ";
    os<<endl;
    return os;
}
// Imprime los elementos comprendidos entre dos iteratores
template <class T>
void Imprime(T first, T second ){
    for (auto it=first; it!=second;++it){
      for_each(it->begin(),it->end(),[](int a){ cout<<a<<" ";});
      cout<<endl;
    }
}

bool operator == (const list<int> & e1, const list<int> &e2){
    if (e1.size()!=e2.size()) return false;
	for (auto it1 = e1.cbegin(),it2=e2.cbegin(); it1!=e1.cend();++it1,++it2)
        if ((*it1)!=(*it2)) return false;
    return true;
}


// Simplemente comprueba que la solución para "in" es correcta
void test_sol(list<int> in,list<int> sol){
	cout << "Solución correcta?: " << (in==sol ? "YES" : "NO") << endl;
}


//Ejemplo
int main() {
    list<int> L = {5, 4, 5, 8, 6, 2, 10, 9, 3, 1};
    list<int> subl;
    
    cout << "________________________________________________________________________________________" << endl;
    cout << endl;
    cout << "** EJEMPLO 1 - Encontrar la sublista máxima de números pares consecutivos:" << endl;
    cout << "Lista original: ";
    cout << L;
    cout << "Sublista máxima obtenida: ";    
    max_sublist(L, subl);
    cout << subl;
    test_sol(subl, {8,6,2,10});

    cout << "________________________________________________________________________________________" << endl;
    cout << endl;


    cout << "** EJEMPLO 2 - Comprobar que cuando hay varias sublistas máximas, se retorna la primera:" << endl;
    L = {1,2,4,6,5,8,2,4};
    subl.clear();
    max_sublist(L, subl);
    cout << "Lista original: ";
    cout << L;
    cout << "Sublista máxima: ";
    cout << subl;
    test_sol(subl, {2,4,6});
    cout << "________________________________________________________________________________________" << endl;
    cout << endl;
    return 0;
}