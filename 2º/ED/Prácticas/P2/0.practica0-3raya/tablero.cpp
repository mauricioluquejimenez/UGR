
#include <cassert>
#include "tablero.h"

using namespace std;

/* ***************************************** */

Tablero::Tablero()
{
  // El constructor pone en blanco el tablero
  PonerEnBlanco();
}

/* ***************************************** */

Tablero::Tablero(const Tablero &orig)
{
  copia_tablero(orig);
}

/* ***************************************** */

void Tablero::PonerEnBlanco()
{
  // Ponemos en blanco el tablero
  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++)
      tab[i][j] = BLANCO;
  numfichas=0;
}

/* ***************************************** */

Tablero& Tablero::operator=(const Tablero &orig)
{
  if (this!=&orig)
    copia_tablero(orig);
  return *this;
}

/* ***************************************** */

bool Tablero::PonFicha(int f, int c, const Ficha &fic)
{
  assert((f>=0) && (f<3) && (c>=0) && (c<3));    // Estamos dentro del tablero
  if (tab[f][c]==BLANCO) {                       // No hay ficha en esa casilla
    tab[f][c] = fic;
    numfichas++;
    return true;
  }
  return false;    // Ya hay ficha en esa casilla
}

/* ***************************************** */

Ficha Tablero::QueFichaHay(int f, int c) const
{
  assert((f>=0) && (f<3) && (c>=0) && (c<3));    // Estamos dentro del tablero
  return tab[f][c];
}

/* ***************************************** */

Ficha Tablero::Busca3Raya() const
{
  if (hay3raya(CIRCULO)) return CIRCULO;
  if (hay3raya(CRUZ)) return CRUZ;
  return BLANCO;    // No hay 3 en raya
}

/* ***************************************** */

ostream& operator<<(ostream &salida, const Tablero &tab)
{
  salida << "   -------------"<<endl;
  for (int i=0; i<3; i++) {
    salida<<"   |";
    for (int j=0; j<3; j++)
      salida << tab.QueFichaHay(i,j) << "|";
    salida << endl<<"   -------------"<<endl;
  }
  return salida;
}

/* ***************************************** */

// Método privado
void Tablero::copia_tablero(const Tablero &orig)
{
  for (int i=0; i<3; i++)
    for (int j=0; j<3; j++)
      tab[i][j] = orig.tab[i][j];
  numfichas=orig.numfichas;
}

/* ***************************************** */

// Método privado
bool Tablero::hay3raya(const Ficha &fic) const
{
  // Buscaremos 3 en raya en las direcciones marcadas por estos vectores
  static const int dir[4][2] = {{1,0},{1,1},{0,1},{1,-1}};

  for (int f=0; f<3; f++) {    //  Recorremos todas las casillas
    for (int c=0; c<3; c++) {  //
      if (tab[f][c]==fic) {        // Cuando encontramos una casilla con fic
        for (int d=0; d<4; d++) {  // Buscamos en las 4 direcciones 3 en raya
          int fx=f, cx=c;          //  a partir de dicha ficha
          fx+=dir[d][0];
          cx+=dir[d][1];
          int numfic=1;
          while ((fx>=0) && (fx<3) && (cx>=0) && (cx<3) && (tab[fx][cx]==fic)) {
            numfic++;
            fx+=dir[d][0];
            cx+=dir[d][1];
          }
          if (numfic==3) return true;
        }
      }
    }
  }
  return false;
}
