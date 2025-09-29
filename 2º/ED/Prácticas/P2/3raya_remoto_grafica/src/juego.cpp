#include <cassert>
#include "juego.h"

using namespace std;

/* ***************************************** */

Juego3Raya::Juego3Raya(const Tablero &t, const Jugador &j1, const Jugador &j2,
                       char mod, Conexion con)
       : jug1(j1), jug2(j2), tab(t), turno(0), modalidad(mod), conex(con)
{
}

/* ***************************************** */

void Juego3Raya::NuevoJuego()
{
  turno = (turno+1) % 2;  // Al comenzar un nuevo juego hacemos que comience
                          // a jugar el que perdió en la partida anterior
  tab.PonerEnBlanco();
}

/* ***************************************** */

void Juego3Raya::JugarTurno()
{
  int f,c;
  Jugador *jug[2] = {&jug1, &jug2};    // Vector de punteros a los jugadores
  char msg[20];
  // Este vector de punteros se usa para evitar usar un if dentro del bucle

  // Preguntamos al jugador mientras su jugada no sea válida
  do {
       if((jug[turno] -> gettipo()) == 'L')  
           jug[turno]->PiensaJugada(tab,f,c);
       else{
           cout << "Esperamos al jugador Remoto" << endl;
           conex.Recibir_Msg(msg);
           f=msg[0]-'0';
           c=msg[1]-'0';
       }    
  } while (!tab.PonFicha(f,c,jug[turno]->Color()));
  
  if((modalidad == 'R') && (jug[turno] -> gettipo()) == 'L'){  
        msg[0]=f+'0';
        msg[1]=c+'0';
        msg[2]='\0';
        conex.Enviar_Msg(msg);

  }

  turno = (turno+1) % 2;  // Avanzamos para el siguiente turno
}

/* ***************************************** */

bool Juego3Raya::HemosAcabado() const
{
  return ((tab.CuantasFichas()==9) || (tab.Busca3Raya()!=BLANCO));
}

/* ***************************************** */

const Jugador & Juego3Raya::ElJugador(int n) const
{
  assert((n==0)||(n==1));
  return ((n==0) ? jug1 : jug2);
}

/* ***************************************** */

int Juego3Raya::QuienGana() const
{
  Ficha g = tab.Busca3Raya();
  if (g==jug1.Color())
    return 0;
  else if (g==jug2.Color())
    return 1;
  return -1;    // No gana nadie
}

void Juego3Raya::setFilesFichas(){
    Ficha allfichas[3]={BLANCO,CIRCULO,CRUZ};
    string im_fichas[]={"./images/B.png","./images/O.png","./images/X.png"};
    tab.setFilesFichas(im_fichas,allfichas,3);
}
