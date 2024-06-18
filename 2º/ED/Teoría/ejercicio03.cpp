#include<iostream>
#include<stack>
using namespace std;
template<typename T>

class cola{
    private:
        stack<T> principal;
        stack<T> auxiliar;
    public:
        void push(T n){
          int size=principal.size();
          for(int i=0;i<size;i++){
              auxiliar.push(principal.top());
              principal.pop();
          }
          principal.push(n);
          for(int i=0;i<size;i++){
              principal.push(auxiliar.top());
              auxiliar.pop();
          }
        }
        void pop(){
            principal.pop();
        }
        T front(){
            return principal.top();
        }
        bool empty(){
            return principal.empty();
        }
};
int main(int argc, char *argv[]){
    cola<int> cola;

     for(int i = 1; i < argc; i++){
        if (argv[i][0] == '.'){
             cout << cola.front() << endl;
             cola.pop();
         } else {
             cola.push(atoi(argv[i]));
         }
     }
}