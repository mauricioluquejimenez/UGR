#include<iostream>
#include<queue>
using namespace std;
struct s{
    string apellidos;
    string nombre;
    int prioridad;
};

class compare{
    public:
        bool operator()(s s1, s s2){
            return s1.prioridad < s2.prioridad;
        }   
};
int main(int argc, char *argv[]){
    vector<s> S(3);
    priority_queue<s,vector<s>,compare> pq;
    for(int i=0;i<3;i++){
        S[i].nombre=argv[(3*i)+1];
        S[i].apellidos=argv[(3*i)+2];
        S[i].prioridad=atoi(argv[3*(1+i)]);
    }
	
	pq.push(S[0]);
	pq.push(S[1]);
	pq.push(S[2]);
    
    while(!pq.empty()){
        cout << pq.top().nombre << " " << pq.top().apellidos << endl;
        pq.pop();
    }
}