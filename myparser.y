%{
#include <stdio.h>
#include "mylexer.h"
#include "struct.h"
#include "id.h"
#include "init.h"
#include "execute.h"
#define YYSTYPE LTree
LTree root;			//�﷨���ĸ��ڵ�
extern int linenum;
%}

%token	ID				//��������
%token	NUM BOOL STRING		//���ǳ�������
%token	TBOOL INT ARRAY		//�ⶼ�Ǳ�������,����������
%token	EQUAL AND OR NEW	//���ַ��Ĳ�����
%token	IF ELSE WHILE		//����ʶ���֧��ѭ�����
%token	NEG IFU IFF 		//����ʶ�𸺺š�������if������if
%token	MULTI MULTI2		//�������½ڵ�,ֻ�ǰѺ�������������(MULTI),
							//�ṩһ���½ڵ㺢�����Ǹ������(MULTI2)
%token	PRINT				//�ṩprint����
%token	MAIN				//root�Ľڵ�����
%token	NOP					//�����,��ȫ��if else��,���if������ǿ����,
							//�Ͷ���,������else��������,�������ø������

%right	'='
%left	OR
%left	AND 
%left	EQUAL
%left	'<' '>'
%left	'+' '-'
%left	'*' '/'
%right	'!' uminus

%%
//���������,���Ƕ������
S:	D M	{root=buildTree(MAIN,buildTree(MULTI2,$2));}		//�����﷨���͵�����O(��_��)O����~
	;
	
D:
	|D TYPE ID ';'			{declare($2,$3);}
	|D TYPE '[' ']' ID ';'	{declareArray($2,$5);}
	;
 
TYPE:INT	{$$=newTYPE(INT);}
	|TBOOL	{$$=newTYPE(TBOOL);}
	;

/*3������:�����(ֻ��һ���ֺ�), �������, �������, 
��3�ֶ��ǿ�����Ϊif��while��ִ�п��
*/
E:	';'			{$$=newNOP();}		//�����(Ϊ������������,ִ��ʱ����)
	|Q								//�������
	|'{' M '}'	{$$=buildTree(MULTI2,$2);}			//�������
	;

Q:	I ';'	//���λ򲼶�����
	|F		//IF���
	|W		//WHILE���
	|P		//print���
	|A 		//new����
//	//|error ER ';'	{fprintf(stderr,"line:%d:�������ϳ����﷨����!",linenum);}		//������д�����
	//|error	{printf("I catched a error");}		//������д�����
	;
	
A:	ID '=' NEW '[' I ']' ';' 	{$$=newArray($1,$5);}
	;
	/*
ER:	
	|ER T
	;

//���г��ֺ����ڵ���������ܳ��ֵ��ս��,����error�Ե�
T:	EQUAL|AND|OR|'!'	//���������
	|'+'|'-'|'*'|'/'	//��������
	|'<'|'>'
	|'('|')'|'['|']'
	|'='|':'|','
	|TYPE					{printf("line:%d:���������������ǰ��\n",linenum);}
	|BOOL|NUM|ID|STRING
	|PRINT|IF|ELSE
	;*/

//�������
M:				{$$=NULL;}
	|M Q		{$$=buildTree(MULTI,$1,$2);}
	;

V:	ID
	;

//�����������
I:	ID '=' I		{refer($1); $$=buildTree('=',$1,$3);}	//������ֵ
	|ID '[' I ']' '=' I		{referArray($1,$3); $$=buildTree('=',$1,$6);}
	|ID '[' I ']'	{referArray($1,$3);}
	|V				{refer($1);}

	//����ֵΪ���ε����
	|I '+' I	    {$$=buildTree('+',$1,$3);}
	|I '-' I		{$$=buildTree('-',$1,$3);}
	|I '*' I		{$$=buildTree('*',$1,$3);}
	|I '/' I		{$$=buildTree('/',$1,$3);}
	|'(' I ')'		{$$=$2;}
	|NUM			{$$=$1;}
	|'-' I			%prec uminus		{$$=buildTree(NEG,$2);}
	
	//����ֵΪ���������
	|'!' I			{$$=buildTree('!',$2);}
	|I OR I			{$$=buildTree(OR,$1,$3);}
	|I AND I		{$$=buildTree(AND,$1,$3);}
	|I EQUAL I		{$$=buildTree(EQUAL,$1,$3);}
	|I '<' I		{$$=buildTree('<',$1,$3);}
	|I '>' I		{$$=buildTree('>',$1,$3);}
	|BOOL			{$$=$1;}
	;

//if else ���
F:	IF '(' I ')' E				{$$=buildTree(IFU,$3,$5);}		//����ȫƥ��
	|IF '(' I ')' E ELSE E		{$$=buildTree(IFF,$3,$5,$7);}	//��ȫƥ��
	;

//whileѭ��
W:	WHILE'(' I ')' E			{$$=buildTree(WHILE,$3,$5);}
	;

//print���
P:	PRINT ':' STRING ARGS ';'	{$$=buildTree(PRINT,$3,$4);}
	;

//PRINT�Ĳ����б�
ARGS:				{$$=NULL;}
	|ARGS ',' I		{$$=buildTree(MULTI,$1,$3);}
	;
	
%%

int main(void)
{
	init();			//������Դ�����޹ص�һЩ��ʼ��
	yyparse();		//����, ���ɸ��ڵ�Ϊroot���﷨��(�ڱ��ļ������涨���ȫ�ֱ���)
	generateVars();	//���ɱ����ռ�
	do_solve(root);	//�����﷨������ִ��Դ����
	return 0;
}

