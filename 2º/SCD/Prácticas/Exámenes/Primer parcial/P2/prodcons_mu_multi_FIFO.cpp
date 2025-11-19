// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Seminario 2. Introducción a los monitores en C++11.
//
// Archivo: prodcons_mu.cpp
//
// Ejemplo de un monitor en C++11 con semántica SU, para el problema
// del productor/consumidor, con productor y consumidor únicos.
// Opcion LIFO
//
// Historial:
// Creado el 30 Sept de 2022. (adaptado de prodcons2_su.cpp)
// 20 oct 22 --> paso este archivo de FIFO a LIFO, para que se corresponda con lo que dicen las transparencias
// -----------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>
#include <thread>
#include "scd.h"

using namespace std ;
using namespace scd ;

constexpr int
   num_items = 50 ;   // número de items a producir/consumir

constexpr int               
   min_ms    = 5,     // tiempo minimo de espera en sleep_for
   max_ms    = 20 ;   // tiempo máximo de espera en sleep_for

const unsigned
   num_prod = 10 , // número de hebras productoras
   num_cons	= 5 ; // número de hebras consumidoras

unsigned
   cont_prod[num_items] = {0}, // contadores de verificación: producidos
   cont_cons[num_items] = {0}, // contadores de verificación: consumidos
   producidos[num_prod]	= {0}; // Se deja de usar la variable "siguiente_dato" porque la producción ya no es lineal y cada hebra produce un rango concreto de items.
										 // Cuenta los datos producidos por cada hebra individualmente
   
mutex mtx;

//**********************************************************************
// funciones comunes a las dos soluciones (fifo y lifo)
//----------------------------------------------------------------------

int producir_dato( unsigned hebra )
{
   
   this_thread::sleep_for( chrono::milliseconds( aleatorio<min_ms,max_ms>() ));

   /* Para varios productores, hay que tener en cuenta otras hebras para saber qué parte del vector ocupar */
   const unsigned valor_producido = hebra * (num_items / num_prod) + producidos[hebra] ;

   /* Simplemente se incrementa el contador de datos producidos por esta hebra */
   producidos[hebra] ++;

   mtx.lock();
   cout << "Productor " << hebra << " produce: " << valor_producido << endl << flush ;
   mtx.unlock();

   /* Se incrementa el contador de elementos producidos */
   cont_prod[valor_producido]++ ;
   return valor_producido ;
}
//----------------------------------------------------------------------

void consumir_dato( unsigned valor_consumir, unsigned hebra )
{
   if ( num_items <= valor_consumir )
   {
      cout << " valor a consumir === " << valor_consumir << ", num_items == " << num_items << endl ;
      assert( valor_consumir < num_items );
   }

   /* Se incrementa el contador de elementos consumidos */
   cont_cons[valor_consumir] ++ ;
   this_thread::sleep_for( chrono::milliseconds( aleatorio<min_ms,max_ms>() ));
   
   mtx.lock();
   cout << "                  Consumidor " << hebra << " consume: " << valor_consumir << endl ;
   mtx.unlock();
}
//----------------------------------------------------------------------

void test_contadores()
{
   bool ok = true ;
   cout << "comprobando contadores ...." << endl ;

   for( unsigned i = 0 ; i < num_items ; i++ )
   {
      if ( cont_prod[i] != 1 )
      {
         cout << "error: valor " << i << " producido " << cont_prod[i] << " veces." << endl ;
         ok = false ;
      }
      if ( cont_cons[i] != 1 )
      {
         cout << "error: valor " << i << " consumido " << cont_cons[i] << " veces" << endl ;
         ok = false ;
      }
   }
   if (ok)
      cout << endl << flush << "solución (aparentemente) correcta." << endl << flush ;
}

// *****************************************************************************
// clase para monitor buffer, version FIFO, semántica SC, multiples prod/cons

class ProdConsMu : public HoareMonitor
{
 private:
 static const int           // constantes ('static' ya que no dependen de la instancia)
   num_celdas_total = 10;   //   núm. de entradas del buffer
 int                        // variables permanentes
   buffer[num_celdas_total],//   buffer de tamaño fijo, con los datos
   primera_libre ,          //   indice de celda de la próxima inserción ( == número de celdas ocupadas)
   primera_ocupada ;

 CondVar                    // colas condicion:
   ocupadas,                //  cola donde espera el consumidor (n>0)
   libres ;                 //  cola donde espera el productor  (n<num_celdas_total)

 public:                    // constructor y métodos públicos
   ProdConsMu() ;             // constructor
   int  leer();                // extraer un valor (sentencia L) (consumidor)
   void escribir( int valor ); // insertar un valor (sentencia E) (productor)
} ;
// -----------------------------------------------------------------------------

ProdConsMu::ProdConsMu(  )
{
   primera_libre = 0 ;
   primera_ocupada = 0 ;
   ocupadas      = newCondVar();
   libres        = newCondVar();
}
// -----------------------------------------------------------------------------
// función llamada por el consumidor para extraer un dato

int ProdConsMu::leer(  )
{
   // esperar bloqueado hasta que 0 < primera_libre
   if ( primera_libre == primera_ocupada ) ocupadas.wait();

   //cout << "leer: ocup == " << primera_libre << ", total == " << num_celdas_total << endl ;
   assert( primera_libre != primera_ocupada  );

   // hacer la operación de lectura, actualizando estado del monitor
   const int valor = buffer[primera_ocupada] ;

   /* Se incrementa el valor de primera ocupada para la siguiente consumición, teniendo en cuenta el tamaño del vector */
   primera_ocupada = (primera_ocupada + 1) % num_celdas_total ;
   
   // señalar al productor que hay un hueco libre, por si está esperando
   libres.signal();

   // devolver valor
   return valor ;
}
// -----------------------------------------------------------------------------

void ProdConsMu::escribir( int valor )
{
   // esperar bloqueado hasta que primera_libre < num_celdas_total
   if ( ((primera_libre + 1) % num_celdas_total) == primera_ocupada ) 
      libres.wait();

   //cout << "escribir: ocup == " << primera_libre << ", total == " << num_celdas_total << endl ;
   assert( ((primera_libre + 1) % num_celdas_total) != primera_ocupada );

   // hacer la operación de inserción, actualizando estado del monitor
   buffer[primera_libre] = valor ;

   /* Se incrementa el valor de primera libre para la siguiente producción, teniendo en cuenta el tamaño del vector */
   primera_libre = (primera_libre + 1) % num_celdas_total ;

   // señalar al consumidor que ya hay una celda ocupada (por si esta esperando)
   ocupadas.signal();
}
// *****************************************************************************
// funciones de hebras

void funcion_hebra_productora( MRef<ProdConsMu> monitor, unsigned hebra )
{
   for( unsigned i = 0 ; i < num_items / num_prod ; i++ )
   {
      int valor = producir_dato( hebra ) ;
      monitor->escribir( valor );
   }
}
// -----------------------------------------------------------------------------

void funcion_hebra_consumidora( MRef<ProdConsMu>  monitor, unsigned hebra )
{
   for( unsigned i = 0 ; i < num_items / num_cons ; i++ )
   {
      int valor = monitor->leer();
      consumir_dato( valor, hebra ) ;
   }
}
// -----------------------------------------------------------------------------

int main()
{
   cout << "----------------------------------------------------------------------------" << endl
        << "Problema de los productores-consumidores múltiples (Monitor SU, buffer FIFO). " << endl
        << "----------------------------------------------------------------------------" << endl
        << flush ;

   // crear monitor  ('monitor' es una referencia al mismo, de tipo MRef<...>)
   MRef<ProdConsMu> monitor = Create<ProdConsMu>() ;

   // crear y lanzar las hebras
   thread hebra_productora[num_prod],
          hebra_consumidora[num_cons];
        
   for(int i = 0; i < num_prod; i++) hebra_productora[i]  = thread(funcion_hebra_productora, monitor, i);
   for(int i = 0; i < num_cons; i++) hebra_consumidora[i] = thread(funcion_hebra_consumidora, monitor, i);
   
   // esperar a que terminen las hebras
   for(int i = 0; i < num_prod; i++) hebra_productora[i].join() ;
   for(int i = 0; i < num_cons; i++) hebra_consumidora[i].join() ;
   test_contadores() ;
}
