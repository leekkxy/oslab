#define __LIBRARY__			//同whoami
#include<unistd.h>			//同whoami

_syscall1(int, iam,const char*,name);//同whoami
void main(int argc,char **argv) //接收的第一个参数是用户的名字，命令行读入
{
	if(argc!=2){				//判断参数情况，只接收一个参数
		printf("Usage: filename yourname\n")
		return -1;
	}

	}
iam(argv[1]);//将名字存入核心数据段内

}
