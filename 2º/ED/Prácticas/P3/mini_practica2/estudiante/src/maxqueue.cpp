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