#include<iostream>
#include<stack>
using namespace std;
bool parentizada(string expresion){
    return((expresion[0]=='(')&&(expresion[expresion.size()-1]==')'));
}
int main(){
    string expresion;
    cout << "Introduzca una expresión con una configuración de paréntesis correcta: ";
    cin >> expresion;
    if(parentizada(expresion))
        cout << "Bien introducidos" << endl;
    else
        cout << "Mal introducidos" << endl;
}