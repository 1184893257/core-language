%{
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
"/*"		BEGIN(comment);

\n				linenum++;	//行号递增,可用于语法检查

{space}			;//吃掉空格和\t

"=="			return EQUAL;

"!="			return NOTEQUAL;

"&&"			return AND;

"||"			return OR;

"+"|"-"|"*"|"/"		return yytext[0];

"<"|">"|"!"			return yytext[0];

"="|"("|")"|"["|"]"	return yytext[0];

";"|","|":"			return yytext[0];

"false"			{yylval=newBOOL(false); return BOOL;}

"true"			{yylval=newBOOL(true); return BOOL;}

"int"			return INT;

"bool"			return TBOOL;

"print"			return PRINT;
	
("_"|{char})({char}|"_"|{digit})*	{
		yylval=newID(yytext);
		return ID;
	}
	
{digit}+		{
		yylval=newNUM(atoi(yytext));
		return NUM;
	}
	
\"([^\\\"]|\\.)*\"	{
		yytext[strlen(yytext)-1]='\0';
		yylval=newSTRING(yytext+1);
		//printf("STRING:%s\n",yytext+1);
		return STRING;
	}
		
<comment>"*/"	BEGIN(0);
<comment>.		;
<comment>\n		linenum++;
%%
