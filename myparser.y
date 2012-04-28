%{
#include "mylexer.h"
#include "struct.h"
#include "id.h"
#define YYSTYPE LTree
%}

%token	ID				//变量名称
%token	NUM BOOL STRING		//这是常量类型
%token	TBOOL INT 			//这都是变量类型
%token	EQUAL AND OR		//多字符的操作符
%token	IF ELSE WHILE		//由于识别分支，循环语句
%token	NEG IFU IFF MULTI	//用于识别负号、不完整if、完整if和多条语句
%token	PRINT				//提供print方法

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
 |D		//声明语句
 |E		//执行语句
 ;

D:
 |INT ID	{declare(INT,$2->val.strval);}
 |TBOOL ID	{declare(TBOOL,$2->val.strval);}
 ;

E:
 |Q		//单行语句
 |M		//多行语句
 ;
 
Q:
 |I		//整形类运算
 |B		//BOOL类运算
 |ID '=' T	{buildTree('=',$1,$3);}			//T可以是整形，也可以是BOOL，赋值操作单独弄出来比较好
 |F		//IF语句
 |W		//WHILE语句
 ;

M:
 |'{' L '}'	//L里面是真正的多行语句
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

