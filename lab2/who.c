#include <errno.h>
#include <asm/segment.h>
#include<linux/kernel.h>

char  kname[24]="";	//最大24个，因要求23个字符所以数组+1
int nbr = -1;   	//实际字符指针的最后位置，包括\0字符。-1表示空

int sys_iam(const char * name){
/***
完成的功能是将字符串参数name的内容拷贝到内核中保存下来。
**要求name的长度不能超过23个字符。返回值是拷贝的字符数。如果
**name的字符个数超过了23，则返回“-1”，并置errno为EINVAL。
***/
	char tmp[25];	//临时数组，暂存用户传来的数据。若数据不符合要求不改变name。
	int i;		//计数器，循环用

	for(i=0;i<24&&(tmp[i]=get_fs_byte(name+i));i++);	//循环读入name中字符数据到tmp。若长度大于23（i>=24）时或
								//name[i]结束时结束循环
	if(i<24&&!tmp[i])					//长度不大于23且最后一个字符为\0
	{
		int k;
		for (k=i;k>=0;k--)
			kname[k]=tmp[k];
		nbr= i;
		return i+1;
	}
	errno = EINVAL;
	return -1;
}

int sys_whoami (char * name,unsigned int size){
	/***
	它将内核中由iam()保存的名字拷贝到name指向的用户地址空间中，
	***同时确保不会对name越界访存（name的大小由size说明）。返回值是拷贝
	***的字符数。如果size小于需要的空间，则返回“-1”，并置errno为EINVAL。
	***/

	if(nbr+1>size){		//长度不够
		errno = EINVAL;
		return -1;
	}	
	int i= 0;
	for (;kname[i];i++)		//把内核段的数据复制到用户段
		put_fs_byte(kname[i],name+i);
	put_fs_byte('\0',name+i+1);	//字符串最后加\0
	return i+1;  
}

