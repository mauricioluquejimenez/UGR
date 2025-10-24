
#include <iostream>
#include <ctime>    // Para función time()
#include <cstdlib>  // Para números aleatorios
#include <cstring>

#include "tablero.h"
#include "jugador.h"
#include "juego.h"
#include "conexion.h"
#include <QApplication>
#include "ventana.h"
#include <sstream>
using namespace std;

/* ***************************************** */

// Preguntamos por teclado los datos de un jugador y lo devolvemos
Jugador LeeJugador(Consola * con )
{
  string nom;
  int n;
  ostringstream os;
  string txt ="";
  txt+="Dime el nombre del jugador\n";
  con->WriteText(txt);
  nom=con->ReadString();
  
  return Jugador(nom);
}

/* ***************************************** */

int main(int argc, char *argv[])
{
  char p;

  Jugador j1;



  QApplication a(argc, argv);
  srand(time(0));  // Inicializamos el generador de números aleatorios

  //suponemos que el tablero va a ser un 9,9
  TableroGrafico *tg = new TableroGrafico(500,700,9,9);
  Consola *con =new Consola();
  Ventana v(tg,con,"Buscaminas");
  v.show();
  j1=LeeJugador(con);
   
  
  
  // Creamos un juego usando un tablero 9x9 filas y columnas con 10 minas y un jugador
  JuegoBuscaminas juego(Tablero(9,9,10),j1);
  juego.setTableroGrafico(tg); //asignamos el tablerografico

  do {
    con->WriteText("El jugador es: ");
    string aux = " ";
    aux = aux+juego.ElJugador().Nombre();
    con->WriteText(aux);
   
    aux="\n Para destapar una casilla en el tablero\nhaz DOBLE-CLICK DERECHO ";
    con->WriteText(aux);    
    aux ="\n Para poner una bandera en una casilla \nhaz CTRL +DOBLE-CLICK DERECHO";
    con->WriteText(aux);    
    con->WriteText("Comenzamos!!");
    juego.NuevoJuego();    // Comenzamos el juego
    bool estalla=false;
    int inir,inic;
    juego.getIniCasilla(inir,inic);
    aux = "La casilla inicial segura es "+to_string(inir)+" "+to_string(inic);
    con->WriteText(aux);
    cout<<juego.ElTablero()<<endl;
    int totalMinas = juego.ElTablero().numMinas();
    int totalClose = juego.ElTablero().numClose();
    do {
      aux ="Pulsa casilla : ";
      aux +=juego.ElJugador().Nombre()+"\n" ;
      con->WriteText(aux );
      estalla =juego.JugarTurno();    // Avanzamos turno
      if (estalla){
        aux =" Has pisado una mina !!! ";
        con->WriteText(aux);
        aux = "¿Quieres ver el tablero ?(S/N)";
        con->WriteText(aux);
        p=con->ReadChar();
	if ((p=='s') || (p=='S'))
		juego.ElTablero().drawAll();
      }
      else{
        totalClose = juego.ElTablero().numClose();
        aux =" Numero minas "+to_string(totalMinas)+"\n";
        con->WriteText(aux);
        aux =" Numero cerradas "+to_string(totalClose)+"\n";
        con->WriteText(aux);
        
      }	
      cout<<juego.ElTablero()<<endl;
      
    } while (estalla ==false && !juego.HemosAcabado());    // Comprobamos si hemos acabado
	
    con->WriteText("¿Otra partida (S/N)?");
    p=con->ReadChar();
    if ((p=='s') || (p=='S'))
       juego.NuevoJuego(); 
	

  } while ((p=='s') || (p=='S'));
  

  v.cerrar();
  return a.exec();

}
