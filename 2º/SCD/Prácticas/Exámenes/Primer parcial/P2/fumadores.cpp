/* g++ -std=c++11 -pthread -o fumadores fumadores.cpp scd.cpp */

#include <iostream>
#include <thread>
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include "scd.h"

using namespace std ;
using namespace scd ;

// numero de fumadores 

const int num_fumadores = 3 ;
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

class Estanco : public HoareMonitor
{
 private:
   // Valor del ingrediente en el mostrador (-1 si está vacío)
   int mostrador;

   CondVar
      // ¿Está el mostrador vacío?                    
      mostr_vacio,

      // Variable condición para cada fumador
      ingredientes[num_fumadores];

 public:                    // constructor y métodos públicos
   Estanco() ;             // constructor
   void obtenerIngrediente( int i );
   void ponerIngrediente( int i );
   void esperarRecogidaIngrediente();
} ;

Estanco::Estanco()
{
   // Se inicializa el mostrador como vacío
   mostrador = -1 ;

   // Se crea la variable de condición para el estanquero
   mostr_vacio = newCondVar();

   // Se crean las variables de condición para los fumadores
   for(int i = 0; i < num_fumadores; i++) ingredientes[i] = newCondVar();
}

void Estanco::obtenerIngrediente( int i)
{
   // Si el ingrediente no está en el mostrador, esperar
   if(mostrador != i) ingredientes[i].wait();

   // Cuando el ingrediente está en el mostrador, se retira
   mostrador = -1;

   // Se indica al estanquero que el mostrador está vacío
   mostr_vacio.signal();
}

void Estanco::ponerIngrediente( int i )
{
   // Si el mostrador no está vacío, esperar
   if(mostrador != -1) mostr_vacio.wait();

   // Cuando el mostrador está vacío, se pone el ingrediente
   mostrador = i;
   cout << "Estanquero pone ingrediente " << i << " en el mostrador." << endl;

   // Se avisa al fumador correspondiente de que su ingrediente está en el mostrador
   ingredientes[i].signal();
}

void Estanco::esperarRecogidaIngrediente()
{
   // Si el mostrador no está vacío, esperar
   if(mostrador != -1) mostr_vacio.wait();
}

// función que ejecuta la hebra del fumador
void  funcion_hebra_fumador( MRef<Estanco> monitor, int num_fumador )
{
   while( true )
   {
      // El fumador recoge su ingrediente
   	monitor->obtenerIngrediente(num_fumador);
		cout << "Fumador " << num_fumador << " retira su ingrediente" << endl;
		
      // Una vez el fumador ha recogido su ingrediente, puede fumar
		fumar(num_fumador);
   }
}

// función que ejecuta la hebra del estanquero
void funcion_hebra_estanquero(  MRef<Estanco> monitor )
{
	int i;
	
	while(true)
	{
      // Se produce un ingrediente
		i = producir_ingrediente();

      // El estanquero pone el ingrediente en el mostrador
		monitor->ponerIngrediente(i);
		cout << "Estanquero produce " << i << endl;

      // El estanquero espera a que el fumador retire el ingrediente
		monitor->esperarRecogidaIngrediente();
   }
}

//----------------------------------------------------------------------

int main()
{
	cout << "-------------------------------------------------------------------" << endl
        << "			Problema de los fumadores" 											<< endl
        << "-------------------------------------------------------------------" << endl
        << flush ;
        
   // Se crea el monitor del estanco
   MRef<Estanco> estanco = Create<Estanco>();

   // Se crean las hebras del estanquero y de los fumadores
   thread hebra_estanquero;
   thread hebra_fumadores[num_fumadores];

   // Se lanza la hebra del estanquero
   hebra_estanquero = thread(funcion_hebra_estanquero, estanco);
   
   // Se lanzan las hebras de los fumadores
   for(int i = 0; i < num_fumadores; i++) hebra_fumadores[i] = thread(funcion_hebra_fumador, estanco, i);
   
   // Se espera a que termine la hebra del estanquero (nunca en este caso)
   hebra_estanquero.join();
   
   // Se espera a que terminen las hebras de los fumadores (nunca en este caso)
   for(int i = 0; i < num_fumadores; i++) hebra_fumadores[i].join();
}