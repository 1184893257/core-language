#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "struct.h"
#include "myparser.h"
#include "sema.h"

LTree newBOOL(bool val)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->type=BOOL;
	ans->chi=ans->bro=NULL;
	ans->returnType=TBOOL;
	ans->val.boolval=val;
	return ans;
}

LTree newNUM(int num)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->type=NUM;
	ans->chi=ans->bro=NULL;
	ans->returnType=INT;
	ans->val.intval=num;
	return ans;
}

#define isnum(c) ('0'<=(c) && (c)<='9')
#define ishex(c) (isnum(c)||('a'<=(c) && (c)<='f')||('A'<=(c) && (c)<='F'))
#define tonum(c) ((c)-'0')
#define tohex(c) ((c)<='9'?tonum(c):((c)<='F'?(c)-'A'+10:(c)-'a'+10))

//s[*i]是'\\',s[*i+1]是x或数字,把后面的数字转化为字符返回,并修改*i
static char readCh(const char *s, int *i)
{
	int ri=*i,num=0,j;
	char ans;

	if(s[ri+1]=='x')
		for(j=2; j<4 && ishex(s[ri+j]); ++j)
			num=(num<<4)+tohex(s[ri+j]);
	else
		for(j=1; j<4 && isnum(s[ri+j]); ++j)
			num=(num<<3)+tonum(s[ri+j]);
	ans=num;
	*i=ri+j;
	return ans;
}

//字符串转义处理,字符串空间是动态申请到的
static char* strconver(const char *s)
{
	int len,i,j;
	char *ans;

	len=strlen(s);
	ans=(char *)malloc(len+1);	//返回的字符串不会比原字符串长
	for(i=0,j=0;i<len;)
	{
		if(s[i]!='\\')
			ans[j++]=s[i++];
		else
		{
			switch(s[i+1])
			{
			case 'a':	ans[j++]='\a';	i+=2; break;
			case 'b':	ans[j++]='\b';	i+=2; break;
			case 'f':	ans[j++]='\f';	i+=2; break;
			case 'n':	ans[j++]='\n';	i+=2; break;
			case 'r':	ans[j++]='\r';	i+=2; break;
			case 't':	ans[j++]='\t';	i+=2; break;
			case 'v':	ans[j++]='\v';	i+=2; break;
			case '"':	ans[j++]='"';	i+=2; break;
			case 'x':	ans[j++]=readCh(s,&i);break;
			default:
				if(isnum(s[i+1])) ans[j++]=readCh(s,&i);
				else
				{
					if(s[++i]!='\\') ans[j++]='\\';	//复制一个'\\'
					if(s[i]) ans[j++]=s[i++];
				}
			}
		}
	}
	ans[j]='\0';
	return ans;
}

LTree newSTRING(const char *s)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->type=STRING;
	ans->chi=ans->bro=NULL;
	ans->returnType=STRING;
	ans->val.strval=strconver(s);
	return ans;
}

LTree newID(char *name)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->chi=ans->bro=NULL;
	ans->type=ID;
	//ans->returnType=INT;		//这个要引用的时候设置
	ans->val.strval=strdup(name);
	return ans;
}

LTree newTYPE(int type)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->type=type;
	return ans;
}

LTree newNOP()
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->type=NOP;
	ans->bro=ans->chi=NULL;
	return ans;
}

LTree buildTree(int type, ...)
{
	va_list args;	//可变参数
	int n;			//参数个数,根据type来判定
	LTree ans;		//这次要建成的树
	int i;
	LTree *tail;

	if(type==MULTI)		//不要新建树,而是将type后的两个参数连接起来
	{
		LTree list;		//type后第一个参数,非NULL的时候是已经识别出来的链
		LTree node;		//type后第二个参数,新识别出来的树的根节点

		args=va_start(args,type);
		list=va_arg(args,LTree);
		node=va_arg(args,LTree);
		va_end(args);
		if(!list) ans=node;
		else
		{
			ans=list;
			while(list)
			{
				tail=&list->bro;
				list=list->bro;
			}
			*tail=node;
		}
		return ans;
	}

	//根据type确定参数的个数
	switch(type)
	{
	case NEG: case '!': case MAIN: case MULTI2:	//单元运算符和main以及多行语句
		n=1; break;
	case IFF:				//三元运算符
		n=3; break;
	default:				//大部分都是双元运算符
		n=2; break;
	}
	
	ans=(LTree)malloc(sizeof(Tree));
	ans->type=type;
	ans->bro=ans->chi=NULL;
	tail=&ans->chi;
	
	args=va_start(args,type);
	for(i=0;i<n;++i)
	{
		*tail=va_arg(args,LTree);
		tail=&(*tail)->bro;
	}
	va_end(args);

	verifySema(ans);	//检查语义,如果有类型不匹配的情况就发出警告

	return ans;
}