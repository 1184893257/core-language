#ifndef _MYID_
#define _MYID_
#include "struct.h"
//############���ļ���ű����ĸ��ֲ���###############
//��ʼ��
void idinit();

//��������
void declare(int type, char *name);

//���ñ���
void refer(LTree idTree);

//��ִ��֮ǰ���ɱ����Ĵ洢�ռ�,�������ֽ���0
void generateVars();

//������ֵ�ı�����ֵ
void putvalue(LTree idTree, Type value);

//��������ֵ
Type getvalue(LTree idTree);

//��������,��δ�깤
void declareArray(int type, char *name);

//��������, id�Ǵ����������ֵĽڵ�, offset������������±����,��δ�깤
void referArray(LTree id, LTree offset);

//�����������µĿռ�,array������ID�ڵ�,size�Ƿ���ֵΪ������������,��δ�깤
void newArray(LTree array, LTree size);
#endif