%{
#include <stdlib.h>
#include <string.h>
#include "myparser.h"
#include "struct.h"
int linenum;
extern LTree yylval;
%}

digit		[0-9]
char		[a-zA-Z]
space		[" "\t]
%x	comment
%%
"/*"		BEGIN(comment);	//多行注释的互斥入口(没有单行注释O(∩_∩)O)

\n				linenum++;	//行号递增,可用于语法检查

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

"print"			return PRINT;

"if"			return IF;

"else"			return ELSE;

"while"			return WHILE;
	
("_"|{char})({char}|"_"|{digit})*	{	//标示符
		yylval=newID(yytext);
		return ID;
	}
	
{digit}+		{			//数字常量
		yylval=newNUM(atoi(yytext));
		return NUM;
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
