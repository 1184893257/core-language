#ifndef _MYID_
#define _MYID_
#include "struct.h"
//############此文件存放变量的各种操作###############

//声明变量
void declare(int type, char *name);

//引用变量
void refer(LTree idTree);

//在执行之前生成变量的存储空间,将所有字节置0
void generateVars();

//对有左值的变量赋值
void putvalue(LTree idTree, Type value);

//读变量的值
Type getvalue(LTree idTree);

//声明数组,尚未完工
void declareArray(int type, char *name);

//引用数组, id是存有数组名字的节点, offset是能算出数组下标的树,尚未完工
void referArray(LTree id, LTree offset);

//给数组申请新的空间,array是数组ID节点,size是返回值为整数的运算树,尚未完工
void newArray(LTree array, LTree size);
#endif