
#include <cassert>
#include <cstdlib>
#include "jugador.h"

using namespace std;

/* ***************************************** */
Jugador::Jugador(){
     nombre="";
     fic=CRUZ;
     nivel=0;
     tipo='L';
     ip="";
}
/* ***************************************** */
Jugador::Jugador(const std::string &n, const Ficha &f, int ni, char tip, std::string direc)
                 : nombre(n), fic(f), nivel(ni), tipo(tip), ip(direc)
{
  assert((nivel>=0)&&(nivel<2));   // Comprobamos que el nivel es correcto
}

/* ***************************************** */

Jugador& Jugador::operator=(const Jugador &orig)
{
  if (this!=&orig){
    this->nombre=orig.nombre;
    this->fic=orig.fic;
    this->nivel=orig.nivel;
    this->tipo=orig.tipo;
    this->ip=orig.ip;
  }
  return *this;
}

/* ***************************************** */

std::ostream& operator<<(std::ostream &salida, const Jugador &jug)
{
  salida <<jug.Nombre()<<"  ("<<jug.Color()<<")"<<" ("<<jug.gettipo()<<")" 
         <<" ("<<jug.getip()<<")";
  return salida;
}

/* ***************************************** */

void Jugador::PiensaJugada( Tablero &tab, int &fil, int &col) const
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

void Jugador::piensa_nivel_0(Tablero &tab, int &fil, int &col) const
{

  fil =-1;
  col =-1;

  do {

    tab.getCasillaElegida(fil,col);
  } while ( (fil<0)||(fil>2)||(col<0)||(col>2));

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
