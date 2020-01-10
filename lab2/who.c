#include <errno.h>
#include <asm/segment.h>
#include<linux/kernel.h>

char  kname[24]="";
int nbr = -1;

int sys_iam(const char * name){
/***完成的功能是将字符串参数name的内容拷贝到内核中保存下来。要求name的长度不能超过23个字符。返回值是拷贝的字符数。如果name的字符个数超过了23，则返回“-1”，并置errno为EINVAL。
***/
	char tmp[25];	//临时数组，暂存用户传来的数据。若数据不符合要求不改变name。
	int i;		//计数器，循环用

	for(i=0;i<25;i++){
		tmp[i]=get_fs_byte(name+i);
		printk("%d",i);
		if(!tmp[i])
			break;
	}
	if(i>23||(i==23&&tmp[23])){
		return -(EINVAL);
	}
		int k;
		for (k=i;k>=0;k--)
			kname[k]=tmp[k];
		nbr= i-1;
		return i;
}

int sys_whoami (char * name,unsigned int size){
	/****它将内核中由iam()保存的名字拷贝到name指向的用户地址空间中，同时确保不会对name越界访存（name的大小由size说明）。返回值是拷贝的字符数。如果size小于需要的空间，则返回“-1”，并置errno为EINVAL。****/

	if(nbr+1>size)
		return -(EINVAL);
	int i= 0;
	for (;kname[i];i++)
		put_fs_byte(kname[i],name+i);
	put_fs_byte('\0',name+i+1);
	return i;
}

