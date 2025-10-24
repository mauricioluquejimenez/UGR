/**
 * @file maxstack.cpp
 * @brief  Archivo de implementación del TDA MaxStack
 * @author
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