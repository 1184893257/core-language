%{
#include <stdlib.h>
#include <string.h>
#include "myparser.h"
#include "struct.h"

int tokenpos;		//行中位置
int tokenleng;		//获取的记号的长度,出错后不再更新
int linenum;		//当前行号
char linebuf[1024];	//当前行的内容(预读机制)

extern LTree yylval;
%}

digit		[0-9]
char		[a-zA-Z]
space		[" "\t]
%x	comment
%%
	if(!YYRECOVERING){
		tokenpos+=yyleng;	//出错后不递增,保持出错时的位置
		tokenleng=yyleng;
	}

"/*"		BEGIN(comment);	//多行注释的互斥入口(没有单行注释O(∩_∩)O)

\n.*			{
		tokenpos=0;
		strcpy(linebuf,yytext+1);
		linenum++;
		yyless(1);
}

{space}			;//吃掉空格和\t

"=="			return EQUAL;

"&&"			return AND;

"||"			return OR;

"+"|"-"|"*"|"/"		return yytext[0];	//四则运算

"<"|">"|"!"			return yytext[0];	//逻辑运算

"("|")"|"["|"]"|"{"|"}"		return yytext[0];	//括号

"="|";"|","|":"		return yytext[0];

"false"			{yylval=newBOOL(false); return BOOL;}

"true"			{yylval=newBOOL(true); return BOOL;}

"new"			return NEW;

"int"			return INT;

"bool"			return TBOOL;

"double"		return TDOUBLE;

"print"			return PRINT;

"if"			return IF;

"else"			return ELSE;

"while"			return WHILE;

"break"			return BREAK;

"continue"		return CONTINUE;
	
("_"|{char})({char}|"_"|{digit})*	{	//标示符
		yylval=newID(yytext);
		return ID;
	}
	
{digit}+		{			//数字常量
		yylval=newNUM(atoi(yytext));
		return NUM;
	}

{digit}+(\.{digit}+)?([eE][+-]?{digit}+)?	{//浮点数
		double t;
		sscanf(yytext,"%lf",&t);
		yylval=newDOUBLE(t);
		return DOUBLE;
	}
	
\"([^\\\"]|\\.)*\"	{		//字符串常量,只在print中被用到
		yytext[strlen(yytext)-1]='\0';
		yylval=newSTRING(yytext+1);
		return STRING;
	}
		
<comment>"*/"	BEGIN(0);
<comment>.		;
<comment>\n		linenum++;
%%
