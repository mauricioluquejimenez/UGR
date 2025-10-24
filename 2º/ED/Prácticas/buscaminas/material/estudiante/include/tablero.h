
#ifndef __TABLERO__H__
#define __TABLERO__H__

/* ***************************************** */

#include <iostream>
#include "tablerografico.h"
#include <map>
/* ***************************************** */

/**
  @brief TDA Tablero juego buscaminas
  @author Rosa Rodríguez-Sanchez
  @date 3/10/2025
**/
class Tablero {

  private:
    int **tab;      //-1 hay mina 0 no
    bool ** cerradas; //tablero indicando si la casilla esta abierta o cerrada
    bool **marcadas ; //tablero para poner banderas en las casillas.True puesta
    		       //false no puesta
    int ** entorno ; //numero de minas en el entorno de cada casilla
    int numminas;  // Número de  minas
    int numfilas;  // Número de filas
    int numcols;           //Numero de columnas del tablero
    TableroGrafico *TG;   //puntero al tablero grafico
    int inicio_row,inicio_col; //Fila y Columna para iniciar el juego
    // Copia un tablero desde orig
    void copia_tableros(const Tablero &orig);

    //borra la memoria de los tableros
    void borrar_tableros();
    //contabiliza el numero de bombas que hay en el entorno de f,c
    void contabilizaEntorno(int f,int c);
    //inicia la matriz entorno. Si no es mina contabiliza los vecinos que son
    //mina
    void iniciaEntorno();


  public:
    Tablero();                                  // Constructor por defecto
    Tablero(int filas, int cols,int nminas);                                 // Constructor por parámetros filas columnas y numero de minas. Las coloca aleatoriamente. cerradas se pone todo a true
    ~Tablero() {borrar_tableros(); };                             // Destructor (vacío)

    Tablero(const Tablero &orig);               // Constructor de copia
    Tablero& operator=(const Tablero &orig);    // Sobrecarga de asignación

    // Inicializa los tableros. POne aleatoriamente las minas y pone
    // las casillas de cerrada a true
    void iniciaTableros();

    // Destapa una casilla
    // Si es una casilla vacia se destapa las casillas del alrededor
    // Si es una casilla abierta no se puede destapar y devuelve fase
    // Si es una casilla con mina estalla se pone a true. False en caso contrario

    bool destapaCasilla(int f, int c, bool &estalla);

    //Para las casillas en el entorno de una dada (f,c) muestra las
    // casillas
    bool pintaEntorno(int f, int c);

    //Descubre todas las casillas no minadas en el entorno de la casilla (f,c)

    void abreEntorno(int f, int c);

    // Colocal una bandera en la fila f y columna c
    // Si puede poner la bandera porque es una casilla no abierta devuelve true
    // En caso contrario false.
    bool ponerBandera(int f,int c);


    //Devuelve el numero de minas

    int numMinas()const{return numminas;}

    // Devuelve el numero de casillas abiertas
    int numOpen()const;

    //Devuelve el numero de casillas cerradas
    int numClose()const;

    //True si el jugador ha descubierto todas las minas. False en caso contrario
    bool terminado()const;

    //Devuelve la fila de inicio
    int getInicioRow()const{ return inicio_row;}

    //Devuelve la columna de inicio
    int getInicioCol()const{ return inicio_col;}

    //devuelve el numero filas
    int getFilas()const{return numfilas;}

    //devuelve el numero columnas
    int getCols()const{ return numcols;}

    //Obtiene la casilla pulsada.Devuelve True si ha conseguido una fila y columna. False si se ha cerrado con X. Ademas en button recoge si se ha pulsado
    //el boton derecho True o izquierdo False.
    void getCasillaElegida(int &fil,int &col,bool &button){
        TG->getCasillaElegida(fil,col,button);
    }
    void setTableroGrafico(TableroGrafico *tg){
        TG=tg;
    }

    //Dibuja el tablero grafico entero
    void Redraw();

    //Dibueja el tablero grafico inicial
    void RedrawInicio();

    //Dibueja todo el tablero
    void drawAll()const;

    //Dibuja una imagen bobam en el tablero en la casilla dada por f y c
    void pintaBomba(int f, int c);
    //salida
    friend
    ostream & operator<<(ostream &os, const Tablero & T);


};

// Para mostrar el tablero en pantalla sobrecargamos <<
std::ostream& operator<<(std::ostream &salida, const Tablero &tab);

/* ***************************************** */

#endif
