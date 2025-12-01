#include <iostream>
#include <map>
#include <list>
#include <string>

using namespace std;

map<string, list<int>> mapa(const multimap<string,int> & multi)
{
    map<string, list<int>> resultado;
    multimap<string, int>::const_iterator it = multi.cbegin();

    while(it != multi.cend())
    {
        // Se inserta el valor en la lista asociada a la clave
        // Si la clave no existe, el mapa crea una nueva entrada con una lista vacía

        // resultado[*i] devuelve un list<int> asociado a la clave string,
        // de manera que se asigna el contador mediante el método push_back propio de list
        resultado[it->first].push_back(it->second);

        ++it;
    }

    return resultado;
}

int main(){
    multimap<string, int> mim({{"ahora",1},{"ahora",3},{"ahora",4},{"aunque",3},
                            {"aunque",5},{"banco",2},{"banco",10},
                            {"zanahoria",1},{"zanahoria",2}});
    map<string,list<int> > mimap=mapa(mim);

    map<string,list<int> >::iterator it;

    cout << mimap.size() << endl;
    for (it=mimap.begin(); it!=mimap.end();++it)
    {
        cout<<"Clave: '"<<it->first<<"', "<< " Lista asoc.: ";
        for (auto it_l = it->second.begin(); it_l!=it->second.end();++it_l)
            cout<<*it_l<<" ";
        cout<<endl;
   }
}