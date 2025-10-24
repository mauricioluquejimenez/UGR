/* g++ -std=c++11 -pthread -o prodcons_LIFO prodcons_LIFO.cpp scd.cpp */

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
	tam_vec   = 10 ;   // tamaño del buffer
	
unsigned  
   cont_prod[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha producido.
   cont_cons[num_items] = {0}, // contadores de verificación: para cada dato, número de veces que se ha consumido.
   siguiente_dato       =  0 ; // siguiente dato a producir en 'producir_dato' (solo se usa ahí)
   
unsigned int vec[tam_vec] = {0};
unsigned int primera_libre = 0 ;

Semaphore libres = tam_vec;
Semaphore ocupadas = 0;
Semaphore exclusion_mutua = 1;

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

unsigned producir_dato()
{
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));
   const unsigned dato_producido = siguiente_dato ;
   siguiente_dato++ ;
   cont_prod[dato_producido] ++ ;
   cout << "producido: " << dato_producido << endl << flush ;
   return dato_producido ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato )
{
   assert( dato < num_items );
   cont_cons[dato] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   cout << "                  consumido: " << dato << endl ;
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

void  funcion_hebra_productora(  )
{	
   for( unsigned i = 0 ; i < num_items ; i++ )
   {
      int dato = producir_dato() ;

		/* Se decrementa el valor de libres para impedir que la hebra consumidora intervenga mientras se está produciendo */
      sem_wait(libres);
      
      /* Se evita que un productor y un consumidor entren simultáneamente a la sección crítica */
      sem_wait(exclusion_mutua);
      
      	vec[primera_libre] = dato;
      	primera_libre++;
      	
      /* Una vez termina la sección crítica, se permite que todos los productores continúen sus procesos */
      sem_signal(exclusion_mutua);

      /* Una vez se ha producido, se aumenta el valor de libres para que la hebra consumidora pueda consumir */
      sem_signal(ocupadas);
   }
}

//----------------------------------------------------------------------

/* Mientras que los productores sí tienen asignado un rango concreto, los consumidores consumen el primer dato que ven */
void funcion_hebra_consumidora(  )
{	
   for( unsigned i = 0 ; i < num_items ; i++ )
   {
      int dato ;

		/* Se decrementa el valor de ocupadas para impedir que la hebra productora intervenga mientras se está consumiendo */
		sem_wait(ocupadas);

		/* Se evita que un productor y un consumidor entren simultáneamente a la sección crítica */
		sem_wait(exclusion_mutua);
		
			dato = vec[primera_libre - 1];
			primera_libre--;
			
		/* Una vez termina la sección crítica, se permite que todos los productores y consumidores continúen sus procesos */
		sem_signal(exclusion_mutua);
		
		/* Una vez se ha producido, se aumenta el valor de libres para que la hebra consumidora pueda consumir */
		sem_signal(libres);
		
      consumir_dato( dato ) ;
    }
}
//----------------------------------------------------------------------

int main()
{
   cout << "------------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (solución LIFO)." << endl
        << "------------------------------------------------------------------" << endl
        << flush ;

   thread hebra_productora ( funcion_hebra_productora ),
          hebra_consumidora( funcion_hebra_consumidora );

   hebra_productora.join() ;
   hebra_consumidora.join() ;
 
 	cout << "----------------------------------------------------------------" << endl
 		  << "                            Fin                                 " << endl
 		  << "----------------------------------------------------------------" << endl
 		  << flush ;  

   test_contadores();
}
