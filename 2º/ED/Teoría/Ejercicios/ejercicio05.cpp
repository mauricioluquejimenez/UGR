#include<iostream>
using namespace std;

pair<float,bool>evalua(string expresion,pair<char,float>*variables,int num_variables){
    for(i=0;i<expresion.size();i++){
        if((expresion[i]>=0)&&(expresion[i]))
            cout << 0;
        else{
            switch(expresion[i]){
                case '+': expresion[i]+variables->second;break;
                case '-': expresion[i]+variables->second;break;
                case '*': expresion[i]+variables->second;break;
                case '/': expresion[i]+variables->second;break;

            }
        }
    }
}
int main(int argc,char *argv[]){
    
}