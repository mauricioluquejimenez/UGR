
#include <iostream>
#include <ctime>    // Para función time()
#include <cstdlib>  // Para números aleatorios
#include "ficha.h"
#include "tablero.h"
#include "jugador.h"
#include "juego.h"

using namespace std;

/* ***************************************** */

// Preguntamos por teclado los datos de un jugador y lo devolvemos
Jugador LeeJugador(const Ficha f)
{
  string nom;
  int n;
  cout << "Dime el nombre del jugador "<< f <<" : ";
  cin >> nom;
  cout << "  Dime de que nivel es (0=humano, 1=aleatorio)";
  cin >> n;
  return Jugador(nom,f,n);
}

/* ***************************************** */

int main(int argc, char *argv[])
{
  char p;

  srand(time(0));  // Inicializamos el generador de números aleatorios

  // Creamos un juego usando un tablero y dos jugadores leidos por teclado
  Juego3Raya juego(Tablero(),LeeJugador(CRUZ),LeeJugador(CIRCULO));

  // También se podría hacer de esta otra forma:
  //   Jugador j1=LeeJugador(cruz);       // Creamos los jugadores
  //   Jugador j2=LeeJugador(circulo);
  //   Tablero tab;                       // Creamos un tablero
  //   Juego3Raya juego(tab,j1,j2);       // Creamos el juego

  do {

    cout << "Los jugadores son: " << endl;
    cout << "  " << juego.ElJugador(0) << endl;
    cout << "  " << juego.ElJugador(1) << endl;
    cout << "Comenzamos!!!" << endl << endl;

    juego.NuevoJuego();    // Comenzamos el juego
    do {
      cout << "Le toca jugar a : " << juego.AQuienLeToca() << endl;
      juego.JugarTurno();    // Avanzamos turno
      cout << "Tras poner la ficha, el tablero queda así: " << endl
           << juego.ElTablero() << endl;
    } while (!juego.HemosAcabado());    // Comprobamos si hemos acabado

    cout << "Se acabó la partida !!!" << endl;

    int ganador=juego.QuienGana();   // Consultamos quien ganó
    if (ganador==-1)
      cout << "Hubo empate" << endl;
    else
      cout << "El ganador ha sido: " << juego.ElJugador(ganador) << endl;

    cout << "¿Otra partida (S/N)?";
    cin >> p;

  } while ((p=='s') || (p=='S'));
}
