#include <stdio.h>
#include "execute.h"
#include "myparser.h"
#include "id.h"
#include "print.h"

Type do_solve(LTree root)
{
	Type ans,t1,t2;
	LTree c1,c2,c3;

	switch(root->type)
	{
	//NOP
	case NOP: break;	//ֱ�ӱ�������

	//NEW
	case NEW:
		arrayNewSize(root); break;

	//main
	case MAIN: do_solve(root->chi); break;

	//�������
	case MULTI2:
		for(c1=root->chi;c1;c1=c1->bro)
			do_solve(c1);
		break;

	//while
	case WHILE:
		c1=root->chi;
		c2=c1->bro;
		while(do_solve(c1).boolval==true)
			do_solve(c2);
		break;

	//����ȫƥ���if else
	case IFU:
		c1=root->chi;
		c2=c1->bro;
		if(do_solve(c1).boolval==true)
			do_solve(c2);
		break;

	//��ȫƥ���if else
	case IFF:
		c1=root->chi;
		c2=c1->bro;
		c3=c2->bro;
		if(do_solve(c1).boolval==true)
			do_solve(c2);
		else
			do_solve(c3);
		break;

	//print����
	case PRINT:
		print(root); break;

	//����
	case NUM: case BOOL:
		ans=root->val; break;

	//������ֵ
	case INT: case TBOOL: case ARRAY:
		ans=getvalue(root); break;

	//��ֵ
	case '=':
		c1=root->chi;
		c2=c1->bro;
		ans=do_solve(c2);
		putvalue(c1,ans);
		break;

	//��Ԫ�����
	case '!': case NEG:
		c1=root->chi;
		t1=do_solve(c1);
		switch(root->type)
		{
		case '!': ans.boolval=!(t1.boolval); break;
		case NEG: ans.intval=-t1.intval; break;
		}
		break;

	//˫Ԫ�����
	default:
		c1=root->chi;
		c2=c1->bro;
		t1=do_solve(c1);
		t2=do_solve(c2);
		switch(root->type)
		{
		case '+': ans.intval=t1.intval+t2.intval; break;
		case '-': ans.intval=t1.intval-t2.intval; break;
		case '*': ans.intval=t1.intval*t2.intval; break;
		case '/': ans.intval=t1.intval/t2.intval; break;

		case EQUAL: ans.boolval=t1.intval==t2.intval; break;
		case AND: ans.boolval=t1.boolval && t2.boolval; break;
		case OR: ans.boolval=t1.boolval || t2.boolval; break;
		case '<': ans.boolval=t1.intval < t2.intval; break;
		case '>': ans.boolval=t1.intval > t2.intval; break;
		default:
			fprintf(stderr,"����Ĳ�����:%d\n",root->type); break;
		}
		break;
	}

	return ans;
}