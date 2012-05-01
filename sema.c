#include <stdio.h>
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

void verifySema(LTree node)
{
	LTree c1,c2;
	int t;

	switch(node->type)
	{
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
			sprintf(info,"%s���Ͳ��ܸ�ֵΪ%s����",findType(c1->returnType),findType(c2->returnType));
			warn();
		}
		node->returnType=c1->returnType;
		break;

	//����˫Ԫ������
	case '+': case '-': case '*': case '/': case '<': case '>':
		c1=node->chi;
		c2=c1->bro;
		if(c1->returnType!=INT || c2->returnType!=INT)
		{
			sprintf(info,"%c���㲻��Ӧ����%s������%s����",node->type,findType(c1->returnType),findType(c2->returnType));
			warn();
		}
		node->returnType=(node->type=='<' || node->type=='>') ? TBOOL : INT;
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