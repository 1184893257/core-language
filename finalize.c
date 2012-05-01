#include <stdlib.h>
#include "finalize.h"
#include "id.h"
#include "myparser.h"

static void finalizeTree(LTree root)
{
	void **p;

	if(!root) return;
	if(root->bro) finalizeTree(root->bro);
	if(root->chi) finalizeTree(root->chi);
	switch(root->type)
	{
	case STRING:
		free(root->val.strval);
		break;
	case ARRAY:
		p=getPoint(root);
		if(*p) free(*p);
		*p=NULL;
		break;
	}
	free(root);
}

void finalize(LTree root)
{
	finalizeTree(root);
	destroyVars();
}