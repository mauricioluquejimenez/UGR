/**
 * @file maxqueue.cpp
 * @brief  Archivo de implementación del TDA MaxQueue
 * @author  Mauricio Luque Jimenez, Salma Ezzamarty
 */

#include "maxqueue.h"

void MaxQueue::push(const int &elemento){
    element e;
    stack<element> pila_aux;
    e.elem = elemento;
    e.maximum = e.elem;

    while(!pila.empty()){
        if(pila.top().maximum < elemento)
            pila.top().maximum = elemento;
        pila_aux.push(pila.top());
        pila.pop();
    }

    pila_aux.push(e);
    while(!pila_aux.empty()) {
        pila.push(pila_aux.top());
        pila_aux.pop();
    }
}

void MaxQueue::pop(){
    if(!pila.empty())
        pila.pop();
}
int MaxQueue::size() const{
    return pila.size();
}
element MaxQueue::front(){
    return pila.top();
}
bool MaxQueue::isEmpty() const{
    return pila.empty();
}
