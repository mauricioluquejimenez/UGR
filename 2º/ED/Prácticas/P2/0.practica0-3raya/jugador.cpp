
#include <cassert>
#include <cstdlib>
#include "jugador.h"

using namespace std;

/* ***************************************** */

Jugador::Jugador(const std::string &n, const Ficha &f, int ni)
                 : nombre(n), fic(f), nivel(ni)
{
  assert((nivel>=0)&&(nivel<2));   // Comprobamos que el nivel es correcto
}

/* ***************************************** */

std::ostream& operator<<(std::ostream &salida, const Jugador &jug)
{
  salida <<jug.Nombre()<<"  ("<<jug.Color()<<")";
  return salida;
}

/* ***************************************** */

void Jugador::PiensaJugada(const Tablero &tab, int &fil, int &col) const
{
  // En función del nivel del jugador elegimos una estrategia u otra
  switch (nivel) {
    case 0: piensa_nivel_0(tab,fil,col);
            break;
    case 1: piensa_nivel_1(tab,fil,col);
            break;
  }
}

/* ***************************************** */

void Jugador::piensa_nivel_0(const Tablero &tab, int &fil, int &col) const
{
  cout << " El tablero es: " << endl << tab;
  do {
    cout << " ¿Donde pones ficha (dime fila y columna)? : ";
    cin >> fil >> col;
  } while ((fil<0)||(fil>2)||(col<0)||(col>2));
}

/* ***************************************** */

void Jugador::piensa_nivel_1(const Tablero &tab, int &fil, int &col) const
{
  cout << " ... estoy pensando ... " << endl;
  do {
    fil = rand()%3;
    col = rand()%3;
  } while (tab.QueFichaHay(fil,col)!=BLANCO);
  cout << "      ... y pongo ficha en (" << fil << "," << col << ")" << endl;
}

/* ***************************************** */
