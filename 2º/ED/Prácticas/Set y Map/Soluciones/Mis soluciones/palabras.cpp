#include <iostream>
#include <list>
#include <map>
#include <set>

using namespace std;

set<string> palabras(const map<string, list<pair<int, int>>> & m, int pag)
{
    set<string> resultado;

    map<string, list<pair<int, int>>>::const_iterator it = m.cbegin();
    list<pair<int, int>>::const_iterator itList;

    while(it != m.cend())
    {
        itList = it->second.cbegin();
        while(itList != it->second.cend())
        {
            if(itList->first == pag) resultado.insert(it->first);
            ++itList;
        }
        ++it;
    }

    return resultado;
}

int main()
{
    map<string,list<pair<int,int> > > mimap({{"Informatica",{{1,20},{3,10}}},
                                            {"Robot",{{3,20},{10,15},{4,1}}} ,{"Sistema",{{2,10},{10,5}}}});
    set<string> miset= palabras(mimap,10);

    for (auto it = miset.begin(); it!=miset.end();++it)
        cout<<*it<<endl;
}