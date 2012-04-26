%{
#include "mylexer.h"
#include "struct.h"
#define YYSTYPE LTree
%}

%token	ID				//��������
%token	NUM BOOL STRING		//���ǳ�������
%token	TBOOL INT			//�ⶼ�Ǳ�������
%token	EQUAL AND OR		//���ַ��Ĳ�����

%%

L	:
	|	L '='	{printf("��ֵ\n");}
	|	L NUM	{printf("NUM:%d\n",$2->val.intval);}
	|	L ID	{printf("ID\n");}
	|	L BOOL	{printf("BOOL\n");}
	|	L '+'	{printf("add\n");}
	|	L ';'	{printf("end of a exp\n");}
	|	L STRING	{$$=$1;}
	|	L INT	{printf("val type:int\n");}
	|	L TBOOL	{printf("val type:bool\n");}
	;

//T��һ�����
T	:	D		//�������
	|	I		//����ֵΪ���������
	|	B		//����ֵΪ���������
	|	AR		//����ֵΪ��������
	|	IFELSE	//if else �������
	|	WHILE	//while ѭ�����
	|	PRINT	//print���
	;
	
TYPE	:
		| TBOOL
		| INT
		;

%%

int main(void)
{
	//yylex();
	yyparse();
	return 0;
}

