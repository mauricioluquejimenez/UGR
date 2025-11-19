/* g++ -std=c++11 -pthread -o prodcons_FIFO-multi prodcons_FIFO-multi.cpp scd.cpp */

#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <random>
#include "scd.h"

using namespace std ;
using namespace scd ;

//**********************************************************************
// Variables globales

const unsigned 
   num_items = 40 ,   // número de items
	tam_vec   = 10 ,   // tamaño del buffer
	num_prod	 = 4	,
	num_cons	 = 2	;
	
unsigned  
   cont_prod[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha producido. Cuenta los datos producidos en total
   cont_cons[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha consumido.
   producidos[num_prod]	= {0}; // Se deja de usar la variable "siguiente_dato" porque la producción ya no es lineal y cada hebra produce un rango concreto de items.
										 // Cuenta los datos producidos por cada hebra individualmente
   
unsigned int vec[tam_vec] = {0};
unsigned int primera_libre = 0 , primera_ocupada = 0;

Semaphore libres = tam_vec;
Semaphore ocupadas = 0;
Semaphore exclusion_mutua_prod = 1; // Al haber varios productores, además de la sincronización entre productores y consumidores, hay que asegurarse que los propios productores no se solapan
Semaphore exclusion_mutua_cons = 1; // Al haber varios consumidores, además de la sincronización entre productores y consumidores, hay que asegurarse que los propios consumidores no se solapan

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

unsigned producir_dato( unsigned hebra )
{
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   
   /* Para varios productores, hay que tener en cuenta otras hebras para saber qué parte del vector ocupar */
   const unsigned dato_producido = hebra * (num_items / num_prod) + producidos[hebra] ;

   /* Se incrementa el siguiente dato a producir dentro de la hebra actual*/
   producidos[hebra] ++;

   /* Se incrementa el contador de elementos producidos */
   cont_prod[dato_producido] ++ ;
   cout << "Productor " << hebra << " ha producido: " << dato_producido << endl << flush ;
   return dato_producido ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato, unsigned hebra )
{
   assert( dato < num_items );

   /* Se incrementa el contador de elementos consumidos */
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "                  Consumidor " << hebra << " ha consumido: " << dato << endl ;
}


//----------------------------------------------------------------------

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." ;
   for( unsigned i = 0 ; i < num_items ; i++ )
   {  if ( cont_prod[i] != 1 )
      {  cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {  cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

//----------------------------------------------------------------------

void  funcion_hebra_productora( unsigned hebra )
{
   for( unsigned i = 0 ; i < num_items / num_prod ; i++ )
   {
      int dato = producir_dato(hebra) ;
      
      /* Se decrementa el valor de libres para impedir que la hebra consumidora intervenga mientras se está produciendo */
      sem_wait(libres);
      
      /* Se evita que varios productores se solapen */
      sem_wait(exclusion_mutua_prod);

         /* Inicio de sección crítica */
      
         /* Se incrementa el vector vec con el dato producido en la primera posición libre */
      	vec[primera_libre] = dato;

         /* Se incrementa el valor de primera libre para la siguiente producción, teniendo en cuenta el tamaño del vector */
      	primera_libre = (primera_libre + 1) % tam_vec;

         /* Fin de sección crítica */
      
      /* Una vez termina la sección crítica, se permite que todos los productores continúen sus procesos */
      sem_signal(exclusion_mutua_prod);
      
      /* Una vez se ha producido, se aumenta el valor de libres para que la hebra consumidora pueda consumir */
      sem_signal(ocupadas);
   }
}

//----------------------------------------------------------------------

/* Mientras que los productores sí tienen asignado un rango concreto, los consumidores consumen el primer dato que ven */
void funcion_hebra_consumidora( unsigned hebra )
{
   for( unsigned i = 0 ; i < num_items / num_cons ; i++ )
   {
      int dato ;
      
      /* Se decrementa el valor de ocupadas para impedir que la hebra productora intervenga mientras se está consumiendo */
      sem_wait(ocupadas);
      
      /* Se evita que varios consumidores entren simultáneamente a la sección crítica */
      sem_wait(exclusion_mutua_cons);

         /* Inicio de sección crítica */
		
         /* Se lee el dato de la primera posicion ocupada para asignárselo al dato que se va a consumir */
			dato = vec[primera_ocupada];

         /* Se incrementa el valor de primera ocupada para la siguiente consumición, teniendo en cuenta el tamaño del vector */
			primera_ocupada = (primera_ocupada + 1) % tam_vec;

         /* Fin de sección crítica */
			
		/* Una vez termina la sección crítica, se permite que todos los consumidores continúen sus procesos */
		sem_signal(exclusion_mutua_cons);
		
		/* Una vez se ha consumido, se aumenta el valor de libres para que la hebra productora pueda producir */
		sem_signal(libres);
      
      consumir_dato( dato, hebra ) ;
    }
}
//----------------------------------------------------------------------

int main()
{
   cout << "-------------------------------------------------------------------" << endl
        << "Problema de los múltiples productores-consumidores (solución FIFO)." << endl
        << "-------------------------------------------------------------------" << endl
        << flush ;
        
  	thread hebra_productora[num_prod],
          hebra_consumidora[num_cons];
        
   for(int i = 0; i < num_prod; i++) hebra_productora[i]  = thread(funcion_hebra_productora, i);
   for(int i = 0; i < num_cons; i++) hebra_consumidora[i] = thread(funcion_hebra_consumidora, i);
   
   for(int i = 0; i < num_prod; i++) hebra_productora[i].join() ;
   for(int i = 0; i < num_cons; i++) hebra_consumidora[i].join() ;
 
 	cout << "----------------------------------------------------------------" << endl
 		  << "                            Fin                                 " << endl
 		  << "----------------------------------------------------------------" << endl
 		  << flush ;  

   test_contadores();
}
