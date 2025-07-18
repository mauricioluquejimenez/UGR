#include<iostream>
#include<stack>
using namespace std;

template<typename T>
void modificar(stack<T> &P, const T &x, const T &y){
    stack<int> P2;
    P2.push(P.top());
    if(P2.top()==x){
        P2.pop();
        P2.push(y);
    }
    cout << P2.top() << endl;;
}

int main(){
    stack<int> P1;

    for(int i=0;i<10;i++){
        P1.push(rand()%10);
        cout << P1.top() << " ";
        modificar(P1,6,5);
    } 
}