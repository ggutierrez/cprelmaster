#include "util.h"
#include "cuddInt.h"

#define VARS 4

void todot(DdManager *manager, DdNode *f, const char * fname) {
  FILE *out = fopen(fname,"w");
  DdNode *outputs[] = {f};
  char *names[] = {"f"};
  Cudd_DumpDot(manager,1,outputs,NULL,names,out);
  fclose(out);
}

// Returns the variable v. if b is 0 it will be negated
DdNode* getVar(DdManager *manager, int v, int b) {
  return b ?
    Cudd_bddIthVar(manager,v) :
    Cudd_Not(Cudd_bddIthVar(manager,v));
}

DdNode* path(DdManager *manager, int* p, int vars) {
  DdNode *tmp, *var, *f;
  f = Cudd_ReadOne(manager);
  Cudd_Ref(f);
  for (int i = vars; i--;) {
    tmp = Cudd_bddAnd(manager,getVar(manager,i,p[i]),f);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(manager,f);
    f = tmp;
  }
  return f;
}

int main(void) {
  DdManager *manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);	
  //DdNode *one = DD_ONE(manager);
  //DdNode *zero = Cudd_Not(one);
	
  //DdNode *tmp;
  //DdNode *f;
	
  int p1[] = {1,0,0,0};
  // DdNode *v = getVar(manager,0,1);
  DdNode *p = path(manager,p1,4);

  printf("Is the new path an evaluation of the bdd? %d\n",Cudd_Eval(manager,p,p1) == DD_ONE(manager));
	
  todot(manager,p,"tuple1000.dot");
  printf("Paths to one in p: %f from a total of %f\n",
	 Cudd_CountPathsToNonZero(p),
	 Cudd_CountPath(p));
	
	
  Cudd_RecursiveDeref(manager,p);	
  printf("This number should be zero: %d\n",Cudd_CheckZeroRef(manager));
  return 0;
}
