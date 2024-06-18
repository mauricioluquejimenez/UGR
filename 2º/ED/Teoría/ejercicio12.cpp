#include<iostream>
#include<queue>
using namespace std;
class compare{
    public:
        bool operator()(string a,string b){
            bool largo=true;
            if(a.size()>b.size())
                largo=true;
            else if(a.size()<b.size())
                largo=false;
            else{
                for(int i=0;i<a.size();i++){
                    if(a[i]>b[i])
                        largo=false;
                    else if(a[i]<b[i])
                        largo=true;
                }
            }
            return !largo;
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