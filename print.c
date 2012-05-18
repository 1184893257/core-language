#include <stdio.h>
#include <stdlib.h>
#include "print.h"
#include "myparser.h"
#include "execute.h"

/*��print�е�bro������ת��Ϊ�ֽ�����,�洢��*args��,
�����ܲ�����С(��λ:�ֽ�, ��ԼΪ4�ı���(pushһ��ѹջ4�ֽ�))*/
static int buildArgs(LTree bro, char **args)
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
	char *fmt=c->val.strval;	//��ʽ��
	char *args;					//����ֵ����
	char *tail;
	int size1;					//��������Ĵ�С(�ֽ�)
	int size2;					//��������Ĵ�С���ķ�֮һ
	int size3;

	size1=buildArgs(c->bro,&args);
	size2=size1/4;				//����lodsd��ѭ������(�����0,��ʾֻ��ӡfmt,û�в���,�Ѿ����ǵ���)
	tail=args+size1-4;			//����esi����ʼλ��
	size3=size1+4;			//call printfǰѹջ��������,��ջʱ��
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