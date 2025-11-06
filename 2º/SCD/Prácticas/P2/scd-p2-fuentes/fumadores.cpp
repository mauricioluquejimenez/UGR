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

mutex estanco;

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

void funcion_hebra_estanquero(  MRef<Estanco> monitor )
{
	int i;
	
	while(true)
	{
		i = producir_ingrediente();
		monitor->ponerIngrediente(i);
		cout << "Estanquero produce " << i << endl;

		monitor->esperarRecogidaIngrediente();
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
void  funcion_hebra_fumador( MRef<Estanco> monitor, int num_fumador )
{
   while( true )
   {
   	monitor->obtenerIngrediente(num_fumador);
		cout << "Fumador " << num_fumador << " retira su ingrediente" << endl;
		
		fumar(num_fumador);
	
		sem_signal(exclusion_mutua);
   }
}

class Estanco : public HoareMonitor
{
 private:
 static const int           // constantes ('static' ya que no dependen de la instancia)
   num_celdas_total = 10;   //   núm. de entradas del buffer
 int                        // variables permanentes
   buffer[num_celdas_total],//   buffer de tamaño fijo, con los datos
   primera_libre ;          //   indice de celda de la próxima inserción ( == número de celdas ocupadas)

 CondVar                    // colas condicion:
   ocupadas,                //  cola donde espera el consumidor (n>0)
   libres ;                 //  cola donde espera el productor  (n<num_celdas_total)

 public:                    // constructor y métodos públicos
   Estanco() ;             // constructor
   int  obtenerIngrediente( int i);
   void ponerIngrediente( int i );
   void esperarRecogidaIngrediente();
} ;

Estanco::Estanco()
{
   primera_libre = 0 ;
   ocupadas      = newCondVar();
   libres        = newCondVar();
}

//----------------------------------------------------------------------

int main()
{
	cout << "-------------------------------------------------------------------" << endl
        << "			Problema de los fumadores" 											<< endl
        << "-------------------------------------------------------------------" << endl
        << flush ;
        
   //thread hebra_estanquero(funcion_hebra_estanquero);
   //thread hebra_fumadores[num_fumadores];
   
   //for(int i = 0; i < num_fumadores; i++) hebra_fumadores[i] = thread(funcion_hebra_fumador, i);
   
   //hebra_estanquero.join();
   
   //for(int i = 0; i < num_fumadores; i++) hebra_fumadores[i].join();
}
