#include <stdio.h>
#include <stdlib.h>
#include "sema.h"
#include "myparser.h"

extern int linenum;		//��lexԴ�ļ��ж����,��ǰ�Ǻ�������
extern char linebuf[];	//lex�ж���
extern int errors;		//yacc�ж���

static char info[1024];		//���������Ϣ

//������徯��
static void warn()
{
	printf("line %d:<�������>%s\n%s\n",linenum+1,info,linebuf);
	errors++;
}

//�����������ת��Ϊ�ַ�������ʽ
static char* findType(int type)
{
	char *s;
	switch(type)
	{
	case INT: s="INT"; break;
	case TBOOL: s="TBOOL"; break;
	case ARRAY: s="ARRAY"; break;
	case NORETURN: s="�޷���ֵ"; break;
	default: s="**"; printf("����:%d��δ֧��\n",type);
	}
	return s;
}

//������Ĳ�������ת��Ϊ�ַ�������ʽ
static char* findOp(int type)
{
	char *s;
	switch(type)
	{
	case NEG: s="ȡ��"; break;
	case '!': s="ȡ��"; break;
	case EQUAL: s="�е�"; break;
	case AND: s="��"; break;
	case OR: s="��"; break;
	case '<': s="<"; break;
	case '>': s=">"; break;

	case WHILE: s="whileѭ��"; break;
	case IFU: s="����ȫƥ�������ѡ��"; break;
	case IFF: s="��ȫƥ�������ѡ��"; break;
	}
	return s;
}

//�ж����ķ���ֵ�ǲ�����ֵ�͵�
static bool isNUM(LTree node)
{
	int type=node->returnType;
	return type==INT || type==TDOUBLE;
}

//�Ӹ�����ת���ڵ�
static LTree CHANGETO(LTree node,int type)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->bro=NULL;
	ans->type=CHANGE;
	ans->returnType=type;
	ans->chi=node;
	return ans;
}

//��ֵ�͵�������ת��
static bool compatible(LTree *pc1,LTree *pc2)
{
	LTree c1=*pc1,c2=*pc2;
	if(!isNUM(c1) || !isNUM(c2)) return false;
	if(c1->returnType==c2->returnType) return true;
	if(c1->returnType==INT && c2->returnType==TDOUBLE)
	{
		*pc1=CHANGETO(c1,TDOUBLE);
		c1->bro=NULL;
	}
	else if(c1->returnType==TDOUBLE && c2->returnType==INT)
		*pc2=CHANGETO(c2,TDOUBLE);
	return true;
}

void verifySema(LTree node)
{
	LTree c1,c2;
	int t;

	switch(node->type)
	{
	case CHANGE:		//ǿ������ת�������һ����
		c1=node->chi;
		c2=c1->bro;
		node->returnType=c1->type;
		node->chi=c2;
		free(c1);
		break;

	case MAIN: case MULTI2:	case PRINT:	//�����������,�﷨ͨ���������û����
		break;

	case WHILE: case IFF: case IFU:		//����������һ�������ǲ��ǲ����͵�
		c1=node->chi;
		if(c1->returnType!=TBOOL)
		{
			sprintf(info,"%s���Ͳ�����Ϊ%s�����ж�����",findType(c1->returnType),findOp(node->type));
			errors++;
			printf("<�������>%s\n",info);
		}
		break;

	//��ֵ���ͼ��
	case '=':
		c1=node->chi;
		c2=c1->bro;
		if(c1->returnType!=c2->returnType)	//����new����������������ֵĸ�ʽ����id[I]
		{									//(id��������������������������,���������ͻᱨ���Ҳ�������)
			if(isNUM(c1) && isNUM(c2))//���������
			{
				c2=CHANGETO(c2,c1->returnType);
				c1->bro=c2;
			}
			else
			{
				sprintf(info,"%s���Ͳ��ܸ�ֵΪ%s����",findType(c1->returnType),findType(c2->returnType));
				warn();
			}
		}
		node->returnType=c1->returnType;
		break;

	//����˫Ԫ������
	case '+': case '-': case '*': case '/': case '<': case '>':
		c1=node->chi;
		c2=c1->bro;
		if(!compatible(&c1,&c2))
		{
			sprintf(info,"%c���㲻��Ӧ����%s������%s����",node->type,findType(c1->returnType),findType(c2->returnType));
			warn();
		}
		else
		{
			node->chi=c1;
			c1->bro=c2;
		}
		node->returnType=(node->type=='<' || node->type=='>') ? TBOOL : c1->returnType;
		break;

	//��Ԫ������
	case NEG: case '!':
		t=node->type==NEG ? INT : TBOOL;
		c1=node->chi;
		if(c1->returnType!=t)
		{
			sprintf(info,"���㲻��Ӧ����%s����",findOp(node->type),findType(c1->returnType));
			warn();
		}
		node->returnType=t;
		break;

	//����������
	case OR: case AND:
		c1=node->chi;
		c2=c1->bro;
		if(c1->returnType!=TBOOL || c2->returnType!=TBOOL)
		{
			sprintf(info,"%s���㲻��Ӧ����%s������%s����",findOp(node->type),findType(c1->returnType),findType(c2->returnType));
			warn();
		}
		node->returnType=TBOOL;		//Ϊ�˾�ȷ��λ����,һ��ڵ�ķ���ֵ���Ǹ�ֵΪ��ȷ��
		break;
	
	//�е��������INT,INT��TBOOL,TBOOL��϶�����ȷ��
	case EQUAL:
		c1=node->chi;
		c2=c1->bro;
		if(c1->returnType!=c2->returnType)
		{
			sprintf(info,"%s���㲻��Ӧ����%s������%s����",findOp(node->type),findType(c1->returnType),findType(c2->returnType));
			warn();
		}
		node->returnType=TBOOL;		//Ϊ�˾�ȷ��λ����,һ��ڵ�ķ���ֵ���Ǹ�ֵΪ��ȷ��
		break;
	}
}