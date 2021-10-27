#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(){
    ifstream f;
    f.open("./test.txt");
    int counter=0;
    string buff;
    while(getline(f,buff)){
        cout<<buff.size()<<endl;
    }
}