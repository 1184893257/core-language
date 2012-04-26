#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "struct.h"
#include "myparser.h"

LTree newBOOL(bool val)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->type=BOOL;
	ans->chi=ans->bro=NULL;
	ans->returnType=TBOOL;
	ans->val.boolval=val;
	return ans;
}

LTree newNUM(int num)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->type=NUM;
	ans->chi=ans->bro=NULL;
	ans->returnType=INT;
	ans->val.intval=num;
	return ans;
}

LTree newSTRING(char *s)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->type=STRING;
	ans->chi=ans->bro=NULL;
	ans->returnType=STRING;
	ans->val.strval=strdup(s);
	return ans;
}

LTree newID(char *name)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->type=ID;
	ans->chi=ans->bro=NULL;
	//ans->returnType=INT;		//���Ҫ���õ�ʱ������
	ans->val.strval=strdup(name);
	return ans;
}

LTree buildTree(int type, ...)
{
	va_list args;	//�ɱ����
	int n;			//��������,����type���ж�
	LTree ans;		//���Ҫ���ɵ���
	int i;
	LTree *tail;

	//����typeȷ�������ĸ���
	switch(type)
	{
	case '+': n=2; break;
	}
	
	ans=(LTree)malloc(sizeof(Tree));
	ans->type=type;
	ans->bro=ans->chi=NULL;
	tail=&ans->chi;
	
	args=va_start(args,type);
	for(i=0;i<n;++i)
	{
		*tail=va_arg(args,LTree);
		tail=&(*tail)->bro;
	}
	va_end(args);

	return ans;
}