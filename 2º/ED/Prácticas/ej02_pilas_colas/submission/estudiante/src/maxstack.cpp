/**
 * @file maxstack.cpp
 * @brief  Archivo de implementación del TDA MaxStack
 * @author  Mauricio Luque Jimenez, Salma EZzamarty
 */
#include <iostream>
#include <maxstack.h>

using namespace std;

void MaxStack::push (const int &elemento){
    element e;
    queue<element> cola_aux;

    e.elem = elemento;

    if(cola.empty()) {
        e.maximum = elemento;
    }
    else{
        if(cola.front().maximum < elemento)
            e.maximum = elemento;
        else
            e.maximum = cola.front().maximum;
    }

    cola_aux.push(e);

    while (!cola.empty()){
        cola_aux.push(cola.front());
        cola.pop();
    }
    cola = cola_aux;
}
void MaxStack::pop (){
    if (!cola.empty()){
        cola.pop();
    }
}

element MaxStack::top(){
    return cola.front();
}
bool MaxStack::isEmpty() const{
    return cola.empty();
}

int MaxStack::size() const{
    return cola.size();
}


