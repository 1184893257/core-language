#include <stdio.h>
#include <stdlib.h>
#include "sema.h"
#include "myparser.h"

extern int linenum;		//在lex源文件中定义的,当前记号所在行
extern char linebuf[];	//lex中定义
extern int errors;		//yacc中定义

static char info[1024];		//缓存错误信息

//输出语义警告
static void warn()
{
	printf("line %d:<语义错误>%s\n%s\n",linenum+1,info,linebuf);
	errors++;
}

//将编码的类型转化为字符串的形式
static char* findType(int type)
{
	char *s;
	switch(type)
	{
	case INT: s="INT"; break;
	case TBOOL: s="TBOOL"; break;
	case ARRAY: s="ARRAY"; break;
	case NORETURN: s="无返回值"; break;
	default: s="**"; printf("类型:%d尚未支持\n",type);
	}
	return s;
}

//将编码的操作类型转化为字符串的形式
static char* findOp(int type)
{
	char *s;
	switch(type)
	{
	case NEG: s="取负"; break;
	case '!': s="取非"; break;
	case EQUAL: s="判等"; break;
	case AND: s="与"; break;
	case OR: s="或"; break;
	case '<': s="<"; break;
	case '>': s=">"; break;

	case WHILE: s="while循环"; break;
	case IFU: s="不完全匹配的条件选择"; break;
	case IFF: s="完全匹配的条件选择"; break;
	}
	return s;
}

//判断树的返回值是不是数值型的
static bool isNUM(LTree node)
{
	int type=node->returnType;
	return type==INT || type==TDOUBLE;
}

//加个类型转换节点
static LTree CHANGETO(LTree node,int type)
{
	LTree ans=(LTree)malloc(sizeof(Tree));
	ans->bro=NULL;
	ans->type=CHANGE;
	ans->returnType=type;
	ans->chi=node;
	return ans;
}

//数值型的树兼容转换
static bool compatible(LTree *pc1,LTree *pc2)
{
	LTree c1=*pc1,c2=*pc2;
	if(!isNUM(c1) || !isNUM(c2)) return false;
	if(c1->returnType==c2->returnType) return true;
	if(c1->returnType==INT && c2->returnType==TDOUBLE)
		*pc1=CHANGETO(c1,TDOUBLE);
	else if(c1->returnType==TDOUBLE && c2->returnType==INT)
		*pc2=CHANGETO(c2,TDOUBLE);
	return true;
}

void verifySema(LTree node)
{
	LTree c1,c2;
	int t;

	switch(node->type)
	{
	case CHANGE:		//类型转换需调整一下树
		c1=node->chi;
		c2=c1->bro;
		node->returnType=c1->type;
		node->chi=c2;
		free(c1);
		break;

	case MAIN: case MULTI2:	case PRINT:	//这种树不检查,语法通过了语义就没问题
		break;

	case WHILE: case IFF: case IFU:		//这种树检查第一棵子树是不是布尔型的
		c1=node->chi;
		if(c1->returnType!=TBOOL)
		{
			sprintf(info,"%s类型不能作为%s语句的判定条件",findType(c1->returnType),findOp(node->type));
			errors++;
			printf("<语义错误>%s\n",info);
		}
		break;

	//赋值类型检查
	case '=':
		c1=node->chi;
		c2=c1->bro;
		if(c1->returnType!=c2->returnType)	//除了new其他运算中数组出现的格式都是id[I]
		{									//(id都被当做变量而非数组来检索,如果是数组就会报告找不到变量)
			if(isNUM(c1) && isNUM(c2))//还可以挽救
			{
				c2=CHANGETO(c2,c1->returnType);
				c1->bro=c2;
			}
			else
			{
				sprintf(info,"%s类型不能赋值为%s类型",findType(c1->returnType),findType(c2->returnType));
				warn();
			}
		}
		node->returnType=c1->returnType;
		break;

	//整数双元运算检查
	case '+': case '-': case '*': case '/': case '<': case '>':
		c1=node->chi;
		c2=c1->bro;
		if(!compatible(&c1,&c2))
		{
			sprintf(info,"%c运算不能应用于%s类型与%s类型",node->type,findType(c1->returnType),findType(c2->returnType));
			warn();
		}
		else
		{
			node->chi=c1;
			c1->bro=c2;
		}
		node->returnType=(node->type=='<' || node->type=='>') ? TBOOL : c1->returnType;
		break;

	//单元运算检查
	case NEG: case '!':
		t=node->type==NEG ? INT : TBOOL;
		c1=node->chi;
		if(c1->returnType!=t)
		{
			sprintf(info,"运算不能应用于%s类型",findOp(node->type),findType(c1->returnType));
			warn();
		}
		node->returnType=t;
		break;

	//布尔运算检查
	case OR: case AND:
		c1=node->chi;
		c2=c1->bro;
		if(c1->returnType!=TBOOL || c2->returnType!=TBOOL)
		{
			sprintf(info,"%s运算不能应用于%s类型与%s类型",findOp(node->type),findType(c1->returnType),findType(c2->returnType));
			warn();
		}
		node->returnType=TBOOL;		//为了精确定位错误,一般节点的返回值还是赋值为正确的
		break;
	
	//判等运算对于INT,INT和TBOOL,TBOOL组合都是正确的
	case EQUAL:
		c1=node->chi;
		c2=c1->bro;
		if(c1->returnType!=c2->returnType)
		{
			sprintf(info,"%s运算不能应用于%s类型与%s类型",findOp(node->type),findType(c1->returnType),findType(c2->returnType));
			warn();
		}
		node->returnType=TBOOL;		//为了精确定位错误,一般节点的返回值还是赋值为正确的
		break;
	}
}