#ifndef _MYID_
#define _MYID_
#include "struct.h"
//############此文件存放变量的各种操作###############
//初始化
void idinit();

//声明变量,type是包含类型的树,name是包含变量名的树
void declare(LTree type, LTree name);

//引用变量
void refer(LTree idTree);

//在执行之前生成变量的存储空间,将所有字节置0
void generateVars();

//对有左值的变量赋值
void putvalue(LTree idTree, Type value);

//读变量的值
Type getvalue(LTree idTree);

//声明数组
void declareArray(LTree type, LTree name);

//引用数组, array是存有数组名字的节点, offset是能算出数组下标的树
void referArray(LTree array, LTree offset);

//给数组申请新的空间,array是数组ID节点,size是返回值为整数的运算树
LTree newArray(LTree array, LTree size);

//执行NEW节点,给数组重新申请空间
void arrayNewSize(LTree node);
#endif