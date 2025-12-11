#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <climits>
#include <cstdlib> 
#include <ctime> 

using namespace std;

/**
 * @brief Estructura que representa un estado en el juego de las cifras.
 */
struct Estado {
    int valor;
    string operaciones;

    Estado(int v = 0, string op = "") : valor(v), operaciones(op) {}
};

vector<Estado> GeneraOperaciones(const Estado &actual, int n) {
    vector<Estado> op;

    // Suma
    op.push_back(Estado(actual.valor + n,
        actual.operaciones + to_string(actual.valor) + " + " + to_string(n) + "\n"));

    // Resta (solo si no da negativo)
    if (actual.valor - n >= 0)
        op.push_back(Estado(actual.valor - n,
            actual.operaciones + to_string(actual.valor) + " - " + to_string(n) + "\n"));

    // Multiplicación
    op.push_back(Estado(actual.valor * n,
        actual.operaciones + to_string(actual.valor) + " * " + to_string(n) + "\n"));

    // División solo si es exacta y n != 0)
    if (n != 0 && actual.valor % n == 0)
        op.push_back(Estado(actual.valor / n,
            actual.operaciones + to_string(actual.valor) + " / " + to_string(n) + "\n"));

    return op;
}

//Caso imposible de encontrar solucion: la salida sería el numero mas cercano posible al que nos piden
bool mejor(const Estado &a, const Estado &b, int objetivo) {
    return abs(a.valor - objetivo) < abs(b.valor - objetivo);
}

void Cifras(multiset<int> S, int objetivo, Estado actual, Estado &best, int intento_jugador, bool &alcanzable) {
    if(actual.valor == intento_jugador) alcanzable = true;
    
    //si hemos llegado al numero terminamos
    if (actual.valor == objetivo) {
        best = actual;
        return;
    }
    //si actual está mas cerca que best, actualizamos
    if (mejor(actual, best, objetivo))
        best = actual;

    for (int n : S) {
        multiset<int> restantes = S;

        auto it = restantes.find(n);
        restantes.erase(it);

        vector<Estado> ops = GeneraOperaciones(actual, n);

        for (auto &op : ops)
            Cifras(restantes, objetivo, op, best, intento_jugador, alcanzable);
    }
}

int main() {

    srand(time(NULL));  

    vector<int> C = {1,2,3,4,5,6,7,8,9,10,25,50,75,100};

    // 6 números aleatorios 
    multiset<int> S;
    while (S.size() < 6) {
        int num = C[rand() % C.size()];
        S.insert(num);
    }

    // Objetivo aleatorio entre 100 y 999
    int objetivo = 100 + rand() % 900;
    int intento, intento_jugador;
    bool alcanzable = false;

    // Mostrar datos
    cout << " S = { ";
    for (int x : S){ 
        cout << x << " ";
    }
    cout << " } " << endl;
    cout << "Objetivo = " << objetivo << endl;

    cout << "\nIntroduce tu solución: ";
    cin >> intento;

    // Inicializo best a un valor muy lejano 
    Estado best(999999, ""); 

    // Probar cada número como inicio posible
    for (int n : S) {
        Estado inicial(n, "Empiezo con " + to_string(n) + "\n");

        multiset<int> resto = S;
        auto it = resto.find(n);
        resto.erase(it);

        Cifras(resto, objetivo, inicial, best, intento, alcanzable);
    }

    cout << "\n --> MEJOR SOLUCION \n";
    cout << "Valor obtenido: " << best.valor << endl;
    cout << "Diferencia con el objetivo: " << abs(best.valor - objetivo) << endl;

    cout << "\nOperaciones realizadas: " << endl;
    cout << best.operaciones << endl;

    cout << " --> TU SOLUCION: " << intento << endl;
    if(alcanzable) cout << "Diferencia con el objetivo: " << abs(intento - objetivo) << endl;
    else cout << "Tu solución no es alcanzable con los números dados." << endl;

    return 0;
}