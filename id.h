#ifndef _MYID_
#define _MYID_
#include "struct.h"
//############���ļ���ű����ĸ��ֲ���###############
//��ʼ��
void idinit();

//��������,type�ǰ������͵���,name�ǰ�������������
void declare(LTree type, LTree name);

//���ñ���
void refer(LTree idTree);

//��ִ��֮ǰ���ɱ����Ĵ洢�ռ�,�������ֽ���0
void generateVars();

//���ٱ����ռ�
void destroyVars();

//������ֵ�ı�����ֵ
void putvalue(LTree idTree, Type value);

//��������ֵ
Type getvalue(LTree idTree);

//��������
void declareArray(LTree type, LTree name);

//��������, array�Ǵ����������ֵĽڵ�, offset������������±����
void referArray(LTree array, LTree offset);

//�����������µĿռ�,array������ID�ڵ�,size�Ƿ���ֵΪ������������
LTree newArray(LTree array, LTree size);

//ִ��NEW�ڵ�,��������������ռ�
void arrayNewSize(LTree node);

//���ָ��̬����ָ���ָ��,�����ͷſռ�
void** getPoint(LTree array);
#endif