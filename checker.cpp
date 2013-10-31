#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#define SIZE 256
using namespace std;

char temp;
char _name1[SIZE];
char _name2[SIZE]="ans";

int main(int argc,char **argv){
	string	str_1="", str_2="";
	
	strcat(_name1,argv[1]);
	strcat(_name1,argv[2]);
	strcat(_name1,".out");

	ifstream fin_1(_name1);
	while((temp=fin_1.get())!=EOF) str_1+=temp;
	fin_1.close();

	strcat(_name2,argv[2]);
	strcat(_name2,".out");

	ifstream fin_2(_name2);
	while((temp=fin_2.get())!=EOF) str_2+=temp;
	fin_2.close();
	
	if(str_1!=str_2) return -1;
	return 0;
}
