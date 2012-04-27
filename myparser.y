%{
#include "mylexer.h"
#include "struct.h"
#define YYSTYPE LTree
%}

%token	ID				//变量名称
%token	NUM BOOL STRING		//这是常量类型
%token	TBOOL INT			//这都是变量类型
%token	EQUAL AND OR		//多字符的操作符

%%

L	:
	|	L '='	{printf("赋值\n");}
	|	L NUM	{printf("NUM:%d\n",$2->val.intval);}
	|	L ID	{printf("ID\n");}
	|	L BOOL	{printf("BOOL\n");}
	|	L '+'	{printf("add\n");}
	|	L ';'	{printf("end of a exp\n");}
	|	L STRING	{$$=$1;}
	|	L INT	{printf("val type:int\n");}
	|	L TBOOL	{printf("val type:bool\n");}
	;

//T是一条语句
T	:	D		//声明语句
	|	I		//返回值为整数的语句
	|	B		//返回值为布尔的语句
	|	AR		//返回值为数组的语句
	|	IFELSE	//if else 条件语句
	|	WHILE	//while 循环语句
	|	PRINT	//print语句
	;
	
D	:	TYPE ID		//声明变量
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

