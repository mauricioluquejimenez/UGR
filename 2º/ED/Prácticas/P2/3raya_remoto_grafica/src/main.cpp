
#include <iostream>
#include <ctime>    // Para función time()
#include <cstdlib>  // Para números aleatorios
#include <cstring>
#include "ficha.h"
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
Jugador LeeJugador(const Ficha f, char mod, string direc,Consola * con )
{
  string nom;
  int n;
  ostringstream os;
  //os<<f;
  if (f==CIRCULO)
      con->WriteText("FICHA : O\n");
  else
      con->WriteText("FICHA : X\n");
  if (mod=='L'){
      string txt=string("******* JUGADOR LOCAL *******\n")+
                       "Dime el nombre del jugador " +os.str()+" :\n ";
      con->WriteText(txt);
      nom= con->ReadString();
      txt= "Dime de que nivel es (0=humano, 1=aleatorio)\n";
      con->WriteText(txt);
      n=con->ReadInt();
  }
  else{

      string txt="******* JUGADOR REMOTO *******\n";
      txt+="Dime la ip del ordenador donde esta el Jugador Remoto\n";
      con->WriteText(txt);
      direc=con->ReadString();

      n=0;
  }
  
  return Jugador(nom,f,n,mod,direc);
}

/* ***************************************** */

int main(int argc, char *argv[])
{
  char p;
  char modalidad='k';
  char papel='N';
  Jugador j1;
  Jugador j2;
  Conexion conexion;

  char msg[50];
  QApplication a(argc, argv);
  srand(time(0));  // Inicializamos el generador de números aleatorios


  TableroGrafico *tg = new TableroGrafico(400,400,3,3);
  Consola *con =new Consola();
  Ventana v(tg,con,"3Raya");
  v.show();
  do{

     string txt = string("Modalida de Juego\n ") +
                  "Juego (L)ocal. Cuando los dos jugadores estan en el mismo ordenador\n"+
                  "\nIntroduce L o R ";
     con->WriteText(txt);

     modalidad=con->ReadChar();
     modalidad = toupper(modalidad);
  }while ((modalidad != 'L') && (modalidad != 'R'));
  
  if (modalidad=='R' ){
      do{

         string txt="\nQue papel quieres desempeñar\n";
         txt+="  (S)erver o (C)liente\n";
         txt+="  Cada jugador debe elegir uno de estos papeles no afecta al juego\n\n";
         txt+="Introduce S o C";
         con->WriteText(txt);
         papel=con->ReadChar();
         papel = toupper(papel);
      }while((papel != 'S') && (papel != 'C') );
      if (papel=='S'){                              //Server
         j1=LeeJugador(CIRCULO,'R',"",con);
         j2=LeeJugador(CRUZ,'L',"",con);
         conexion.Crear_Servidor();
         conexion.Aceptar_Cliente();
         conexion.Recibir_Msg(msg);
         j1.setNombre(&msg[1]);
         j1.setnivel(msg[0]-'0');        
         msg[0]='0'+j2.getnivel();
         msg[1]='\0';
         strcat(msg,j2.Nombre().c_str());
         conexion.Enviar_Msg(msg);

      }else {                                         //Cliente

         j1=LeeJugador(CIRCULO,'L',"",con);
         std::cout<<"Leido jugador local"<<endl;
         j2=LeeJugador(CRUZ,'R',"",con);
         std::cout<<"Leido jugador remoto"<<endl;
         conexion.Establecer_Conexion(j2.getip().c_str());
         msg[0]='0'+j1.getnivel();
         msg[1]='\0';
         strcat(msg,j1.Nombre().c_str());
         conexion.Enviar_Msg(msg);
         conexion.Recibir_Msg(msg);
         j2.setNombre(&msg[1]);
         j2.setnivel(msg[0]-'0');

      } 
  }
  else { //Modalidad de juego local los dos juagadores son locales
     j1=LeeJugador(CRUZ,modalidad,"",con);
     j2=LeeJugador(CIRCULO,modalidad,"",con);
  }
  
  // Creamos un juego usando un tablero y dos jugadores leidos por teclado  
  Juego3Raya juego(Tablero(),j1,j2,modalidad,conexion);
  juego.setTableroGrafico(tg); //asignamos el tablerografico
  juego.setFilesFichas();//Inicializa las imagenes de las fichas
  // También se podría hacer de esta otra forma:
  //   Jugador j1=LeeJugador(CRUZ);       // Creamos los jugadores
  //   Jugador j2=LeeJugador(CIRCULO);
  //   Tablero tab;                       // Creamos un tablero
  //   Juego3Raya juego(tab,j1,j2);       // Creamos el juego


  do {
    con->WriteText("Los jugadores son: ");
    string aux = " ";
    aux = aux+juego.ElJugador(0).Nombre();
    con->WriteText(aux);
    aux = " ";
    aux = aux+juego.ElJugador(1).Nombre();
    con->WriteText(aux);
    aux="\n Para escoger casilla en el tablero \nHAZ DOBLE-CLICK DERECHO en el RATON\n";
    con->WriteText(aux);
    con->WriteText("Comenzamos!!");
    juego.NuevoJuego();    // Comenzamos el juego
    do {
      aux ="Le toca jugar a : ";
      aux +=juego.ElJugador(juego.AQuienLeToca()).Nombre()+"\n" ;
      con->WriteText(aux );
      juego.JugarTurno();    // Avanzamos turno

    } while (!juego.HemosAcabado());    // Comprobamos si hemos acabado

    con->WriteText("Se acabó la partida !!!");

    int ganador=juego.QuienGana();   // Consultamos quien ganó
    if (ganador==-1)
        con->WriteText("Hubo empate" );
    else{
        aux = "El ganador ha sido: ";
        aux +=juego.ElJugador(ganador).Nombre();
        con->WriteText(aux);
    }
    con->WriteText("¿Otra partida (S/N)?");
    p=con->ReadChar();


  } while ((p=='s') || (p=='S'));
  
  if(modalidad == 'R') 
     conexion.Cerrar_Conexion();
  v.cerrar();
  return a.exec();

}
