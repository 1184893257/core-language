#include <stdlib.h>
#include <string.h>
#include "id.h"
#include "myparser.h"

//下标(字节下标)--变量名 键值对 链表
typedef struct idlist
{
	int type;
	int type2;		//数组元素的类型
	int pos;
	char *name;
	struct idlist *next;
}IdList,*LIdList;

static int size;	//目前要存储所有变量所需的空间大小(字节)
static LIdList ids;		//声明列表
static LIdList *tail;	//声明列表的尾部,供插入声明使用
static char *idheap;	//变量最终的存储空间

//声明一个变量,将变量名,类型记录在列表中,确定变量在变量堆中的位置
void declare(int type, char *name)
{
	LIdList t;

	if(!tail) tail=&ids;

	t=(LIdList)malloc(sizeof(IdList));
	t->type=type;
	t->next=NULL;
	t->pos=size;
	t->name=strdup(name);
	*tail=t;
	tail=&t->next;

	switch(type)
	{
	case TBOOL:
	case INT:
	case STRING:
		size+=4; break;
	}
}

//引用一个变量,按照变量名找到声明过的变量,找到它的类型和位置
void refer(LTree idTree)
{
	LIdList t;

	for(t=ids;t;t=t->next)
		if(!strcmp(t->name,idTree->val.strval))
			break;

	if(!t)
	{
		fprintf(stderr,"没有声明变量:%s\n",idTree->val.strval);
		return;
	}

	free(idTree->val.strval);
	idTree->val.intval=t->pos;
	idTree->returnType=idTree->type=t->type;
}

//生成变量后列表就没用了,调用这个函数清除ids
static void clearlist()
{
	LIdList t,p;
	for(t=ids;t;)
	{
		p=t->next;
		free(t->name);
		free(t);
		t=p;
	}
}

//生成变量的存储空间,并置所有字节为0
void generateVars()
{
	idheap=(char*)malloc(size);
	memset(idheap,0,size);
	clearlist();
}

//对变量赋值
void putvalue(LTree idTree, Type value)
{
	switch(idTree->type)
	{
	case INT:
		*(int *)(idheap+idTree->val.intval)=value.intval; break;
	case TBOOL:
		*(bool *)(idheap+idTree->val.intval)=value.boolval; break;
	}
}

//读变量的值
Type getvalue(LTree idTree)
{
	Type ans;
	switch(idTree->type)
	{
	case INT:
		ans.intval=*(int *)(idheap+idTree->val.intval); break;
	case TBOOL:
		ans.boolval=*(bool *)(idheap+idTree->val.intval); break;
	}
	return ans;
}