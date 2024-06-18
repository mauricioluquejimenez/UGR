#include<iostream>
#include<queue>
using namespace std;
template<typename T>

class pila{
    private:
        queue<T> principal;
        queue<T> auxiliar;
    public:
        void push(T n){
            int size=principal.size();
            auxiliar.swap(principal);
            principal.push(n);

            for(int i=0;i<size;i++){
                principal.push(auxiliar.front());
                auxiliar.pop();
            }
        }
        void pop(){
            principal.pop();
        }
        T top(){
            return principal.front();
        }
        bool empty(){
            return principal.empty();
        }
};
int main(int argc, char *argv[]){
    pila<int> pila;

    for(int i = 1; i < argc; i++){
        if (argv[i][0] == '.'){
            cout << pila.top() << endl;
            pila.pop();
        } else {
            pila.push(atoi(argv[i]));
        }
    }
}