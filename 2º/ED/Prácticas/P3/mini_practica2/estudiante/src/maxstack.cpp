/**
 * @file maxstack.cpp
 * @brief  Archivo de implementación del TDA MaxStack
 * @author Mauricio Luque Jiménez
 */

#include <maxstack.h>

bool MaxStack::empty() const
{
    return cola.empty();
}

void MaxStack::push(const int & e)
{
    element aux;
    queue<element> cola_aux;

    aux.e = e;

    if(cola.empty()) aux.max = e;
    else
    {
        if(cola.front().max < e) aux.max = e;
        else aux.max = cola.front().max;
    }

    cola_aux.push(aux);

    while(!cola.empty())
    {
        cola_aux.push(cola.front());
        cola.pop();
    }

    cola = cola_aux;
}

void MaxStack::pop ()
{
    if (!cola.empty()) cola.pop();
}

element MaxStack::top() const
{
    return cola.front();
}

int MaxStack::size() const
{
    return cola.size();
}

/**************************************/
/* Funciones de prueba para el examen del 03/11  */ 
/**************************************/

MaxStack MaxStack::QuitaImpares(MaxStack & A)
{
    MaxStack aux, B;

    while(!A.empty())
    {
        int n = A.top().e;
        A.pop();

        if(n % 2 == 0) aux.push(n);
    }

    while(!aux.empty()) { B.push(aux.top().e); aux.pop(); }

    return B;
}

MaxStack MaxStack::RestaPositiva(MaxStack A, MaxStack B)
{
    MaxStack auxA, auxB, C;

    while(!A.empty()) { auxA.push(A.top().e); A.pop(); }
    while(!B.empty()) { auxB.push(B.top().e); B.pop(); }

    while(!auxA.empty() && !auxB.empty())
    {
        int resta = auxA.top().e - auxB.top().e;
        if(resta > 0) C.push(resta);
        
        auxA.pop();
        auxB.pop();
    }
    
    while(!auxA.empty())
    {
        int resta = auxA.top().e;
        if(resta > 0) C.push(resta);
        auxA.pop();
    }

    return C;
}

MaxStack MaxStack::Interseccion(MaxStack A, MaxStack B)
{
    MaxStack auxA, auxB, C;

    while(!A.empty()) { auxA.push(A.top().e); A.pop(); }
    while(!B.empty()) { auxB.push(B.top().e); B.pop(); }

    while(!auxA.empty())
    {
        int v = auxA.top().e;
        auxA.pop();

        MaxStack auxB_original;
        while(!auxB.empty())
        {
            int w = auxB.top().e;
            auxB.pop();
            
            if(v == w) { C.push(v); break; }
            else auxB_original.push(w);
        }

        while(!auxB_original.empty()) { auxB.push(auxB_original.top().e); auxB_original.pop(); }
    }

    return C;
}

MaxStack MaxStack::Alterna(MaxStack A, MaxStack B)
{
    MaxStack auxA, auxB, C;

    while(!A.empty()) { auxA.push(A.top().e); A.pop(); }
    while(!B.empty()) { auxB.push(B.top().e); B.pop(); }

    while(!auxA.empty() && !auxB.empty())
    {
        C.push(auxA.top().e); auxA.pop();
        C.push(auxB.top().e); auxB.pop();
    }

    while(!auxA.empty()) { C.push(auxA.top().e); auxA.pop(); }
    while(!auxB.empty()) { C.push(auxB.top().e); auxB.pop(); }

    return C;
}

MaxStack MaxStack::Compacta(MaxStack A)
{
    MaxStack aux, compactado;

    while(!A.empty()) { aux.push(A.top().e); A.pop(); }

    int v = aux.top().e;
    compactado.push(v);
    aux.pop();

    while(!aux.empty())
    {
        if(aux.top().e != v)
{
            v = aux.top().e;
            compactado.push(v); 
        }

        aux.pop();
    }

    return compactado;
}