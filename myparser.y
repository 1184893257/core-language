%{
#include <stdio.h>
#include "mylexer.h"
#include "struct.h"
#include "id.h"
#include "init.h"
#include "execute.h"
#define YYSTYPE LTree
LTree root;			//语法树的根节点
extern int linenum;
%}

%token	ID				//变量名称
%token	NUM BOOL STRING		//这是常量类型
%token	TBOOL INT ARRAY		//这都是变量类型,引入数组了
%token	EQUAL AND OR NEW	//多字符的操作符
%token	IF ELSE WHILE		//由于识别分支，循环语句
%token	NEG IFU IFF 		//用于识别负号、不完整if、完整if
%token	MULTI MULTI2		//不产生新节点,只是把孩子们连接起来(MULTI),
							//提供一个新节点孩子们是各种语句(MULTI2)
%token	PRINT				//提供print方法
%token	MAIN				//root的节点类型
%token	NOP					//空语句,完全的if else中,如果if后跟的是空语句,
							//就断了,连不上else的语句块了,所以设置个空语句

%right	'='
%left	OR
%left	AND 
%left	EQUAL
%left	'<' '>'
%left	'+' '-'
%left	'*' '/'
%right	'!' uminus

%%
//先声明语句,再是多条语句
S:	D M	{root=buildTree(MAIN,buildTree(MULTI2,$2));}		//整棵语法树就诞生了O(∩_∩)O哈哈~
	;
	
D:
	|D TYPE ID ';'			{declare($2,$3);}
	|D TYPE '[' ']' ID ';'	{declareArray($2,$5);}
	;
 
TYPE:INT	{$$=newTYPE(INT);}
	|TBOOL	{$$=newTYPE(TBOOL);}
	;

/*3种语句块:空语句(只有一个分号), 单行语句, 多行语句, 
这3种都是可以作为if和while的执行块的
*/
E:	';'			{$$=newNOP();}		//空语句(为了连接树而建,执行时忽略)
	|Q								//单行语句
	|'{' M '}'	{$$=buildTree(MULTI2,$2);}			//多行语句
	;

Q:	I ';'	//整形或布尔运算
	|F		//IF语句
	|W		//WHILE语句
	|P		//print语句
	|A 		//new数组
//	//|error ER ';'	{fprintf(stderr,"line:%d:此行以上出现语法错误!",linenum);}		//这里进行错误处理
	//|error	{printf("I catched a error");}		//这里进行错误处理
	;
	
A:	ID '=' NEW '[' I ']' ';' 	{$$=newArray($1,$5);}
	;
	/*
ER:	
	|ER T
	;

//所有除分号外在单条语句中能出现的终结符,用于error吃掉
T:	EQUAL|AND|OR|'!'	//布尔运算符
	|'+'|'-'|'*'|'/'	//四则运算
	|'<'|'>'
	|'('|')'|'['|']'
	|'='|':'|','
	|TYPE					{printf("line:%d:声明语句必须放在最前面\n",linenum);}
	|BOOL|NUM|ID|STRING
	|PRINT|IF|ELSE
	;*/

//多行语句
M:				{$$=NULL;}
	|M Q		{$$=buildTree(MULTI,$1,$2);}
	;

V:	ID
	;

//单行运算语句
I:	ID '=' I		{refer($1); $$=buildTree('=',$1,$3);}	//变量左值
	|ID '[' I ']' '=' I		{referArray($1,$3); $$=buildTree('=',$1,$6);}
	|ID '[' I ']'	{referArray($1,$3);}
	|V				{refer($1);}

	//返回值为整形的语句
	|I '+' I	    {$$=buildTree('+',$1,$3);}
	|I '-' I		{$$=buildTree('-',$1,$3);}
	|I '*' I		{$$=buildTree('*',$1,$3);}
	|I '/' I		{$$=buildTree('/',$1,$3);}
	|'(' I ')'		{$$=$2;}
	|NUM			{$$=$1;}
	|'-' I			%prec uminus		{$$=buildTree(NEG,$2);}
	
	//返回值为布尔的语句
	|'!' I			{$$=buildTree('!',$2);}
	|I OR I			{$$=buildTree(OR,$1,$3);}
	|I AND I		{$$=buildTree(AND,$1,$3);}
	|I EQUAL I		{$$=buildTree(EQUAL,$1,$3);}
	|I '<' I		{$$=buildTree('<',$1,$3);}
	|I '>' I		{$$=buildTree('>',$1,$3);}
	|BOOL			{$$=$1;}
	;

//if else 语句
F:	IF '(' I ')' E				{$$=buildTree(IFU,$3,$5);}		//不完全匹配
	|IF '(' I ')' E ELSE E		{$$=buildTree(IFF,$3,$5,$7);}	//完全匹配
	;

//while循环
W:	WHILE'(' I ')' E			{$$=buildTree(WHILE,$3,$5);}
	;

//print语句
P:	PRINT ':' STRING ARGS ';'	{$$=buildTree(PRINT,$3,$4);}
	;

//PRINT的参数列表
ARGS:				{$$=NULL;}
	|ARGS ',' I		{$$=buildTree(MULTI,$1,$3);}
	;
	
%%

int main(void)
{
	init();			//与输入源程序无关的一些初始化
	yyparse();		//建树, 生成根节点为root的语法树(在本文件最上面定义的全局变量)
	generateVars();	//生成变量空间
	do_solve(root);	//根据语法树解释执行源程序
	return 0;
}

