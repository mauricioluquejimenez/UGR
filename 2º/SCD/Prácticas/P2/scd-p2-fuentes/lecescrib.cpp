#include <iostream>
#include <iomanip>
#include <cassert>
#include <random>
#include <thread>
#include "scd.h"

using namespace std ;
using namespace scd ;

// Número de lectores y escritores
const int num_lectores = 3, num_escritores = 3 ;

// Mutex para controlar la salida por pantalla
mutex m;

class LecEscrib : public HoareMonitor
{
    private:
        // Número de lectores leyendo
        int n_lec;

        // ¿Hay un escritor escribiendo?
        bool escrib;

        // No hay lectores/escritores leyendo/escribiendo, se puede escribir/leer
        CondVar lectura, escritura;

    public:
        LecEscrib();
        void ini_lectura();
        void fin_lectura();
        void ini_escritura();
        void fin_escritura();
};

LecEscrib::LecEscrib()
{
    // Inicialmente no hay lectores leyendo y ningún escritor está escribiendo
    n_lec = 0;
    escrib = false;

    lectura = newCondVar();
    escritura = newCondVar();
}

void LecEscrib::ini_lectura()
{
    // Si hay un escritor escribiendo, esperar
    if (escrib) lectura.wait();

    // Cuando no hay escritores escribiendo, se puede leer: se incrementa el número de lectores
    n_lec++;
    lectura.signal();
}

void LecEscrib::fin_lectura()
{
    // Se decrementa el número de lectores: un lector ha terminado de leer
    n_lec--;

    // Si no hay lectores leyendo, se puede escribir
    if(n_lec == 0) escritura.signal();
}

void LecEscrib::ini_escritura()
{
    // Si no hay lectores leyendo o hay un escritor escribiendo, esperar
    if(n_lec > 0 || escrib) escritura.wait();

    // Cuando no hay lectores leyendo y no hay un escritor escribiendo, se puede escribir
    escrib = true;
}

void LecEscrib::fin_escritura()
{
    // Cuando se termina de escribir, escrib se vuelve a false
    escrib = false;

    // Si hay lectores leyendo, se puede leer
    if(!lectura.empty()) lectura.signal();

    // Si no hay lectores leyendo, se puede escribir
    else escritura.signal();
}

void funcion_hebra_lectora(MRef<LecEscrib> monitor, int i)
{
    while(true)
    {
        // Se empieza a leer
        monitor->ini_lectura();

        m.lock();
        cout << "Lector " << i << " leyendo." << endl;
        m.unlock();

        // Se termina de leer
        monitor->fin_lectura();
        
        m.lock();
        cout << "Lector " << i << " termina de leer." << endl;
        m.unlock();
    }
}

void funcion_hebra_escritora(MRef<LecEscrib> monitor, int i)
{
    while(true)
    {
        // Se empieza a escribir
        monitor->ini_escritura();

        m.lock();
        cout << "Escritor " << i << " escribiendo." << endl;
        m.unlock();

        // Se termina de escribir
        monitor->fin_escritura();

        m.lock();
        cout << "Escritor " << i << " termina de escribir." << endl;
        m.unlock();
    }
}

int main()
{
    // Se crea el monitor
    MRef<LecEscrib> monitor = Create<LecEscrib>();

    // Se crean las hebras de los lectores y los escritores
    thread hebra_lectora[num_lectores];
    thread hebra_escritora[num_escritores];

    // Se lanzan las hebras
    for(int i = 0; i < num_lectores; i++) hebra_lectora[i] = thread(funcion_hebra_lectora, monitor, i);
    for(int i = 0; i < num_escritores; i++) hebra_escritora[i] = thread(funcion_hebra_escritora, monitor, i);

    // Se espera a que terminen las hebras (nunca en este caso)
    for(int i = 0; i < num_lectores; i++) hebra_lectora[i].join();
    for(int i = 0; i < num_escritores; i++) hebra_escritora[i].join();
}