%{
#include <stdio.h>
#include "mylexer.h"
#include "struct.h"
#include "id.h"
#include "init.h"
#include "execute.h"
#include "finalize.h"
#define YYSTYPE LTree
LTree root;			//�﷨���ĸ��ڵ�

extern int linenum;		//lex�ļ��ж���������к�
extern int tokenpos;	//lex�ж���,����λ��
extern int tokenleng;	//lex�ж���,���������Ǵ�����Ǹ��Ǻŵĳ���
extern char linebuf[];	//�����Դ����

int errors;				//�����Ĵ���ĸ���

int loopdeep;			//whileѭ�������,�����ж�break��continue�Ƿ�Ϸ�

LTree errHandle();		//���涨����,�ڵ����������ʱ�����
%}

%token	ID				//��������
%token	NUM BOOL STRING	DOUBLE	//���ǳ�������
%token	TDOUBLE TBOOL INT ARRAY	NORETURN	//������������, ����, ������, �޷���ֵ
%token	CHANGE				//����ת������
%token	EQUAL AND OR NEW	//���ַ��Ĳ�����
%token	IF ELSE WHILE		//����ʶ���֧��ѭ�����
%token	NEG IFU IFF 		//����ʶ�𸺺š�������if������if
%token	MULTI MULTI2		//�������½ڵ�,ֻ�ǰѺ�������������(MULTI),
							//�ṩһ���½ڵ㺢�����Ǹ������(MULTI2)
%token	PRINT				//�ṩprint����
%token	MAIN				//root�Ľڵ�����
%token	NOP					//�����,��ȫ��if else��,���if������ǿ����,
							//�Ͷ���,������else��������,�������ø������
							
%token	BREAK CONTINUE		//�����������Ǻ�,���Ծ�������

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
	|error TYPE ID ';'			{printf("����������������ǰ��\n");errHandle();}
	|error TYPE '[' ']' ID ';'	{printf("����������������ǰ��\n");errHandle();}
	;
	
D:
	|D TYPE ID ';'			{declare($2,$3);}
	|D TYPE '[' ']' ID ';'	{declareArray($2,$5);}
	;
 
TYPE:INT	{$$=newTYPE(INT);}
	|TBOOL	{$$=newTYPE(TBOOL);}
	|TDOUBLE	{$$=newTYPE(TDOUBLE);}
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
	|BREAK ';'		{$$=newBREAK();}//break���
	|CONTINUE ';'	{$$=newCONTINUE();}//continue���
	|error ';'	{$$=errHandle();}		//������д�����
	|error '}'	{$$=errHandle();}
	;
	
A:	ID '=' NEW '[' I ']' ';' 	{$$=newArray($1,$5);}
	;

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

	//����ֵΪ��ֵ�͵����
	|I '+' I	    {$$=buildTree('+',$1,$3);}
	|I '-' I		{$$=buildTree('-',$1,$3);}
	|I '*' I		{$$=buildTree('*',$1,$3);}
	|I '/' I		{$$=buildTree('/',$1,$3);}
	|'(' I ')'		{$$=$2;}
	|'(' TYPE ')' I	%prec uminus		{$$=buildTree(CHANGE,$2,$4);}
	|NUM			{$$=$1;}
	|DOUBLE			{$$=$1;}
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
W:	WHILE {loopdeep++;} '(' I ')' E			{$$=buildTree(WHILE,$4,$6); loopdeep--;}
	;

//print���
P:	PRINT ':' STRING ARGS ';'	{$$=buildTree(PRINT,$3,$4);}
	;

//PRINT�Ĳ����б�
ARGS:				{$$=NULL;}
	|ARGS ',' I		{$$=buildTree(MULTI,$1,$3);}
	;
	
%%

LTree errHandle()
{
	errors++;
	yyclearin();
	yyerrok();
	printf("line %d:<�﷨����>\n",linenum+1);
	printf("%s\n%*s\n",linebuf,tokenpos-tokenleng+1,"^");
	return newNOP();
}

int main(void)
{
	printf("/******************************************************************************\n");
	printf("*                   �� �� ԭ �� �� �� ҵ ����C���Ա�������                   \n");
	printf("*                                     ���ߣ�������(03091445)  ������(03091388) \n");
	printf("*******************************************************************************\n");
	printf("\n");
	printf("/******************************************************************************\n");
	printf("���������������㣺\n");
	printf("1.�������ʦҪ��Ļ����������������Ρ�bool�������������Ͻ��д������㡢��������\n");
	printf("2.��������ȫ����C���Ը�ʽ������ĸ���»��߿�ͷ��\n");
	printf("3.ʵ����if��while��䣬��ʵ����break,continue�ؼ��֡�\n");
	printf("4.ʵ���˶�̬���飬���������±���Թ�ʽ����\n");
	printf("5.ʵ����C���Ե�printf��䣬���Դ�ӡ�κ���ʽ����䡣\n");
	printf("6.������Ҫ����ʵ�����﷨��������ܣ���������ȫ�������λ�á�\n");
	printf("7.ʵ����ע�͹��ܡ�\n");
	printf("8.����չ�Ժã���������ʹ��typeʵ�ֵģ�����������ӴӶ�ʵ��float double��������\n");
	printf("*******************************************************************************\n");

	init();			//������Դ�����޹ص�һЩ��ʼ��
	yyparse();		//����, ���ɸ��ڵ�Ϊroot���﷨��(�ڱ��ļ������涨���ȫ�ֱ���)
	generateVars();	//���ɱ����ռ�
	if(errors==0)
		do_solve(root);
	else
		printf("��������%d��,����ִ���﷨��\n",errors);
	finalize(root);
	return 0;
}

