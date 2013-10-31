/*
 * Created by idealnova
 * Version 0.02 2012-3-19
 * Only used for linux/unix/MAC OS
 * Open Source
 */
#include "_header.h"
#define USECS 1000000
#define SIZE 256
using namespace std;

struct rusage wait4_ru;

int check(const char * _name,const char * order){
    pid_t pid_c = -1;
    int status;
    pid_c=fork();
    if(pid_c==0){
        execl("./checker","checker",_name,order,(char*)0);
        perror("CHECKER ERROR!");
        exit(1);
    }else{
        wait4(pid_c, &status, 0, NULL);
    }
    return status;
}

int Compiler_JAVA(const char * _name){return 0;}
int Compiler_Python(const char * _name){return 0;}
int Compiler_Pascal(const char * _name){return 0;}
//......

int Compiler_CPP(const char * _name){
    int status;
    char name[SIZE]="./compiler ";
    strcat(name,_name);
    strcat(name," CPP");

    status=system(name);
    return (status==0?0:6);
}

int judger(int _time,long _byte,const char *_name,const char *prob,const char* ORD){
    int SYS_RE=0;
    pid_t pid = -1;
	int status = 0, wait4_options = 0;

    char name_in[SIZE]="\0";
    char name_out[SIZE]="\0";
    char name_exe[SIZE]="./";

    strcat(name_exe,_name);

    strcat(name_in,prob);
    strcat(name_in,ORD);
    strcat(name_in,".in");

    strcat(name_out,prob);
    strcat(name_out,ORD);
    strcat(name_out,".out");

    rlimit timeL,memoryL;
	getrlimit(RLIMIT_CPU,&timeL);                           //获得CPU时间限制峰值
	getrlimit(RLIMIT_AS,&memoryL);                          //获得物理内存峰值
	timeL.rlim_cur=_time;                                   //设置软限制, second
	memoryL.rlim_cur=_byte;                                 //设置软限制, byte

	pid = fork();
	if(pid == 0){       //子进程
	    freopen(name_in,"r",stdin);                         //重定向流
	    freopen(name_out,"w",stdout);

	    setrlimit(RLIMIT_CPU,&timeL);                       //限制时间
	    setrlimit(RLIMIT_AS,&memoryL);                      //限制内存
	    //ptrace(PTRACE_TRACEME, 0, NULL, NULL);            //跟踪进程信息
        execl(name_exe,_name,(char *)0);
        perror("Runtime Error!");
		exit(0);
	}else{              //父进程
        //获得跟踪状态
        wait4(pid, &status, wait4_options, &wait4_ru);
        //cout<<status<<endl;

        double  FIN_sec = 0.0;
        long    FIN_mem = 0;
        FIN_sec=(double)wait4_ru.ru_utime.tv_sec+
                (double)wait4_ru.ru_utime.tv_usec/USECS;
        FIN_mem=wait4_ru.ru_maxrss+     //RSS
                wait4_ru.ru_ixrss+      //代码段
                wait4_ru.ru_idrss+      //数据段
                wait4_ru.ru_isrss;      //栈

        switch(status){
            case(0):
                if(check(_name,ORD)==0){
                    SYS_RE=0;
                    cout<<"AC!\n";
                }else{
                    SYS_RE=4;
                    cout<<"WA!\n";
                }
                cout<<fixed<<setprecision(3)<<FIN_sec<<" s"<<endl;
                cout<<FIN_mem<<" KB"<<endl;
                return SYS_RE;
            break;
            case(24):
                SYS_RE=1;
                cout<<"TLE!"<<endl;
                return SYS_RE;
            break;
            case(32512):
                SYS_RE=2;
                cout<<"MLE!"<<endl;
                return SYS_RE;
            break;
            default:
                SYS_RE=3;
                cout<<"RTE!"<<endl;
                cout<<fixed<<setprecision(3)<<FIN_sec<<" s"<<endl;
                cout<<FIN_mem<<" KB"<<endl;
                return SYS_RE;
            break;
        }
    }
}

int main(int argc, char **argv){
    //程序名 程序类型 题目名 序号 时限 内限
    int sys_re=0;
    sys_re=Compiler_CPP(argv[1]);

    char *remain1;
    char *remain2;

    int limit_time=strtol(argv[5],&remain1,0);
    int limit_mem=strtol(argv[6],&remain2,0);
    if(limit_time<1 || limit_mem<1048576){
        cerr<<"Too rigorous limit! System cannot begin!"<<endl;
        return -1;
    }

    if(sys_re!=6)
        sys_re=judger(limit_time,limit_mem,argv[1],argv[3],argv[4]);

	return sys_re;
	return 0;
}
