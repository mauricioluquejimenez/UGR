// -----------------------------------------------------------------------------
// Sistemas concurrentes y Distribuidos.
// Práctica 3. Implementación de algoritmos distribuidos con MPI
//
// Archivo: filosofos-cam.cpp
// Implementación del problema de los filósofos (con camarero).
//
//
// Historial:
// Actualizado a C++11 en Septiembre de 2017
// Compilación: mpicxx -std=c++11 -o filosofos-cam filosofos-cam.cpp
// Ejecución: mpirun -oversubscribe -np <num_procesos> ./filosofos-cam
// -----------------------------------------------------------------------------

#include <mpi.h>
#include <thread> // this_thread::sleep_for
#include <random> // dispositivos, generadores y distribuciones aleatorias
#include <chrono> // duraciones (duration), unidades de tiempo
#include <iostream>

using namespace std;
using namespace std::this_thread ;
using namespace std::chrono ;

const int
   num_filosofos = 5 ,              // número de filósofos 
   num_filo_ten  = 2 * num_filosofos, // número de filósofos y tenedores 
   num_procesos  = num_filo_ten + 1,   // número de procesos total a los filósofos y tenedores se añade el camarero
   etiq_solicitar = 0,
   etiq_soltar = 1,
   etiq_sentarse = 2,
   etiq_levantarse = 3,
   id_camarero = 10;

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

void funcion_filosofos( int id )
{
  int id_ten_izq = (id+1)              % num_filo_ten, //id. tenedor izq.
      id_ten_der = (id+num_filo_ten-1) % num_filo_ten, //id. tenedor der.
      valor = 0;

  while ( true )
  {
      // Se solicita permiso al camarero (id_camarero) para sentarse (etiq_sentarse)
      cout << "Filósofo " << id << " solicita sentarse" <<endl;
      MPI_Ssend(&valor, 1, MPI_INT, id_camarero, etiq_sentarse, MPI_COMM_WORLD);

      // ... solicitar (etiq_solicitar) tenedor izquierdo (id_ten_izq)
      cout << "Filósofo " << id << " solicita ten. izq." <<id_ten_izq <<endl;
      MPI_Ssend(&valor, 1, MPI_INT, id_ten_izq, etiq_solicitar, MPI_COMM_WORLD);

      // ... solicitar (etiq_solicitar) tenedor derecho (id_ten_der) 
      cout << "Filósofo " << id <<" solicita ten. der." <<id_ten_der <<endl;
      MPI_Ssend(&valor, 1, MPI_INT, id_ten_der, etiq_solicitar, MPI_COMM_WORLD);

      cout << "Filósofo " << id <<" comienza a comer" <<endl ;
      sleep_for( milliseconds( aleatorio<10,100>() ) );

      // ... soltar (etiq_soltar) tenedor izquierdo (id_ten_izq)
      cout << "Filósofo " << id <<" suelta ten. izq. " <<id_ten_izq <<endl;
      MPI_Ssend(&valor, 1, MPI_INT, id_ten_izq, etiq_soltar, MPI_COMM_WORLD);

      // ... soltar (etiq_soltar) tenedor derecho (id_ten_der) 
      cout << "Filósofo " << id <<" suelta ten. der. " <<id_ten_der <<endl;
      MPI_Ssend(&valor, 1, MPI_INT, id_ten_der, etiq_soltar, MPI_COMM_WORLD);

      // Se notifica al camarero (id_camarero) que se levanta (etiq_levantarse)
      cout << "Filósofo " << id << " solicita levantarse" << endl;
      MPI_Ssend(&valor, 1, MPI_INT, id_camarero, etiq_levantarse, MPI_COMM_WORLD);

      cout << "Filosofo " << id << " comienza a pensar" << endl;
      sleep_for( milliseconds( aleatorio<10,100>() ) );
 }
}
// ---------------------------------------------------------------------

void funcion_tenedores( int id )
{
  int valor = 0, id_filosofo ;  // valor recibido, identificador del filósofo
  MPI_Status estado ;       // metadatos de las dos recepciones

  while ( true )
  {
     // ...... recibir petición de cualquier filósofo (MPI_ANY_SOURCE)
     // Como está recibiendo la petición de un filósofo para coger un tenedor, la etiqueta debe ser etiq_solicitar
     MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_solicitar, MPI_COMM_WORLD, &estado);

     // ...... guardar en 'id_filosofo' el id. del emisor
     id_filosofo = estado.MPI_SOURCE ;
     cout <<"Ten. " <<id <<" ha sido cogido por filo. " <<id_filosofo <<endl;

     // ...... recibir liberación de filósofo 'id_filosofo' (por eso se usa id_filosofo en vez de MPI_ANY_SOURCE)
     // Sólo el filósofo que previamente había cogido el tenedor con el MPI_Recv anterior puede soltarlo
     // Como está recibiendo la petición de un filósofo para soltar un tenedor, la etiqueta debe ser etiq_soltar
     MPI_Recv(&valor, 1, MPI_INT, id_filosofo, etiq_soltar, MPI_COMM_WORLD, &estado);
     cout <<"Ten. "<< id << " ha sido liberado por filo. " <<id_filosofo <<endl ;
  }
}

void funcion_camarero()
{
   int s = 0, valor = 0, id_filosofo, etiq_accion;
   MPI_Status estado;

   while( true )
   {
      // Si el número de filósofos sentados es el máximo, solo se pueden levantar
      if (s == num_filosofos - 1) etiq_accion = etiq_levantarse;

      // Si no hay ningún filósofo sentado, solo se pueden sentar
      else if (s == 0) etiq_accion = etiq_sentarse;

      // Si hay filósofos sentados pero no está lleno, se pueden sentar o levantar
      else etiq_accion = MPI_ANY_TAG;

      // Recibir la acción (sentarse o levantarse) de cualquier filósofo (MPI_ANY_SOURCE)
      MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, etiq_accion, MPI_COMM_WORLD, &estado);

      // Guardar en 'id_filosofo' el id. del emisor
      id_filosofo = estado.MPI_SOURCE;

      // Según la acción, incrementar o decrementar el contador de filósofos sentados
      // Si la acción solicitada por el filósofo del que se ha recibido el mensaje es sentarse, se incrementa el contador de sentados
      if(estado.MPI_TAG == etiq_sentarse)
      {
         s++;
         cout << "Filósofo " << id_filosofo << " se ha sentado" << endl;
      }
      // Si la acción solicitada por el filósofo del que se ha recibido el mensaje es sentarse, se incrementa el contador de sentados
      else if (estado.MPI_TAG == etiq_levantarse)
      {
         s--;
         cout << "Filósofo " << id_filosofo << " se ha levantado" << endl;
      }
   }
   

}
// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int id_propio, num_procesos_actual ;

   MPI_Init( &argc, &argv );

   // Conocer el identificador del proceso que actualmente se está ejecutando
   MPI_Comm_rank( MPI_COMM_WORLD, &id_propio );

   // Conocer el número total de procesos que hay en ejecución
   MPI_Comm_size( MPI_COMM_WORLD, &num_procesos_actual );


   if ( num_procesos == num_procesos_actual )
   {
      // ejecutar la función correspondiente a 'id_propio'
      if (id_propio == id_camarero)
         funcion_camarero();
      else if ( id_propio % 2 == 0 )          // si es par
         funcion_filosofos( id_propio ); //   es un filósofo
      else                               // si es impar
         funcion_tenedores( id_propio ); //   es un tenedor
   }
   else
   {
      if ( id_propio == 0 ) // solo el primero escribe error, indep. del rol
      { cout << "el número de procesos esperados es:    " << num_procesos << endl
             << "el número de procesos en ejecución es: " << num_procesos_actual << endl
             << "(programa abortado)" << endl ;
      }
   }

   MPI_Finalize( );
   return 0;
}

// ---------------------------------------------------------------------
