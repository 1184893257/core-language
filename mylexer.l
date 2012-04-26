%{
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

\n				linenum++;	//�кŵ���,�������﷨���

{space}			;//�Ե��ո��\t

"=="			return EQUAL;

"&&"			return AND;

"||"			return OR;

"+"|"-"|"*"|"/"		return yytext[0];

"<"|">"|"!"			return yytext[0];

";"|"="|"("|")"|"["|"]"		return yytext[0];

"false"			{yylval=newBOOL(false); return BOOL;}

"true"			{yylval=newBOOL(true); return BOOL;}

"int"			return INT;

"bool"			return TBOOL;
	
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