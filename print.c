#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "execute.h"

static int buildArgs(LTree bro, char **agrs)
{
	int size=0;		//存储所有参数所需要的空间大小(字节)
	int pos=0;		//参数的位置(字节)
	char *ans;
	LTree t;
	Type re;		//临时存储参数树执行结果

	for(t=bro;t;t=t->bro)
		switch(t->returnType)
		{
		case INT: case TBOOL:
			size+=4; break;
		}
	size=(size+3)&(~3);		//扩容为4的倍数
	ans=(char *)malloc(size);
	for(t=bro;t;t=t->bro)
		switch(t->returnType)
		{
		case INT:
			re=do_solve(t);
			*(int *)(ans+pos)=re.intval; 
			pos+=4;
			break;
		case TBOOL:
			re=do_solve(t);
			*(char **)(ans+pos)=re.boolval==false ? "false" : "true";
			pos+=4;
			break;
		}
	*args=ans;

	return size;
}

void print(LTree ptree)
{
	LTree c=ptree->chi;
	char *fmt=c->val.strval;	//格式串
	char *args;					//参数值数组
	char *tail;
	int size1;					//参数数组的大小
	int size2;					//参数数组的大小的四分之一
	int size3;

	size1=buildArgs(c->bro,&args);
	size2=size/4;				//这是lodsd的循环次数
	tail=args+size1-4;			//这是esi的起始位置
	size3=size1+4;			//call printf前压栈的数据量,退栈时用
	__asm{
		push eax
		push esi
		mov  esi, tail
		std
loop:	lodsd
		push eax
		dec  size2
		jnz  loop
		push fmt
		call printf
		add  esp, size3
		pop  esi
		pop  eax
	}
}