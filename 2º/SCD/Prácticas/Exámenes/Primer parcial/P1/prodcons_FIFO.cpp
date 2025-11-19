/* g++ -std=c++11 -pthread -o prodcons_FIFO prodcons_FIFO.cpp scd.cpp */

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
   siguiente_dato       =  0 ;    // siguiente dato a producir en 'producir_dato' (solo se usa ahí)
   
unsigned int vec[tam_vec] = {0};
unsigned int primera_libre = 0 , primera_ocupada = 0;

Semaphore libres = tam_vec;
Semaphore ocupadas = 0;

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

unsigned producir_dato()
{
   this_thread::sleep_for( chrono::milliseconds( aleatorio<20,100>() ));

   /* Para un solo productor, sólo hay que consultar el siguiente dato a producir: no hay que tener en cuenta otras hebras */
   const unsigned dato_producido = siguiente_dato ;

   /* Simplemente se incrementa el siguiente dato a producir */
   siguiente_dato++ ;

   /* Se incrementa el contador de elementos producidos */
   cont_prod[dato_producido] ++ ;

   cout << "producido: " << dato_producido << endl << flush ;
   return dato_producido ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned dato )
{
   assert( dato < num_items );

   /* Se incrementa el contador de elementos consumidos */
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

         /* Inicio de sección crítica */

         /* Se incrementa el vector vec con el dato producido en la primera posición libre */
      	vec[primera_libre] = dato;

         /* Se incrementa el valor de primera libre para la siguiente producción, teniendo en cuenta el tamaño del vector */
      	primera_libre = (primera_libre + 1) % tam_vec;

         /* Fin de sección crítica */
      
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
		
         /* Inicio de sección crítica */

         /* Se lee el dato de la primera posicion ocupada para asignárselo al dato que se va a consumir */
			dato = vec[primera_ocupada];

         /* Se incrementa el valor de primera ocupada para la siguiente consumición, teniendo en cuenta el tamaño del vector */
			primera_ocupada = (primera_ocupada + 1) % tam_vec;

         /* Fin de sección crítica */
			
		/* Una vez se ha consumido, se aumenta el valor de libres para que la hebra productora pueda producir */
		sem_signal(libres);
      
      consumir_dato( dato ) ;
    }
}
//----------------------------------------------------------------------

int main()
{
   cout << "------------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores (solución FIFO)." << endl
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
