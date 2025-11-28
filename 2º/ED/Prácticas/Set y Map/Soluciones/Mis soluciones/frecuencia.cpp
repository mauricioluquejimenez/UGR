#include <iostream>
#include <set>
#include <map>

using namespace std;

map<int, int> frecuencia(const multiset<int> & ms)
{
    map<int, int> resultado;
    multiset<int>::const_iterator it = ms.cbegin();

    while(it != ms.cend())
    {
        resultado[*it] = ms.count(*it);
        ++it;
    }

    return resultado;
}

int main(){
 multiset<int> MS({1,1,1,1,3,3,3,2,2,6,6,6}); // en el multiset se colocaran ordenados

 map<int,int> mimap = frecuencia(MS);
 for (auto it=mimap.begin();it!=mimap.end();++it)
    cout<<it->first<<" "<<it->second<<endl;

}