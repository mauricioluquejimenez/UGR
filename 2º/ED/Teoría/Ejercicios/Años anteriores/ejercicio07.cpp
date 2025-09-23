#include<iostream>
#include<queue>
using namespace std;
template<typename T>
void insertar(queue<T> &P,int pos,const T &x){
    queue<T> auxiliar;
    int size=P.size();
    for(int i=0;i<size;i++){
        if(i==pos){
            auxiliar.push(x);
            size++;
        }else{
            auxiliar.push(P.front());
            P.pop();
        }
    }
    int size_a=auxiliar.size();
    for(int i=0;i<size_a;i++){
        P.push(auxiliar.front());
        auxiliar.pop();
    }
    for(int i=0;i<size;i++){
        cout << P.front() << " ";
        P.pop();
    }
    cout << endl;
}
int main(int argc, char *argv[]){
    queue<int> C;
    for(int i=3;i<argc;i++)
        C.push(atoi(argv[i]));
    insertar(C,atoi(argv[1]),atoi(argv[2]));
}