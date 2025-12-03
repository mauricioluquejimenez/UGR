// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: prodcons2.cpp
// Implementación del problema del productor-consumidor con
// un proceso intermedio que gestiona un buffer finito y recibe peticiones
// en orden arbitrario
// (versión con productores y consumidores múltiples)
//
// Historial:
// Actualizado a C++11 en Septiembre de 2017
//
// Compilación: mpicxx -std=c++11 -o prodcons2 prodcons2-mu.cpp
// Ejecución: mpirun -oversubscribe -np <num_procesos> ./prodcons2-mu
// -----------------------------------------------------------------------------

#include <iostream>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <mpi.h>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int
   num_prod              = 5 ,
   num_cons              = 8 ,
   id_buffer             = num_prod ,
   num_procesos_esperado = num_prod + num_cons + 1 ,
   num_items             = 40,
   tam_vector            = 10,
   etiq_prod             = 0 ,
   etiq_cons             = 1 ;

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
// ---------------------------------------------------------------------
// ptoducir produce los numeros en secuencia (1,2,3,....)
// y lleva espera aleatorio
int producir(int prod)
{
   static int contador[num_prod] = {0} ;
   int valor_producido, inicio;
   sleep_for( milliseconds( aleatorio<10,100>()) );
   inicio = prod * (num_items / num_prod);
   contador[prod]++ ;
   valor_producido = inicio + contador[prod];
   cout << "Productor " << prod << " ha producido valor " << valor_producido << endl << flush;
   return valor_producido ;
}
// ---------------------------------------------------------------------

void funcion_productor(int prod)
{
   // producir valor
   for(int i = 0; i < num_items / num_prod; i++)
   {
      int valor_producido = producir(prod);

      // enviar valor
      cout << "Productor " << prod << " va a enviar valor " << valor_producido << endl << flush;
      MPI_Ssend( &valor_producido, 1, MPI_INT, id_buffer, etiq_prod, MPI_COMM_WORLD );
   } 
}
// ---------------------------------------------------------------------

void consumir( int cons, int valor_cons )
{
   // espera bloqueada
   sleep_for( milliseconds( aleatorio<110,200>()) );
   cout << "Consumidor " <<  cons << " ha consumido valor " << valor_cons << endl << flush ;
}
// ---------------------------------------------------------------------

void funcion_consumidor(int cons)
{
   int peticion = 1, valor_rec = 1 ;
   MPI_Status  estado ;

   for( unsigned int i = 0 ; i < num_items / num_cons; i++ )
   {
      MPI_Ssend( &peticion,  1, MPI_INT, id_buffer, etiq_cons, MPI_COMM_WORLD);
      MPI_Recv ( &valor_rec, 1, MPI_INT, id_buffer, etiq_cons, MPI_COMM_WORLD,&estado );
      cout << "Consumidor " << cons << " ha recibido valor " << valor_rec << endl << flush ;
      consumir( cons, valor_rec );
   }
}
// ---------------------------------------------------------------------

void funcion_buffer()
{
   int        buffer[tam_vector],      // buffer con celdas ocupadas y vacías
              valor,                   // valor recibido o enviado
              primera_libre       = 0, // índice de primera celda libre
              primera_ocupada     = 0, // índice de primera celda ocupada
              num_celdas_ocupadas = 0, // número de celdas ocupadas
              etiq_aceptable ;    // identificador de emisor aceptable
   
   MPI_Status estado ;                 // metadatos del mensaje recibido

   for( unsigned int i=0 ; i < num_items*2 ; i++ )
   {
      // 1. determinar si puede enviar solo prod., solo cons, o todos

      if ( num_celdas_ocupadas == 0 )               // si buffer vacío
         etiq_aceptable = etiq_prod ;       // $~~~$ solo prod.
      else if ( num_celdas_ocupadas == tam_vector ) // si buffer lleno
         etiq_aceptable = etiq_cons ;      // $~~~$ solo cons.
      else                                          // si no vacío ni lleno
         etiq_aceptable = MPI_ANY_TAG ;     // $~~~$ cualquiera

      // 2. recibir un mensaje del emisor o emisores aceptables

      MPI_Recv( &valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_aceptable, MPI_COMM_WORLD, &estado );

      // 3. procesar el mensaje recibido

      switch( estado.MPI_TAG ) // leer etiqueta del mensaje en metadatos”
      {
         case etiq_prod: // si ha sido el productor: insertar en buffer
            buffer[primera_libre] = valor ;
            primera_libre = (primera_libre + 1) % tam_vector ;
            num_celdas_ocupadas++ ;
            cout << "Buffer ha recibido valor " << valor << endl ;
            break;

         case etiq_cons: // si ha sido el consumidor: extraer y enviarle
            valor = buffer[primera_ocupada] ;
            primera_ocupada = (primera_ocupada+1) % tam_vector ;
            num_celdas_ocupadas-- ;
            cout << "Buffer va a enviar valor " << valor << endl ;
            MPI_Ssend( &valor, 1, MPI_INT, estado.MPI_SOURCE, etiq_cons, MPI_COMM_WORLD);
            break;
      }
   }
}

// ---------------------------------------------------------------------

int main( int argc, char *argv[] )
{
   int id_propio, num_procesos_actual;

   // inicializar MPI, leer identif. de proceso y número de procesos
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   if ( num_procesos_esperado == num_procesos_actual )
   {
      // ejecutar la operación apropiada a 'id_propio'
      if ( id_propio < num_prod )
         funcion_productor(id_propio);
      else if ( id_propio == id_buffer )
         funcion_buffer();
      else
         funcion_consumidor(id_propio - num_prod - 1);
   }
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos_esperado << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   // al terminar el proceso, finalizar MPI
   MPI_Finalize( );
   return 0;
}
