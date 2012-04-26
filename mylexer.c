/****************************************************************************
*                         A C A D E M I C   C O P Y
* 
* This file was produced by an ACADEMIC COPY of Parser Generator. It is for
* use in non-commercial software only. An ACADEMIC COPY of Parser Generator
* can only be used by a student, or a member of an academic community. If
* however you want to use Parser Generator for commercial purposes then you
* will need to purchase a license. For more information see the online help or
* go to the Bumble-Bee Software homepage at:
* 
* http://www.bumblebeesoftware.com
* 
* This notice must remain present in the file. It cannot be removed.
****************************************************************************/

/****************************************************************************
* mylexer.c
* C source file generated from mylexer.l.
* 
* Date: 04/26/12
* Time: 14:54:27
* 
* ALex Version: 2.07
****************************************************************************/

#include <yylex.h>

/* namespaces */
#if defined(__cplusplus) && defined(YYSTDCPPLIB)
using namespace std;
#endif
#if defined(__cplusplus) && defined(YYNAMESPACE)
using namespace yl;
#endif

#define YYFASTLEXER

#line 1 ".\\mylexer.l"

#include "myparser.h"
#include "struct.h"
int linenum;
extern LTree yylval;

#line 46 "mylexer.c"
/* repeated because of possible precompiled header */
#include <yylex.h>

/* namespaces */
#if defined(__cplusplus) && defined(YYSTDCPPLIB)
using namespace std;
#endif
#if defined(__cplusplus) && defined(YYNAMESPACE)
using namespace yl;
#endif

#define YYFASTLEXER

#include ".\mylexer.h"

#ifndef YYTEXT_SIZE
#define YYTEXT_SIZE 100
#endif
#ifndef YYUNPUT_SIZE
#define YYUNPUT_SIZE YYTEXT_SIZE
#endif
#ifndef YYTEXT_MAX
#define YYTEXT_MAX 0
#endif
#ifndef YYUNPUT_MAX
#define YYUNPUT_MAX YYTEXT_MAX
#endif

/* yytext */
static char YYNEAR yysatext[(YYTEXT_SIZE) + 1];		/* extra char for \0 */
char YYFAR *YYNEAR YYDCDECL yystext = yysatext;
char YYFAR *YYNEAR YYDCDECL yytext = yysatext;
int YYNEAR YYDCDECL yystext_size = (YYTEXT_SIZE);
int YYNEAR YYDCDECL yytext_size = (YYTEXT_SIZE);
int YYNEAR YYDCDECL yytext_max = (YYTEXT_MAX);

/* yystatebuf */
#if (YYTEXT_SIZE) != 0
static int YYNEAR yysastatebuf[(YYTEXT_SIZE)];
int YYFAR *YYNEAR YYDCDECL yysstatebuf = yysastatebuf;
int YYFAR *YYNEAR YYDCDECL yystatebuf = yysastatebuf;
#else
int YYFAR *YYNEAR YYDCDECL yysstatebuf = NULL;
int YYFAR *YYNEAR YYDCDECL yystatebuf = NULL;
#endif

/* yyunputbuf */
#if (YYUNPUT_SIZE) != 0
static int YYNEAR yysaunputbuf[(YYUNPUT_SIZE)];
int YYFAR *YYNEAR YYDCDECL yysunputbufptr = yysaunputbuf;
int YYFAR *YYNEAR YYDCDECL yyunputbufptr = yysaunputbuf;
#else
int YYFAR *YYNEAR YYDCDECL yysunputbufptr = NULL;
int YYFAR *YYNEAR YYDCDECL yyunputbufptr = NULL;
#endif
int YYNEAR YYDCDECL yysunput_size = (YYUNPUT_SIZE);
int YYNEAR YYDCDECL yyunput_size = (YYUNPUT_SIZE);
int YYNEAR YYDCDECL yyunput_max = (YYUNPUT_MAX);

/* backwards compatability with lex */
#ifdef input
#ifdef YYPROTOTYPE
int YYCDECL yyinput(void)
#else
int YYCDECL yyinput()
#endif
{
	return input();
}
#else
#define input yyinput
#endif

#ifdef output
#ifdef YYPROTOTYPE
void YYCDECL yyoutput(int ch)
#else
void YYCDECL yyoutput(ch)
int ch;
#endif
{
	output(ch);
}
#else
#define output yyoutput
#endif

#ifdef unput
#ifdef YYPROTOTYPE
void YYCDECL yyunput(int ch)
#else
void YYCDECL yyunput(ch)
int ch;
#endif
{
	unput(ch);
}
#else
#define unput yyunput
#endif

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn -rch		/* <warning: unreachable code> off */
#endif
#endif

#ifdef YYPROTOTYPE
int YYCDECL yylexeraction(int action)
#else
int YYCDECL yylexeraction(action)
int action;
#endif
{
	yyreturnflg = YYTRUE;
	switch (action) {
	case 1:
		{
#line 13 ".\\mylexer.l"
BEGIN(comment);
#line 167 "mylexer.c"
		}
		break;
	case 2:
		{
#line 15 ".\\mylexer.l"
linenum++;
#line 174 "mylexer.c"
		}
		break;
	case 3:
		{
#line 17 ".\\mylexer.l"
;
#line 181 "mylexer.c"
		}
		break;
	case 4:
		{
#line 19 ".\\mylexer.l"
return EQUAL;
#line 188 "mylexer.c"
		}
		break;
	case 5:
		{
#line 21 ".\\mylexer.l"
return AND;
#line 195 "mylexer.c"
		}
		break;
	case 6:
		{
#line 23 ".\\mylexer.l"
return OR;
#line 202 "mylexer.c"
		}
		break;
	case 7:
		{
#line 25 ".\\mylexer.l"
return yytext[0];
#line 209 "mylexer.c"
		}
		break;
	case 8:
		{
#line 27 ".\\mylexer.l"
return yytext[0];
#line 216 "mylexer.c"
		}
		break;
	case 9:
		{
#line 29 ".\\mylexer.l"
return yytext[0];
#line 223 "mylexer.c"
		}
		break;
	case 10:
		{
#line 31 ".\\mylexer.l"
yylval=newBOOL(false); return BOOL;
#line 230 "mylexer.c"
		}
		break;
	case 11:
		{
#line 33 ".\\mylexer.l"
yylval=newBOOL(true); return BOOL;
#line 237 "mylexer.c"
		}
		break;
	case 12:
		{
#line 35 ".\\mylexer.l"
return INT;
#line 244 "mylexer.c"
		}
		break;
	case 13:
		{
#line 37 ".\\mylexer.l"
return TBOOL;
#line 251 "mylexer.c"
		}
		break;
#line 38 ".\\mylexer.l"
	
#line 256 "mylexer.c"
	case 14:
		{
#line 39 ".\\mylexer.l"

		yylval=newID(yytext);
		return ID;
	
#line 264 "mylexer.c"
		}
		break;
#line 43 ".\\mylexer.l"
	
#line 269 "mylexer.c"
	case 15:
		{
#line 44 ".\\mylexer.l"

		yylval=newNUM(atoi(yytext));
		return NUM;
	
#line 277 "mylexer.c"
		}
		break;
#line 48 ".\\mylexer.l"
	
#line 282 "mylexer.c"
	case 16:
		{
#line 49 ".\\mylexer.l"

		yytext[strlen(yytext)-1]='\0';
		yylval=newSTRING(yytext+1);
		//printf("STRING:%s\n",yytext+1);
		return STRING;
	
#line 292 "mylexer.c"
		}
		break;
#line 55 ".\\mylexer.l"
		
#line 297 "mylexer.c"
	case 17:
		{
#line 56 ".\\mylexer.l"
BEGIN(0);
#line 302 "mylexer.c"
		}
		break;
	case 18:
		{
#line 57 ".\\mylexer.l"
;
#line 309 "mylexer.c"
		}
		break;
	case 19:
		{
#line 58 ".\\mylexer.l"
linenum++;
#line 316 "mylexer.c"
		}
		break;
	default:
		yyassert(0);
		break;
	}
	yyreturnflg = YYFALSE;
	return 0;
}

#ifndef YYNBORLANDWARN
#ifdef __BORLANDC__
#pragma warn .rch		/* <warning: unreachable code> to the old state */
#endif
#endif
YYCONST yymatch_t YYNEARFAR YYBASED_CODE YYDCDECL yymatch[] = {
	0
};

int YYNEAR YYDCDECL yytransitionmax = 192;
YYCONST yytransition_t YYNEARFAR YYBASED_CODE YYDCDECL yytransition[] = {
	{ 0, 0 },
	{ 5, 1 },
	{ 6, 1 },
	{ 22, 21 },
	{ 41, 3 },
	{ 13, 13 },
	{ 13, 13 },
	{ 13, 13 },
	{ 13, 13 },
	{ 13, 13 },
	{ 13, 13 },
	{ 13, 13 },
	{ 13, 13 },
	{ 13, 13 },
	{ 13, 13 },
	{ 27, 16 },
	{ 28, 17 },
	{ 29, 18 },
	{ 30, 19 },
	{ 31, 20 },
	{ 25, 12 },
	{ 0, 23 },
	{ 32, 27 },
	{ 33, 28 },
	{ 5, 1 },
	{ 7, 1 },
	{ 8, 1 },
	{ 34, 29 },
	{ 35, 30 },
	{ 36, 32 },
	{ 9, 1 },
	{ 37, 33 },
	{ 10, 1 },
	{ 10, 1 },
	{ 11, 1 },
	{ 11, 1 },
	{ 42, 3 },
	{ 11, 1 },
	{ 38, 35 },
	{ 12, 1 },
	{ 13, 1 },
	{ 13, 1 },
	{ 13, 1 },
	{ 13, 1 },
	{ 13, 1 },
	{ 13, 1 },
	{ 13, 1 },
	{ 13, 1 },
	{ 13, 1 },
	{ 13, 1 },
	{ 39, 37 },
	{ 10, 1 },
	{ 7, 1 },
	{ 14, 1 },
	{ 7, 1 },
	{ 24, 9 },
	{ 26, 14 },
	{ 43, 42 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 23, 21 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 10, 1 },
	{ 0, 0 },
	{ 10, 1 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 16, 1 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 17, 1 },
	{ 0, 0 },
	{ 0, 0 },
	{ 18, 1 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 19, 1 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 20, 1 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 0, 0 },
	{ 15, 39 },
	{ 0, 0 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 },
	{ 15, 39 }
};

YYCONST yystate_t YYNEARFAR YYBASED_CODE YYDCDECL yystate[] = {
	{ 0, 0, 0 },
	{ 39, -8, 0 },
	{ 1, 0, 0 },
	{ -40, -6, 0 },
	{ 3, 0, 0 },
	{ 0, 0, 3 },
	{ 0, 0, 2 },
	{ 0, 0, 8 },
	{ 21, 0, 3 },
	{ 0, 17, 0 },
	{ 0, 0, 9 },
	{ 0, 0, 7 },
	{ 0, -22, 7 },
	{ 0, -43, 15 },
	{ 0, -5, 9 },
	{ 39, 0, 14 },
	{ 39, -96, 14 },
	{ 39, -81, 14 },
	{ 39, -93, 14 },
	{ 39, -96, 14 },
	{ 0, -105, 0 },
	{ -21, -31, 0 },
	{ 0, 0, 16 },
	{ -21, 11, 0 },
	{ 0, 0, 5 },
	{ 0, 0, 1 },
	{ 0, 0, 4 },
	{ 39, -89, 14 },
	{ 39, -85, 14 },
	{ 39, -89, 14 },
	{ 39, -89, 14 },
	{ 0, 0, 6 },
	{ 39, -79, 14 },
	{ 39, -84, 14 },
	{ 39, 0, 12 },
	{ 39, -63, 14 },
	{ 39, 0, 13 },
	{ 39, -51, 14 },
	{ 39, 0, 11 },
	{ 0, 69, 10 },
	{ 0, 0, 18 },
	{ 0, 0, 19 },
	{ 0, 10, 18 },
	{ 0, 0, 17 }
};

YYCONST yybackup_t YYNEARFAR YYBASED_CODE YYDCDECL yybackup[] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

#line 59 ".\\mylexer.l"


