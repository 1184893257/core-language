#include <stdio.h>
#include "execute.h"
#include "myparser.h"
#include "id.h"
#include "print.h"

static bool breaksig;		//break信号
static bool continuesig;	//continue信号

#define getval(type)			\
static type get##type##val(LTree tree)	\
{								\
	Type t=do_solve(tree);		\
	type ans;					\
	switch(tree->returnType)	\
	{							\
	case INT: ans=(type)t.intval; break;\
	case TDOUBLE: ans=(type)t.doubleval; break;\
	}							\
	return ans;					\
}

getval(int)
getval(double)

Type do_solve(LTree root)
{
	Type ans,t1,t2;
	LTree c1,c2,c3;

	switch(root->type)
	{
	case CHANGE:
		c1=root->chi;
		switch(root->returnType)
		{
		case INT: ans.intval=getintval(c1); break;
		case TDOUBLE: ans.doubleval=getdoubleval(c1); break;
		}
		break;

	//NOP
	case NOP: break;	//直接被忽略了

	//置BREAK信号为true
	case BREAK: breaksig=true; break;

	//置CONTINUE信号为true
	case CONTINUE: continuesig=true; break;

	//NEW
	case NEW:
		arrayNewSize(root); break;

	//main
	case MAIN: do_solve(root->chi); break;

	//多行语句
	case MULTI2:
		for(c1=root->chi;c1;c1=c1->bro)
		{
			do_solve(c1);
			if(breaksig==true || continuesig==true)
				break;	//break就行,while来置这些信号为false
		}
		break;

	//while
	case WHILE:
		c1=root->chi;
		c2=c1->bro;
		while(do_solve(c1).boolval==true)
		{
			do_solve(c2);
			if(breaksig==true)
			{
				breaksig=false;
				break;
			}
			else if(continuesig==true)
			{
				continuesig=false;
				continue;
			}
		}
		break;

	//不完全匹配的if else
	case IFU:
		c1=root->chi;
		c2=c1->bro;
		if(do_solve(c1).boolval==true)
			do_solve(c2);
		break;

	//完全匹配的if else
	case IFF:
		c1=root->chi;
		c2=c1->bro;
		c3=c2->bro;
		if(do_solve(c1).boolval==true)
			do_solve(c2);
		else
			do_solve(c3);
		break;

	//print方法
	case PRINT:
		print(root); break;

	//常量
	case NUM: case BOOL: case DOUBLE:
		ans=root->val; break;

	//变量读值
	case INT: case TBOOL: case TDOUBLE: case ARRAY:
		ans=getvalue(root); break;

	//赋值
	case '=':
		c1=root->chi;
		c2=c1->bro;
		ans=do_solve(c2);
		putvalue(c1,ans);
		break;

	//单元运算符
	case '!': case NEG:
		c1=root->chi;
		t1=do_solve(c1);
		switch(root->type)
		{
		case '!': ans.boolval=!(t1.boolval); break;
		case NEG: ans.intval=-t1.intval; break;
		}
		break;

	//短路计算与和或
	case AND: case OR:
		c1=root->chi;
		c2=c1->bro;
		t1=do_solve(c1);
		if(root->type==AND)
		{
			if(t1.boolval==false) ans.boolval=false;
			else ans.boolval=do_solve(c2).boolval;
		}
		else
		{
			if(t1.boolval==true) ans.boolval=true;
			else ans.boolval=do_solve(c2).boolval;
		}
		break;

	//双元运算符
	default:
		c1=root->chi;
		c2=c1->bro;
		t1=do_solve(c1);
		t2=do_solve(c2);
		switch(root->type)
		{
		case '+':
			if(root->returnType==INT) ans.intval=t1.intval+t2.intval;
			else if(root->returnType==TDOUBLE) ans.doubleval=t1.doubleval+t2.doubleval;
			break;
		case '-':
			if(root->returnType==INT) ans.intval=t1.intval-t2.intval;
			else if(root->returnType==TDOUBLE) ans.doubleval=t1.doubleval-t2.doubleval;
			break;
		case '*':
			if(root->returnType==INT) ans.intval=t1.intval*t2.intval;
			else if(root->returnType==TDOUBLE) ans.doubleval=t1.doubleval*t2.doubleval;
			break;
		case '/':
			if(root->returnType==INT) ans.intval=t1.intval/t2.intval;
			else if(root->returnType==TDOUBLE) ans.doubleval=t1.doubleval/t2.doubleval;
			break;

		case EQUAL: 
			if(c1->returnType==INT) ans.boolval=t1.intval==t2.intval;
			else if(c1->returnType==TDOUBLE) ans.boolval=t1.doubleval==t2.doubleval;
			break;
		case '<':
			if(c1->returnType==INT) ans.boolval=t1.intval < t2.intval;
			else if(c1->returnType==TDOUBLE) ans.boolval=t1.doubleval < t2.doubleval;
			break;
		case '>':
			if(c1->returnType==INT) ans.boolval=t1.intval > t2.intval;
			else if(c1->returnType==TDOUBLE) ans.boolval=t1.doubleval > t2.doubleval;
			break;
		default:
			fprintf(stderr,"错误的操作符:%d\n",root->type); break;
		}
		break;
	}

	return ans;
}