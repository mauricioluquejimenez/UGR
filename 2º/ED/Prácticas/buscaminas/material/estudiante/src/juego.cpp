#include <cassert>
#include "juego.h"

using namespace std;

/* ***************************************** */

JuegoBuscaminas::JuegoBuscaminas(const Tablero &t, const Jugador &j1)
       : jug1(j1),tab(t)
{
}

/* ***************************************** */

void JuegoBuscaminas::NuevoJuego()
{
  tab.iniciaTableros();
  tab.RedrawInicio();
}

/* ***************************************** */

bool JuegoBuscaminas::JugarTurno()
{
  int f,c;
  

  // Este vector de punteros se usa para evitar usar un if dentro del bucle

  // Preguntamos al jugador mientras su jugada no sea válida
  bool estalla=false;
  bool button;
  bool correct=false;
  do {
         
           jug1.PiensaJugada(tab,f,c,button);
           if (button ==true) //boton derecho
             correct = tab.destapaCasilla(f,c,estalla);
           else
             correct = tab.ponerBandera(f,c);  
  } while (!correct);
  if (estalla)
  	tab.pintaBomba(f,c);
  
  return estalla;	
}

/* ***************************************** */

bool JuegoBuscaminas::HemosAcabado() const
{
  return (tab.terminado());
}

/* ***************************************** */

const Jugador & JuegoBuscaminas::ElJugador() const
{
  return jug1;
}

/* ***************************************** */
void JuegoBuscaminas::getIniCasilla(int & inir,int &inic)const{
	inir = tab.getInicioRow();
	inic = tab.getInicioCol();
}	



