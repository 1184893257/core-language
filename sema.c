#include <stdio.h>
#include "sema.h"

extern int linenum;		//��lexԴ�ļ��ж����,��ǰ�Ǻ�������

static void warn(char *msg)
{
	fprintf(stderr,"line %d:<�������>%s\n",linenum,msg);
}

void verifySema(LTree node)
{
	LTree c1,c2,c3;
	switch(node->type)
	{
	}
}