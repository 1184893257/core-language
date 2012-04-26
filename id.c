#include <stdlib.h>
#include <string.h>
#include "id.h"
#include "myparser.h"

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

//����һ������,��������,���ͼ�¼���б���,ȷ�������ڱ������е�λ��
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
	switch(idTree->type)
	{
	case INT:
		*(int *)(idheap+idTree->val.intval)=value.intval; break;
	case TBOOL:
		*(bool *)(idheap+idTree->val.intval)=value.boolval; break;
	}
}

//��������ֵ
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