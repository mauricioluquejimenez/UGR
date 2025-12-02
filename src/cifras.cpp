#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <climits>

using namespace std;

//guardar estado
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

//Caso imposible de encontrar solucion: la salida sería el numero mas
//  cercano posible al que nos piden
bool mejor(const Estado &a, const Estado &b, int objetivo) {
    return abs(a.valor - objetivo) < abs(b.valor - objetivo);
}


void Cifras(set<int> S, int objetivo, Estado actual, Estado &best) {
    //si hemos llegado al numero terminamos
    if (actual.valor == objetivo) {
        best = actual;
        return;
    }
    //si actual está mas cerca que best, actualizamos
    if (mejor(actual, best, objetivo))
        best = actual;

    for (int n : S) {
        set<int> restantes = S;
        restantes.erase(n);

        vector<Estado> ops = GeneraOperaciones(actual, n);

        for (auto &op : ops)
            Cifras(restantes, objetivo, op, best);
    }
}

int main(){
    
}

