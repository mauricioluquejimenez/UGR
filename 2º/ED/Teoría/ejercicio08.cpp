#include<iostream>
#include<queue>
#include<stack>
#include<vector>
using namespace std;
bool palindromo(string c){
    queue<char> C;
    stack<char> P;
    bool palindromo=true;

    for(int i=0;i<c.size();i++)
        c[i]=tolower(c[i]);
    
    for(int i=0;i<c.size();i++){
        if(c[i] != ' '){
            C.push(c[i]);
            P.push(c[i]);
        }
    }
    while(palindromo&&!P.empty()){
        palindromo=(P.top()==C.front());
        C.pop();
        P.pop();
    }
    return palindromo; 
}
int main(int argc,char *argv[]){
    string c=argv[1];
        if(palindromo(c))
            cout << "La cadena '" << c << "' es un palíndromo" << endl;
        else
            cout << "La cadena '" << c << "' no es un palíndromo" << endl;
}