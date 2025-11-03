/**
 * @file maxqueue.cpp
 * @brief  Archivo de implementación del TDA MaxQueue
 * @author Mauricio Luque Jiménez
 */

#include "maxqueue.h"

bool MaxQueue::empty() const
{
    return pila.empty();
}

void MaxQueue::push(const int & e)
{
    element aux;
    stack<element> pila_aux;

    aux.e = e;
    aux.max = aux.e;

    while(!pila.empty())
    {
        if(pila.top().max < e) pila.top().max = e;
        pila_aux.push(pila.top());
        pila.pop();
    }

    pila_aux.push(aux);
    
    while(!pila_aux.empty())
    {
        pila.push(pila_aux.top());
        pila_aux.pop();
    }
}

void MaxQueue::pop()
{
    if(!pila.empty()) pila.pop();
}

element MaxQueue::front() const
{
    return pila.top();
}

int MaxQueue::size() const
{
    return pila.size();
}

/**************************************/
/* Funciones de prueba para el examen del 03/11  */ 
/**************************************/

MaxQueue MaxQueue::Maximos(MaxQueue q, int n)
{
    MaxQueue aux, maximos;
    
    for(int i = 0; i < n; i++)
    {
        aux.push(q.front().e);
        q.pop();
    }

    maximos.push(aux.front().max);

    while(!q.empty())
    {
        aux.pop();
        aux.push(q.front().e);
        q.pop();
        maximos.push(aux.front().max);
    }
    
    return maximos;
}