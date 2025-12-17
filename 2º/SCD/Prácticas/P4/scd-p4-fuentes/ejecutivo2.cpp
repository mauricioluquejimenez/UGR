// -----------------------------------------------------------------------------
//
// Sistemas concurrentes y Distribuidos.
// Práctica 4. Implementación de sistemas de tiempo real.
//
// Archivo: ejecutivo2.cpp
// Cálculo del tiempo real de ejecución y retraso:
//
//   Datos de las tareas:
//   ------------
//   Ta.  C    T
//   ------------
//   A  80   400
//   B  40   400
//   C  100  800
//   D  50   200
//  -------------
//  El ciclo principal tiene una duración Tm = 800 ms (mcm(200,400,400,800) = 800).
//
//  Planificación (con Ts == 200 ms (max(C) <= Ts <= min(T)) )
//
//  Tm / Ts = 4 iteraciones del ciclo secundario
//
//  *---------*---------*---------*---------*
//  | A     D |   B C D | A     D |   B   D |
//  *---------*---------*---------*---------*
//
// ¿Cuál es el mínimo tiempo de espera que queda al final de las iteraciones del ciclo secundario con tu solución?
//  El tiempo mínimo de espera es 10ms, en la iteración 2 del ciclo secundario
//  T(B) + T(C) + T(D) = 40ms + 100ms + 50ms = 190ms ; 200ms - 190ms = 10ms
//
// ¿Cuál es el máximo tiempo de espera que queda al final de las iteraciones del ciclo secundario con tu solución?
//  El tiempo máximo de espera es 110ms, en la iteración 4 del ciclo secundario
//  T(B) + T(D) = 40ms + 50ms = 90ms ; 200ms - 90ms = 110ms
// 
// Historial:
// Creado en Diciembre de 2017
// -----------------------------------------------------------------------------

#include <string>
#include <iostream> // cout, cerr
#include <thread>
#include <chrono>   // utilidades de tiempo
#include <ratio>    // std::ratio_divide

using namespace std ;
using namespace std::chrono ;
using namespace std::this_thread ;

// tipo para duraciones en segundos y milisegundos, en coma flotante:
//typedef duration<float,ratio<1,1>>    seconds_f ;
typedef duration<float,ratio<1,1000>> milliseconds_f ;

// -----------------------------------------------------------------------------
// tarea genérica: duerme durante un intervalo de tiempo (de determinada duración)

void Tarea( const std::string & nombre, milliseconds tcomputo )
{
   cout << "   Comienza tarea " << nombre << " (C == " << tcomputo.count() << " ms.) ... " ;
   sleep_for( tcomputo );
   cout << "fin." << endl ;
}

// -----------------------------------------------------------------------------
// tareas concretas del problema:

void TareaA() { Tarea( "A", milliseconds( 80) );  }
void TareaB() { Tarea( "B", milliseconds( 40) );  }
void TareaC() { Tarea( "C", milliseconds(100) );  }
void TareaD() { Tarea( "D", milliseconds( 50) );  }

// -----------------------------------------------------------------------------
// implementación del ejecutivo cíclico:

int main( int argc, char *argv[] )
{
   // Ts = duración del ciclo secundario (en unidades de milisegundos, enteros)
   const milliseconds Ts_ms( 200 );

   // ini_sec = instante de inicio de la iteración actual del ciclo secundario
   time_point<steady_clock> ini_sec = steady_clock::now();

   while( true ) // ciclo principal
   {
      cout << endl
           << "---------------------------------------" << endl
           << "Comienza iteración del ciclo principal." << endl ;

      for( int i = 1 ; i <= 4 ; i++ ) // ciclo secundario (4 iteraciones)
      {
         cout << endl << "Comienza iteración " << i << " del ciclo secundario." << endl ;

         switch( i )
         {
            case 1 : TareaA();                     TareaD(); break ;
            case 2 :           TareaB(); TareaC(); TareaD(); break ;
            case 3 : TareaA();                     TareaD(); break ;
            case 4 :           TareaB();           TareaD(); break ;
         }

         // calcular el siguiente instante de inicio del ciclo secundario
         ini_sec += Ts_ms ;

         // esperar hasta el inicio de la siguiente iteración del ciclo secundario
         sleep_until( ini_sec );

         time_point<steady_clock> fin_sec = steady_clock::now();
         milliseconds_f retraso = duration_cast<milliseconds_f>(fin_sec - ini_sec);
         milliseconds_f duracion_real = Ts_ms + retraso;

         cout << "Duracion esperada: " << Ts_ms.count() << endl;
         cout << "Duracion real: " << duracion_real.count() << endl;
         cout << "Retraso: " << retraso.count() << endl;
      }
   }
}
