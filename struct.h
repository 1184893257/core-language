#ifndef _MYSTRUCT_
#define _MYSTRUCT_

//############此文件存放树的类型和相关函数###############

//布尔类型
typedef enum
{
	false,true
}bool;

//支持的数据类型
typedef union
{
	int intval;
	bool boolval;
	double doubleval;
	char *strval;
}Type;

//树的节点, yyval的类型将重定义为LTree
typedef struct treenode
{
	int type;	//标志树的类型(记号类型)
	int type2;		//数组元素的类型
	int returnType;		//此节点的返回值类型(取变量的类型)
	Type val;			//节点的值
	struct treenode *bro;	//(缩写brother)兄弟, 都是同一个父节点的孩子
	struct treenode *chi;	//(缩写children)孩子, 是第一个孩子, 他可能还有兄弟
}Tree,*LTree;


//常量型的叶子
//建立布尔常量叶子
LTree newBOOL(bool val);

//建立NUM常量叶子
LTree newNUM(int num);

//建立浮点数常量叶子
LTree newDOUBLE(double val);

//建立STRING常量叶子
LTree newSTRING(const char *name);


//建立变量相关的叶子
//建立ID叶子
LTree newID(char *name);

//只是用于声明的时候统一代码,临时建立的树
LTree newTYPE(int type);


//控制相关的叶子
//建立一颗不含语义的树,只是为了连接与它平行的语法树,也用于替换错误的语法树
LTree newNOP();

//建立一棵能退出循环的树
LTree newBREAK();

//建立一颗丢弃剩余的多条语句继续执行while循环的树
LTree newCONTINUE();

//建立一棵类型为type(记号类型)的树, type后跟多个类型也是LTree的孩子
LTree buildTree(int type, ...);
#endif
