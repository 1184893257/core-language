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
	case PRINT:
		print(root); break;
	case NUM: case BOOL:
		ans=root->val; break;
	case '=':
		c1=root->chi;
		c2=c1->bro;
		ans=do_solve(c2);
		putvalue(c1,ans);
		break;
	case '!': case NEG:
		c1=root->chi;
		t1=do_solve(c1);
		switch(root->type)
		{
		case '!': ans.boolval=!(t1.boolval); break;
		case NEG: ans.intval=-t1.intval; break;
		}
		break;
	/*case '+': case '-': case '*': case '/':
	case EQUAL: case NOTEQUAL: case AND: case OR: case '<': case '>':*/
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
		//case NOTEQUAL: ans.boolval=t1.intval!=t2.intval; break;
		case AND: ans.boolval=t1.boolval && t2.boolval; break;
		case OR: ans.boolval=t1.boolval || t2.boolval; break;
		case '<': ans.boolval=t1.intval < t2.intval; break;
		case '>': ans.boolval=t1.intval > t2.intval; break;
		default:
			fprintf(stderr,"´íÎóµÄ²Ù×÷·û:%d\n",root->type); break;
		}
		break;
	}

	return ans;
}