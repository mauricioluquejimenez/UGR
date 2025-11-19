#include <fstream>
#include <iostream>

#include "dictionary.h"

using namespace std;

int main(int argc, char *argv[])
{
  if(argc != 2){
    cout << "Los parametros son: " << endl;
    cout << "1.- El fichero con el diccionario" << endl;
    return 1;
  }

  ifstream f_diccionario(argv[1]);

  if(!f_diccionario){
    cout << "No puedo abrir el fichero " << argv[1] << endl;
    return 0;
  }

  Dictionary diccionario;
  f_diccionario >> diccionario;

  for (Dictionary::iterator it = diccionario.begin(); it != diccionario.end(); ++it){
    cout << *it << endl;
  }
  cout<<endl;
  cout<<"Numero de palabras en el diccionario "<< diccionario.size()<<endl;

  //Buscamos todas las palabras con prefijo zurr
  string prefijo = "zurr";

  pair<Dictionary::iterator, Dictionary::iterator >ret = diccionario.range_prefix(prefijo);

  for (Dictionary::iterator it = ret.first; it != ret.second; ++it){
        cout<<*it<<endl;
  }

  //Borando una palabra
  string w = "zurrusco";
  if (diccionario.erase(w)){
    cout<<"Palabra borrada "<<diccionario.size()<<endl;
  }
  else
    cout<<"Palabra no borrada "<<diccionario.size()<<endl;

  int longitud;
  cout<<"Dime la longitud de las palabras que  quieres ver";
  cin>>longitud;
  vector<string> v=diccionario.getWordsLength(longitud);
  for (int i=0;i<v.size();i++)
     cout<<v[i]<<endl;

  string input;
  cout<<"Dime una palabra ";
  cin>>input;
  if (diccionario.find(input)!=diccionario.end())
        cout<<"La palabra esta"<<endl;
  else
        cout<<"La palabra no esta"<<endl;


  return 0;
}
