#include<iostream>
#include<queue>
using namespace std;
int getVocales(string cadena){
    int contador=0;
    for(int i=0;i<cadena.size();i++){
        switch (cadena[i]){
            case('a'):contador++;
            case('A'):contador++;
            case('e'):contador++;
            case('E'):contador++;
            case('i'):contador++;
            case('I'):contador++;
            case('o'):contador++;
            case('O'):contador++;
            case('u'):contador++;
            case('U'):contador++;

            break;
        }
    }
    return contador;
}
class compare{
    public:
        bool operator()(string a,string b){
            return(getVocales(a)<=getVocales(b));
        } 
};

int main(int argc, char *argv[]){
    vector<string> VC(argc);
    priority_queue<string,vector<string>,compare> C;

    for(int i=1;i<argc;i++){
        VC[i-1]=argv[i];
        C.push(VC[i-1]);
    }
    while(!C.empty()){
        cout << C.top() << " ";
        C.pop();
    }
}