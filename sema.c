#include <stdio.h>
#include "sema.h"

extern int linenum;		//在lex源文件中定义的,当前记号所在行

static void warn(char *msg)
{
	fprintf(stderr,"line %d:<语义错误>%s\n",linenum,msg);
}

void verifySema(LTree node)
{
	LTree c1,c2,c3;
	switch(node->type)
	{
	}
}