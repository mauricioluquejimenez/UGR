#include<iostream>
#include<stack>
using namespace std;
template<typename T>
void insertar(stack<T>&P,int pos,const T &x){
    stack<T> auxiliar;
    int size=P.size();
    for(int i=0;i<size;i++){
        if(i==pos){
            auxiliar.push(x);
            size++;
        }else{
            auxiliar.push(P.top());
            P.pop();
        }
    }
    int size_a=auxiliar.size();
    for(int i=0;i<size_a;i++){
        P.push(auxiliar.top());
        auxiliar.pop();
    }
    for(int i=0;i<size;i++){
        cout << P.top() << " ";
        P.pop();
    }
    cout << endl;
}
int main(int argc,char *argv[]){
    stack<int> P;
    for(int i=3;i<argc;i++)
        P.push(atoi(argv[i]));
    insertar(P,atoi(argv[1]),atoi(argv[2]));
}