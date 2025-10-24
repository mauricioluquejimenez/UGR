#ifndef __JUEGO__H__
#define __JUEGO__H__

/* ***************************************** */

#include "tablero.h"
#include "jugador.h"
#include "conexion.h"
#include "tablerografico.h"
/* ***************************************** */
/**
  @brief TDA JuegoBuscaminas
  @author Rosa Rodríguez-Sanchez
  @date 3/10/2025
**/
class JuegoBuscaminas {
  private:
    Jugador jug1;    // Jugador
    Tablero tab;           // Tablero
    TableroGrafico *tg;
  public:
    // No existe constructor por defecto
    // Contructor. Para crear un nuevo juego hemos de dar un tablero
    // y dos jugadores obligatoriamente
    JuegoBuscaminas(const Tablero &t, const Jugador &j1);
    ~JuegoBuscaminas()  { };     // Destructor vac o

    void NuevoJuego();      // Prepara el juego para comenzar una nueva partida
    
    // Le pide al jugador que escoja una casilla. Devuelve true si ha escogido 
    // una casilla con mina false en caso contrario
    bool JugarTurno();

    // Devuelve una referencia (const) al tablero de juego (consultor)
    const Tablero &ElTablero() const { return tab; };

    // Devuelve una referencia al jugador n- simo (n=0   1)
    const Jugador &ElJugador() const;

    // Devuelve true si el juego ha terminado (porque se han descubierto las minas) o la estallado una mina
    bool HemosAcabado() const;


    void getIniCasilla(int & inir,int &inic)const;
    	

    //Le asigna el tablero grafico donde se representa el juego.
    void setTableroGrafico(TableroGrafico * tg){
        tab.setTableroGrafico(tg);
    }


};

/* ***************************************** */

#endif
