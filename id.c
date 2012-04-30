#include <stdlib.h>
#include <string.h>
#include "id.h"
#include "myparser.h"
#include "execute.h"

//�±�(�ֽ��±�)--������ ��ֵ�� ����
typedef struct idlist
{
	int type;
	int type2;		//����Ԫ�ص�����
	int pos;
	char *name;
	struct idlist *next;
}IdList,*LIdList;

static int size;	//ĿǰҪ�洢���б�������Ŀռ��С(�ֽ�)
static LIdList ids;		//�����б�
static LIdList *tail;	//�����б��β��,����������ʹ��
static char *idheap;	//�������յĴ洢�ռ�

void idinit()
{
	tail=&ids;
}

//����һ������,��������,���ͼ�¼���б���,ȷ�������ڱ������е�λ��
void declare(LTree type, LTree name)
{
	LIdList t;

	t=(LIdList)malloc(sizeof(IdList));
	t->type=type->type;
	t->next=NULL;
	t->pos=size;
	t->name=name->val.strval;	//strval�Ƕ�̬�����,���ڽ���LIdList��,
	*tail=t;						//free(name)��ʱ��Ͳ��ù�strval��
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

//����һ������,���ձ������ҵ��������ı���,�ҵ��������ͺ�λ��
void refer(LTree idTree)
{
	LIdList t;

	for(t=ids;t;t=t->next)
		if(!strcmp(t->name,idTree->val.strval))
			break;

	if(!t)
	{
		fprintf(stderr,"û����������:%s\n",idTree->val.strval);
		return;
	}

	free(idTree->val.strval);
	idTree->val.intval=t->pos;
	idTree->returnType=idTree->type=t->type;
}

//���ɱ������б��û����,��������������ids
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

//���ɱ����Ĵ洢�ռ�,���������ֽ�Ϊ0
void generateVars()
{
	idheap=(char*)malloc(size);
	memset(idheap,0,size);
	clearlist();
}

//�Ա�����ֵ
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

//��������ֵ
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
	t->name=name->val.strval;	//strval�Ƕ�̬�����,���ڽ���LIdList��,
	*tail=t;					//free(name)��ʱ��Ͳ��ù�strval��
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
		fprintf(stderr,"û����������:%s\n",array->val.strval);
		return;
	}

	free(array->val.strval);	//�ͷŴ洢�ı�����
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
		fprintf(stderr,"û����������:%s\n",array->val.strval);
		return NULL;
	}

	free(array->val.strval);	//�ͷŴ洢�ı�����
	array->val.intval=t->pos;
	array->type=ARRAY;
	array->type2=t->type2;

	ans=(LTree)malloc(sizeof(Tree));
	ans->type=NEW;
	ans->bro=ans->chi=NULL;
	array->bro=size;
	ans->chi=array;
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