#include "util.h"
#include "cuddInt.h"


int main(void) {
	DdManager *manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
	
	DdNode *one = DD_ONE(manager);
	DdNode *zero = Cudd_Not(one);
	
	DdNode *tmp;
	DdNode *f, *g;
	
	
	DdNode *b0 = Cudd_bddIthVar(manager,0);
	DdNode *b1 = Cudd_bddIthVar(manager,1);
	
	// Encode f(1,0) = true
	tmp = Cudd_bddAnd(manager,b0,Cudd_Not(b1));
	Cudd_Ref(tmp);
	f = Cudd_bddAnd(manager,tmp,one);
	Cudd_Ref(f);
	Cudd_RecursiveDeref(manager,tmp);
	
	// Encode g(0,1) = true
	tmp = Cudd_bddAnd(manager,Cudd_Not(b0),b1);
	Cudd_Ref(tmp);
	g = Cudd_bddAnd(manager,tmp,one);
	Cudd_Ref(g);
	Cudd_RecursiveDeref(manager,tmp);
	
	FILE *out = fopen("r01.dot","w");
	DdNode *outputs[] = {f,g};
	char *names[] = {"f", "g"};
	Cudd_DumpDot(manager,2,outputs,NULL,names,out);
	fclose(out);
	
	printf("Paths in f: %f\n",Cudd_CountPathsToNonZero(f));
	printf("Paths in g: %f\n",Cudd_CountPathsToNonZero(g));

	Cudd_RecursiveDeref(manager,f);
	Cudd_RecursiveDeref(manager,g);
	
	printf("This number should be zero: %d\n",Cudd_CheckZeroRef(manager));
	return 0;
}