
#include <cassert>
#include <cstdlib>
#include "jugador.h"

using namespace std;

/* ***************************************** */
Jugador::Jugador(){
     nombre="";
}
/* ***************************************** */
Jugador::Jugador(const std::string &n)
                 : nombre(n) 
{
  
}

/* ***************************************** */

Jugador& Jugador::operator=(const Jugador &orig)
{
  if (this!=&orig){
    this->nombre=orig.nombre;
    
  }
  return *this;
}

/* ***************************************** */

std::ostream& operator<<(std::ostream &salida, const Jugador &jug)
{
  salida <<jug.Nombre()<<endl;
  return salida;
}

/* ***************************************** */

void Jugador::PiensaJugada( Tablero &tab, int &fil, int &col,bool &button) const
{
  // En función del nivel del jugador elegimos una estrategia u otra
  piensa_nivel_0(tab,fil,col,button);

     

}

/* ***************************************** */

void Jugador::piensa_nivel_0(Tablero &tab, int &fil, int &col,bool  & b) const
{

  fil =-1;
  col =-1;

  do {

    tab.getCasillaElegida(fil,col,b);
  } while ( (fil<0)||(fil>tab.getFilas())||(col<0)||(col>tab.getCols()));

}


