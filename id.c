#include <stdlib.h>
#include <string.h>
#include "id.h"
#include "myparser.h"
#include "execute.h"

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

void idinit()
{
	tail=&ids;
}

//声明一个变量,将变量名,类型记录在列表中,确定变量在变量堆中的位置
void declare(LTree type, LTree name)
{
	LIdList t;

	t=(LIdList)malloc(sizeof(IdList));
	t->type=type->type;
	t->next=NULL;
	t->pos=size;
	t->name=name->val.strval;	//strval是动态申请的,现在交给LIdList了,
	*tail=t;						//free(name)的时候就不用管strval了
	tail=&t->next;

	switch(type->type)
	{
	case TBOOL:
	case INT:
	case STRING:
		size+=sizeof(int); break;
	}
	free(type);
	free(name);
}

//引用一个变量,按照变量名找到声明过的变量,找到它的类型和位置
void refer(LTree idTree)
{
	LIdList t;

	for(t=ids;t;t=t->next)
		if(idTree->type!=ARRAY && !strcmp(t->name,idTree->val.strval))
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

//销毁变量空间
void destroyVars()
{
	if(idheap) free(idheap);
	idheap=NULL;
}

//对变量赋值
void putvalue(LTree idTree, Type value)
{
	int offset;
	switch(idTree->type)
	{
	case INT:
		*(int *)(idheap+idTree->val.intval)=value.intval; break;
	case TBOOL:
		*(bool *)(idheap+idTree->val.intval)=value.boolval; break;
	case ARRAY:
		offset=do_solve(idTree->chi).intval;
		switch(idTree->type2)
		{
		case INT:
			*(*((int **)(idheap+idTree->val.intval))+offset)=value.intval; break;
		case TBOOL:
			*(*((bool **)(idheap+idTree->val.intval))+offset)=value.boolval; break;
		}
		break;
	}
}

//读变量的值
Type getvalue(LTree idTree)
{
	Type ans;
	int offset;

	switch(idTree->type)
	{
	case INT:
		ans.intval=*(int *)(idheap+idTree->val.intval); break;
	case TBOOL:
		ans.boolval=*(bool *)(idheap+idTree->val.intval); break;
	case ARRAY:
		offset=do_solve(idTree->chi).intval;
		switch(idTree->type2)
		{
		case INT:
			ans.intval=*(*((int **)(idheap+idTree->val.intval))+offset); break;
		case TBOOL:
			ans.boolval=*(*((bool **)(idheap+idTree->val.intval))+offset); break;
		}
		break;
	}
	return ans;
}

void declareArray(LTree type, LTree name)
{
	LIdList t;

	t=(LIdList)malloc(sizeof(IdList));
	t->type=ARRAY;
	t->type2=type->type;
	t->next=NULL;
	t->pos=size;
	t->name=name->val.strval;	//strval是动态申请的,现在交给LIdList了,
	*tail=t;					//free(name)的时候就不用管strval了
	tail=&t->next;

	size+=sizeof(void *);

	free(type);
	free(name);
}

void referArray(LTree array, LTree offset)
{
	LIdList t;

	for(t=ids;t;t=t->next)
		if(t->type==ARRAY && !strcmp(t->name,array->val.strval))
			break;

	if(!t)
	{
		fprintf(stderr,"没有声明数组:%s\n",array->val.strval);
		return;
	}

	free(array->val.strval);	//释放存储的变量名
	array->val.intval=t->pos;
	array->type=ARRAY;
	array->returnType=array->type2=t->type2;
	array->chi=offset;
}

LTree newArray(LTree array, LTree size)
{
	LTree ans;
	LIdList t;

	for(t=ids;t;t=t->next)
		if(t->type==ARRAY && !strcmp(t->name,array->val.strval))
			break;

	if(!t)
	{
		fprintf(stderr,"没有声明数组:%s\n",array->val.strval);
		return NULL;
	}

	free(array->val.strval);	//释放存储的变量名
	array->val.intval=t->pos;
	array->type=ARRAY;
	array->type2=t->type2;

	ans=(LTree)malloc(sizeof(Tree));
	ans->type=NEW;
	ans->bro=ans->chi=NULL;
	array->bro=size;
	ans->chi=array;
	ans->returnType=ARRAY;
	return ans;
}

void arrayNewSize(LTree node)
{
	LTree array;
	void **p;
	int size;

	array=node->chi;
	size=do_solve(array->bro).intval;
	switch(array->type2)
	{
	case INT: case TBOOL:
		size*=sizeof(int); break;
	}

	p=(void**)(idheap+array->val.intval);
	if(*p) free(*p);
	*p=malloc(size);
}

void** getPoint(LTree array)
{
	return (void**)(idheap+array->val.intval);
}