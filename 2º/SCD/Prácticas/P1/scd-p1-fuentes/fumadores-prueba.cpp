#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "scd.h"

using namespace std ;
using namespace scd ;

const int TAM = 3;

unsigned ingrediente_actual;
Semaphore mostrador_vacio = 1;
Semaphore exclusion_mutua = 1;
Semaphore ingredientes[TAM] = {0,0,0};



//**********************************************************************
// plantilla de función para generar un entero aleatorio uniformemente
// distribuido entre dos valores enteros, ambos incluidos
// (ambos tienen que ser dos constantes, conocidas en tiempo de compilación)
//----------------------------------------------------------------------

template< int min, int max > int aleatorio()
{
  static default_random_engine generador( (random_device())() );
  static uniform_int_distribution<int> distribucion_uniforme( min, max ) ;
  return distribucion_uniforme( generador );
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero(  )
{
  while(true){
    sem_wait(mostrador_vacio);
    ingrediente_actual = aleatorio<0,2>();
    sem_wait(exclusion_mutua);
    cout << "Estanquero produce ingrediente " << ingrediente_actual << endl;
    sem_signal(exclusion_mutua);
    sem_signal(ingredientes[ingrediente_actual]);
  }
}

//-------------------------------------------------------------------------
// Función que simula la acción de fumar, como un retardo aleatoria de la hebra

void fumar( int num_fumador )
{

   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_fumar( aleatorio<20,200>() );

   // informa de que comienza a fumar

    cout << "Fumador " << num_fumador << "  :"
          << " empieza a fumar (" << duracion_fumar.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_fumar' milisegundos
   this_thread::sleep_for( duracion_fumar );

   // informa de que ha terminado de fumar

    cout << "Fumador " << num_fumador << "  : termina de fumar, comienza espera de ingrediente." << endl;

}

//----------------------------------------------------------------------
// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( int num_fumador )
{
   while( true ){
     sem_wait(ingredientes[ingrediente_actual]);
     sem_wait(exclusion_mutua);
     cout << "Fumador " << ingrediente_actual << " retira su ingrediente" << endl;
     sem_signal(mostrador_vacio);
     fumar(ingrediente_actual);
     sem_signal(exclusion_mutua);
   }
}

//----------------------------------------------------------------------

int main()
{

  thread hebra_estanquero ( funcion_hebra_estanquero );
  thread hebras_fumadores[TAM]; //  TAM = 3

  for(unsigned i = 0; i < TAM; i++){
    hebras_fumadores[i] = thread( funcion_hebra_fumador, i );
  }

  hebra_estanquero.join() ;
  for( unsigned i = 0 ; i < TAM ; i++ ){
           hebras_fumadores[i].join() ;
  }

}
