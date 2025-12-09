#include <fstream>
#include <iostream>
#include <cmath>
#include <limits>
#include "dictionary.h"
#include "letters_set.h"


using namespace std;

int main(int argc, char *argv[])
{
  if(argc != 4){
    cout << "Los parametros son: " << endl;
    cout << "1.- El fichero con el diccionario" << endl;
    cout << "2.- El fichero con las letras" << endl;
    cout << "3.- El fichero de salida"<<endl;
    return 0;
  }

  ifstream dic_file(argv[1]);
  if (!dic_file)
  {
    cerr << "No puedo abrir el fichero de diccionario " << argv[1] << endl;
    return 1;
  }

  ifstream letters_file(argv[2]);
  if (!letters_file)
  {
    cerr << "No puedo abrir el fichero de letras " << argv[2] << endl;
    return 1;
  }

  ofstream salida(argv[3]);
  if (!salida)
  {
        cerr << "No puedo abrir el fichero de salida " << argv[3] << endl;
        return 1;
  }

  LettersSet set; letters_file >> set;
  Dictionary dic; dic_file >> dic;

  char letra;
  double min = numeric_limits<double>::max();
  double porcentaje = 0.0;
  double min_log = 0.0;
  double peso = 0.0;
  int apariciones = 0;
  int total = dic.getTotalLetters();

  for(LettersSet::const_iterator it = set.cbegin(); it != set.cend(); ++it)
  {
    letra = (*it).first;
    apariciones = dic.getOcurrences(letra);

    porcentaje = (total > 0) ? static_cast<double>(apariciones) / total : 0.0;
    if(porcentaje > 0.0 && porcentaje < min) min = porcentaje;
  }

  min_log = -log10(min);

  for(LettersSet::const_iterator it = set.cbegin(); it != set.cend(); ++it)
  {
    letra = (*it).first;
    apariciones = dic.getOcurrences(letra);

    porcentaje = (total > 0) ? static_cast<double>(apariciones) / total : 0.0;
    peso = (porcentaje > 0.0) ? 10 * (-log10(porcentaje) / min_log) : 0.0;

    salida << "\nLetra: " << letra << " Apariciones: " << apariciones << " Peso: " << peso;
  }

  return 0;
}