/* g++ -std=c++11 -pthread -o fumadores fumadores.cpp scd.cpp */

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "scd.h"

using namespace std ;
using namespace scd ;

// numero de fumadores 

const int num_fumadores = 3 ;

Semaphore ingredientes[num_fumadores] = {0, 0, 0};
Semaphore mostr_vacio = 1;
Semaphore exclusion_mutua = 1;

//-------------------------------------------------------------------------
// Función que simula la acción de producir un ingrediente, como un retardo
// aleatorio de la hebra (devuelve número de ingrediente producido)

int producir_ingrediente()
{
   // calcular milisegundos aleatorios de duración de la acción de fumar)
   chrono::milliseconds duracion_produ( aleatorio<10,100>() );

   // informa de que comienza a producir
   cout << "Estanquero : empieza a producir ingrediente (" << duracion_produ.count() << " milisegundos)" << endl;

   // espera bloqueada un tiempo igual a ''duracion_produ' milisegundos
   this_thread::sleep_for( duracion_produ );

   const int num_ingrediente = aleatorio<0,num_fumadores-1>() ;

   // informa de que ha terminado de producir
   cout << "Estanquero : termina de producir ingrediente " << num_ingrediente << endl;

   return num_ingrediente ;
}

//----------------------------------------------------------------------
// función que ejecuta la hebra del estanquero

void funcion_hebra_estanquero(  )
{
	int i;
	
	while(true)
	{
		/* Se decrementa el valor del mostrador para evitar que se extraiga nada de él */
		sem_wait(mostr_vacio);
		i = producir_ingrediente();
		
		/* Se evita que varios fumadores entren en la sección crítica en el momento en el que se imprime el ingrediente producido */
		sem_wait(exclusion_mutua);
		cout << "Estanquero produce " << i << endl;
		
		/* Una vez se ha salido de la sección crítica, se permite que los fumadores continúen su ejecución */
		sem_signal(exclusion_mutua);
		
		/* Como ya se ha producido un ingrediente, se incremeneta su respectivo valor de semáforo para que el fumador i-ésimo pueda retirarlo del mostrador */
		sem_signal(ingredientes[i]);
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
   while( true )
   {
   	/* Se decrementa el valor de semáforo del ingrediente i-ésimo ya que el fumador i-ésimo va a retirar el ingrediente del mostrador */
   	sem_wait(ingredientes[num_fumador]);
   	
   	/* Se evita que varios fumadores o el estanquero entren en la sección crítica para modificar el valor del ingrediente retirado */
   	sem_wait(exclusion_mutua);
		cout << "Fumador " << num_fumador << " retira su ingrediente" << endl;
		
		/* Se incrementa el valor del mostrador ya que se ha retirado un ingrediente y el estanquero puede producir un ingrediente y colocarlo en el mostrador */
		sem_signal(mostr_vacio);
		fumar(num_fumador);
		
		/* Una vez se ha salido de la sección crítica, se permite que el estanquero y el resto de fumadores continúen su ejecución */
		sem_signal(exclusion_mutua);
   }
}

//----------------------------------------------------------------------

int main()
{
	cout << "-------------------------------------------------------------------" << endl
        << "			Problema de los fumadores" 											<< endl
        << "-------------------------------------------------------------------" << endl
        << flush ;
        
   thread hebra_estanquero(funcion_hebra_estanquero);
   thread hebra_fumadores[num_fumadores];
   
   for(int i = 0; i < num_fumadores; i++) hebra_fumadores[i] = thread(funcion_hebra_fumador, i);
   
   hebra_estanquero.join();
   
   for(int i = 0; i < num_fumadores; i++) hebra_fumadores[i].join();
}
