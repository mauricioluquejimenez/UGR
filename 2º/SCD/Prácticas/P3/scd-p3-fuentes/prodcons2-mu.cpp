// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: prodcons2-mu.cpp
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
   num_prod              = 5 ,                        // número de productores
   num_cons              = 8 ,                        // número de consumidores 
   id_buffer             = num_prod ,                 // identificador del proceso buffer (entre los productores y los consumidores)
   num_procesos_esperado = num_prod + num_cons + 1 ,  // número de procesos esperando (número total de procesos)
   num_items             = 40,                        // número de items a producir/consumir
   tam_vector            = 10,                        // tamaño del buffer
   etiq_prod             = 0 ,                        // etiqueta para los productores
   etiq_cons             = 1 ;                        // etiqueta para los consumidores

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
      // Se produce el valor
      int valor_producido = producir(prod);

      // Se envía el valor producido al buffer (id_buffer) con la etiqueta etiq_prod (el que envía el mensaje es el productor)
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
      // Enviar petición de consumo al buffer (id_buffer) con la etiqueta etiq_cons
      MPI_Ssend( &peticion,  1, MPI_INT, id_buffer, etiq_cons, MPI_COMM_WORLD);

      // Recibir el valor a consumir del buffer (id_buffer) con la etiqueta etiq_cons
      MPI_Recv ( &valor_rec, 1, MPI_INT, id_buffer, etiq_cons, MPI_COMM_WORLD,&estado );
      cout << "Consumidor " << cons << " ha recibido valor " << valor_rec << endl << flush ;

      // Consumir el valor recibido
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
              etiq_aceptable ;         // identificador de emisor aceptable
   
   MPI_Status estado ;                 // metadatos del mensaje recibido

   for( unsigned int i=0 ; i < num_items*2 ; i++ )
   {
      // 1. determinar si puede enviar solo prod., solo cons, o todos

      // Si el buffer está vacío, solo se pueden aceptar mensajes de productores
      if ( num_celdas_ocupadas == 0 )
         etiq_aceptable = etiq_prod ;

      // Si el buffer está lleno, solo se pueden aceptar mensajes de consumidores
      else if ( num_celdas_ocupadas == tam_vector )
         etiq_aceptable = etiq_cons ;

      // Si el buffer tiene celdas ocupadas y libres, se pueden aceptar mensajes de ambos
      else 
         etiq_aceptable = MPI_ANY_TAG ;

      // 2. recibir un mensaje del emisor o emisores aceptables

      // Recibir un mensaje de cualquier emisor (MPI_ANY_SOURCE) con la etiqueta etiq_aceptable previamente asignada
      MPI_Recv( &valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_aceptable, MPI_COMM_WORLD, &estado );

      // 3. procesar el mensaje recibido

      // leer etiqueta del mensaje en metadatos”
      switch( estado.MPI_TAG )
      {
         // si ha sido el productor: insertar en buffer
         case etiq_prod: 

            // insertar valor en la primera posición libre del buffer
            buffer[primera_libre] = valor ;

            // actualizar índice de primera libre y número de celdas ocupadas
            primera_libre = (primera_libre + 1) % tam_vector ;
            num_celdas_ocupadas++ ;
            cout << "Buffer ha recibido valor " << valor << endl ;
            break;

         // si ha sido el consumidor: extraer y enviarle
         case etiq_cons: 

            // extraer valor de la primera celda ocupada del buffer
            valor = buffer[primera_ocupada] ;

            // actualizar índice de primera ocupada y número de celdas ocupadas
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

   // Conocer el identificador del proceso que actualmente se está ejecutando
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );

   // Conocer el número total de procesos que hay en ejecución
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );

   if ( num_procesos_esperado == num_procesos_actual )
   {
      // ejecutar la operación apropiada a 'id_propio'
      // si el id_propio es menor que num_prod es un productor
      if ( id_propio < num_prod ) 
         funcion_productor(id_propio);

      // si el id_propio es igual a id_buffer es el buffer
      else if ( id_propio == id_buffer )
         funcion_buffer();

      // si no es ni productor ni buffer, es un consumidor
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
