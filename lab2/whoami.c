#define __LIBRARY__ 									//使用unistd.h的条件
#include<unistd.h>										//包含了_syscall0的宏定义
#include<stdio.h>									
char tmp[25];		//用户存储名字的地方
_syscall2(int,whoami,char *,name,unsigned int,size);	//宏展开，相当于声明了whoami函数
void main(int argc,char* argv[]){						
whoami(tmp,25);						//调用whoami()系统调用，将核心数据中的内容cp到tmp数组里
printf("%s\n",tmp);
}
