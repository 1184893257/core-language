#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "execute.h"

static int buildArgs(LTree bro, char **agrs)
{
	int size=0;		//�洢���в�������Ҫ�Ŀռ��С(�ֽ�)
	int pos=0;		//������λ��(�ֽ�)
	char *ans;
	LTree t;
	Type re;		//��ʱ�洢������ִ�н��

	for(t=bro;t;t=t->bro)
		switch(t->returnType)
		{
		case INT: case TBOOL:
			size+=4; break;
		}
	size=(size+3)&(~3);		//����Ϊ4�ı���
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
	char *fmt=c->val.strval;	//��ʽ��
	char *args;					//����ֵ����
	char *tail;
	int size1;					//��������Ĵ�С
	int size2;					//��������Ĵ�С���ķ�֮һ
	int size3;

	size1=buildArgs(c->bro,&args);
	size2=size/4;				//����lodsd��ѭ������
	tail=args+size1-4;			//����esi����ʼλ��
	size3=size1+4;			//call printfǰѹջ��������,��ջʱ��
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