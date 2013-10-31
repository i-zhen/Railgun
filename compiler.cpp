#include"_header.h"
#define SIZE 256
using namespace std;

int main(int argc,char **argv){
    pid_t pid_C = -1;
    int status = 0;
    string comp_info="";
    char name[SIZE]="g++ -O2 -o ";
    strcat(name,argv[1]);
    strcat(name," ");
    strcat(name,argv[1]);
    strcat(name,".cpp");
    strcat(name," 2> compiler.out");

    pid_C=fork();
    if(pid_C==0){
        system(name);
        exit(0);
    }else{
        wait4(pid_C, &status, 0, NULL);
        if(!status){
            ifstream fin("compiler.out");
            fin>>comp_info;
            if(comp_info=="") return 0;
            else return -1;
            fin.close();
        }else
            return -1;
    }
}
