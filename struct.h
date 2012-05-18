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
	double doubleval;
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


//�����͵�Ҷ��
//������������Ҷ��
LTree newBOOL(bool val);

//����NUM����Ҷ��
LTree newNUM(int num);

//��������������Ҷ��
LTree newDOUBLE(double val);

//����STRING����Ҷ��
LTree newSTRING(const char *name);


//����������ص�Ҷ��
//����IDҶ��
LTree newID(char *name);

//ֻ������������ʱ��ͳһ����,��ʱ��������
LTree newTYPE(int type);


//������ص�Ҷ��
//����һ�Ų����������,ֻ��Ϊ����������ƽ�е��﷨��,Ҳ�����滻������﷨��
LTree newNOP();

//����һ�����˳�ѭ������
LTree newBREAK();

//����һ�Ŷ���ʣ��Ķ���������ִ��whileѭ������
LTree newCONTINUE();

//����һ������Ϊtype(�Ǻ�����)����, type����������Ҳ��LTree�ĺ���
LTree buildTree(int type, ...);
#endif
