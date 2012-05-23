%{
#include <stdio.h>
#include "mylexer.h"
#include "struct.h"
#include "id.h"
#include "init.h"
#include "execute.h"
#include "finalize.h"
#define YYSTYPE LTree
LTree root;			//语法树的根节点

extern int linenum;		//lex文件中定义的整型行号
extern int tokenpos;	//lex中定义,行中位置
extern int tokenleng;	//lex中定义,错误发生后是错误的那个记号的长度
extern char linebuf[];	//缓存的源程序

int errors;				//检查出的错误的个数

int loopdeep;			//while循环的深度,用于判定break和continue是否合法

LTree errHandle();		//下面定义了,在单行语句错误的时候调用
%}

%token	ID				//变量名称
%token	NUM BOOL STRING	DOUBLE	//这是常量类型
%token	TDOUBLE TBOOL INT ARRAY	NORETURN	//布尔变量类型, 整型, 数组型, 无返回值
%token	CHANGE				//类型转换操作
%token	EQUAL AND OR NEW	//多字符的操作符
%token	IF ELSE WHILE		//由于识别分支，循环语句
%token	NEG IFU IFF 		//用于识别负号、不完整if、完整if
%token	MULTI MULTI2		//不产生新节点,只是把孩子们连接起来(MULTI),
							//提供一个新节点孩子们是各种语句(MULTI2)
%token	PRINT				//提供print方法
%token	MAIN				//root的节点类型
%token	NOP					//空语句,完全的if else中,如果if后跟的是空语句,
							//就断了,连不上else的语句块了,所以设置个空语句
							
%token	BREAK CONTINUE		//加入这两个记号,语言就完美了

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
	|error TYPE ID ';'			{printf("变量声明必须在最前面\n");errHandle();}
	|error TYPE '[' ']' ID ';'	{printf("变量声明必须在最前面\n");errHandle();}
	;
	
D:
	|D TYPE ID ';'			{declare($2,$3);}
	|D TYPE '[' ']' ID ';'	{declareArray($2,$5);}
	;
 
TYPE:INT	{$$=newTYPE(INT);}
	|TBOOL	{$$=newTYPE(TBOOL);}
	|TDOUBLE	{$$=newTYPE(TDOUBLE);}
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
	|BREAK ';'		{$$=newBREAK();}//break语句
	|CONTINUE ';'	{$$=newCONTINUE();}//continue语句
	|error ';'	{$$=errHandle();}		//这里进行错误处理
	|error '}'	{$$=errHandle();}
	;
	
A:	ID '=' NEW '[' I ']' ';' 	{$$=newArray($1,$5);}
	;

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

	//返回值为数值型的语句
	|I '+' I	    {$$=buildTree('+',$1,$3);}
	|I '-' I		{$$=buildTree('-',$1,$3);}
	|I '*' I		{$$=buildTree('*',$1,$3);}
	|I '/' I		{$$=buildTree('/',$1,$3);}
	|'(' I ')'		{$$=$2;}
	|'(' TYPE ')' I	%prec uminus		{$$=buildTree(CHANGE,$2,$4);}
	|NUM			{$$=$1;}
	|DOUBLE			{$$=$1;}
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
W:	WHILE {loopdeep++;} '(' I ')' E			{$$=buildTree(WHILE,$4,$6); loopdeep--;}
	;

//print语句
P:	PRINT ':' STRING ARGS ';'	{$$=buildTree(PRINT,$3,$4);}
	;

//PRINT的参数列表
ARGS:				{$$=NULL;}
	|ARGS ',' I		{$$=buildTree(MULTI,$1,$3);}
	;
	
%%

LTree errHandle()
{
	errors++;
	yyclearin();
	yyerrok();
	printf("line %d:<语法错误>\n",linenum+1);
	printf("%s\n%*s\n",linebuf,tokenpos-tokenleng+1,"^");
	return newNOP();
}

int main(void)
{
	printf("/******************************************************************************\n");
	printf("*                   编 译 原 理 大 作 业 （简单C语言编译器）                   \n");
	printf("*                                     作者：刘乔羽(03091445)  杨延中(03091388) \n");
	printf("*******************************************************************************\n");
	printf("\n");
	printf("/******************************************************************************\n");
	printf("本程序有以下亮点：\n");
	printf("1.完成了老师要求的基本操作：定义整形、bool变量，并在其上进行代数运算、布尔运算\n");
	printf("2.变量名完全按照C语言格式，以字母或下划线开头。\n");
	printf("3.实现了if、while语句，并实现了break,continue关键字。\n");
	printf("4.实现了动态数组，并且数组下标可以公式化。\n");
	printf("5.实现了C语言的printf语句，可以打印任何形式的语句。\n");
	printf("6.最最主要的是实现了语法语义纠错功能，并且能完全定义出错位置。\n");
	printf("7.实现了注释功能。\n");
	printf("8.可扩展性好，数据类型使用type实现的，可以随意添加从而实现float double数据类型\n");
	printf("*******************************************************************************\n");

	init();			//与输入源程序无关的一些初始化
	yyparse();		//建树, 生成根节点为root的语法树(在本文件最上面定义的全局变量)
	generateVars();	//生成变量空间
	if(errors==0)
		do_solve(root);
	else
		printf("检查出错误%d个,放弃执行语法树\n",errors);
	finalize(root);
	return 0;
}

