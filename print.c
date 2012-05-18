#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "myparser.h"
#include "execute.h"

/*将print中的bro参数链转化为字节数组,存储在*args中,
返回总参数大小(单位:字节, 规约为4的倍数(push一次压栈4字节))*/
static int buildArgs(LTree bro, char **args)
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
			size+=sizeof(int); break;
		case TDOUBLE:
			size+=sizeof(double); break;
		}
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
		case TDOUBLE:
			re=do_solve(t);
			*(double *)(ans+pos)=re.doubleval;
			pos+=sizeof(double);
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
	int size1;					//参数数组的大小(字节)
	int size2;					//参数数组的大小的四分之一
	int size3;

	size1=buildArgs(c->bro,&args);
	size2=size1/4;				//这是lodsd的循环次数(如果是0,表示只打印fmt,没有参数,已经考虑到了)
	tail=args+size1-4;			//这是esi的起始位置
	size3=size1+4;			//call printf前压栈的数据量,退栈时用
	__asm{
		push eax
		push esi
		mov  esi, tail
		std
loop0:	cmp  size2,0
		je	 outloop
		lodsd
		push eax
		dec  size2
		jmp  loop0
outloop:
		cld
		push fmt
		call printf
		add  esp, size3
		pop  esi
		pop  eax
	}
	free(args);
}