%{
#include "mylexer.h"
#include "struct.h"
#include "id.h"
#define YYSTYPE LTree
%}

%token	ID				//��������
%token	NUM BOOL STRING		//���ǳ�������
%token	TBOOL INT 			//�ⶼ�Ǳ�������
%token	EQUAL AND OR		//���ַ��Ĳ�����
%token	IF ELSE WHILE		//����ʶ���֧��ѭ�����
%token	NEG IFU IFF MULTI	//����ʶ�𸺺š�������if������if�Ͷ������
%token	PRINT				//�ṩprint����

%right	'!'
%left	'*'
%left	'+''-'
%left	'<''>'
%left	EQUAL
%left	AND 
%left	OR
%left	'=' 

%%

S:
 |D		//�������
 |E		//ִ�����
 ;

D:
 |INT ID	{declare(INT,$2->val.strval);}
 |TBOOL ID	{declare(TBOOL,$2->val.strval);}
 ;

E:
 |Q		//�������
 |M		//�������
 ;
 
Q:
 |I		//����������
 |B		//BOOL������
 |ID '=' T	{buildTree('=',$1,$3);}			//T���������Σ�Ҳ������BOOL����ֵ��������Ū�����ȽϺ�
 |F		//IF���
 |W		//WHILE���
 ;

M:
 |'{' L '}'	//L�����������Ķ������
 ;
 
L:
 |L ';' Q		{buildTree(MULTI,$1,$3);}
 |Q				{$$=$1;}
 ;
 
T:
 |I
 |B
 ;
 
I:
 |I '+' I	    {buildTree('+',$1,$3);}
 |I '-' I		{buildTree('-',$1,$3);}
 |I '*' I		{buildTree('*',$1,$3);}
 |'-' I			{buildTree(NEG,$1);}
 |'(' I ')'		{$$=$2;}
 |NUM			{$$=$1;}
 |ID			{$$=$1;}
 ;
 
B:
 |'!' B			{buildTree('!',$2);}
 |B OR B		{buildTree(OR,$1,$3);}
 |B EQUAL B		{buildTree(EQUAL,$1,$3);}
 |B AND B		{buildTree(AND,$1,$3);}
 |I '<' I		{buildTree('<',$1,$3);}
 |I '>' I		{buildTree('>',$1,$3);}
 |BOOL			{$$=$1;}
 |ID			{$$=$1;}
 ;
 
F:
 |IF '(' B ')' E			{buildTree(IFU,$3,$5);}
 |IF '(' B ')' E ELSE E		{buildTree(IFF,$3,$5,$7);}
 ;
 
W:
 |WHILE'(' B ')' E			{buildTree(WHILE,$3,$5);}
 ;

%%

int main(void)
{
	//yylex();
	yyparse();
	return 0;
}

