#ifndef _MYSTRUCT_
#define _MYSTRUCT_

//############���ļ�����������ͺ���غ���###############

//��������
typedef enum
{
	false,true
}bool;

//֧�ֵ���������
typedef union
{
	int intval;
	bool boolval;
	char *strval;
}Type;

//���Ľڵ�, yyval�����ͽ��ض���ΪLTree
typedef struct treenode
{
	int type;	//��־��������(�Ǻ�����)
	int type2;		//����Ԫ�ص�����
	int returnType;		//�˽ڵ�ķ���ֵ����(ȡ����������)
	Type val;			//�ڵ��ֵ
	struct treenode *bro;	//(��дbrother)�ֵ�, ����ͬһ�����ڵ�ĺ���
	struct treenode *chi;	//(��дchildren)����, �ǵ�һ������, �����ܻ����ֵ�
}Tree,*LTree;

//����ֻ������Ҷ��:������ID
//������������Ҷ��
LTree newBOOL(bool val);

//����NUM����Ҷ��
LTree newNUM(int num);

//����STRING����Ҷ��
LTree newSTRING(const char *name);

//����IDҶ��
LTree newID(char *name);

//ֻ������������ʱ��ͳһ����,��ʱ��������
LTree newTYPE(int type);

//����һ�Ų����������,ֻ��Ϊ����������ƽ�е��﷨��
LTree newNOP();

//����һ������Ϊtype(�Ǻ�����)����, type����������Ҳ��LTree�ĺ���
LTree buildTree(int type, ...);
#endif
