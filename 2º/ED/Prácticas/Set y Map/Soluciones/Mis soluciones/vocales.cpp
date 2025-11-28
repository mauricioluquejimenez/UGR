#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

map<string, int> vocales(const set<string> & s)
{
    map<string, int> resultado;
    set<string>::const_iterator it = s.cbegin();

    while(it != s.cend())
    {
        // Se inicializa un contador de vocales para cada palabra
        int contador = 0;
        for(char c : *it)
        {
            if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
               c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U')
            {
                // Si alguno de los caracteres de una palabra es una vocal, se incrementa el contador
                contador++;
            }
        }

        // Una vez se ha recorrido la palabra completa y se han contado sus vocales,
        // se inserta el par en el mapa
        
        // resultado[*i] devuelve un int asociado a la clave string,
        // de manera que se asigna el contador mediante =
        resultado[*it] = contador;
        ++it;
    }

    return resultado;
}

int main(){

    set<string> mis({"aguila", "alondra", "buho","buitre", "canario",
                    "gavilan", "gaviota", "halcon"});

    map<string ,int> mim = vocales(mis);

    for (auto it = mim.begin();it!=mim.end();++it)
        cout<<it->first<<" "<<it->second<<endl;
}