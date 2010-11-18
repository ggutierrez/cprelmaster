#include "cuddCdd.h"

// 32 bits integers max 8ary 
#define BBV 5
#define BA 3

// 4 bits integers and binary relations
//#define BBV 2
//#define BA 1

//#define BBV 3
//#define BA 2

#define BV 1<<BBV

// Output in dot format
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

DdNode* path(DdManager *manager, int p, int a) {
  DdNode *tmp, *f;
  int i;
  f = Cudd_ReadOne(manager);
  Cudd_Ref(f);
  for (i = BV; i--;) {
    //printf("current i[%d] = %d\n",BV+1-i,p&1);
    tmp = Cudd_bddAnd(manager,getVar(manager,(i<<BA)+a,p&1),f);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(manager,f);    
    f = tmp;
    p >>= 1; 
  }
  return f;
}

void printTuples(DdManager* dd, DdNode* f, int a){
  DdGen* gen;
  int *cube = (int*)malloc(sizeof(int)*(1<<(BBV+BA)));
  int tuple[1<<BA];
  CUDD_VALUE_TYPE val;
  int done;
  int i,k,j;
  printf("Cardinality: %f\n", Cudd_CountMinterm(dd,f,a<<BBV));
  for(k=0;k<1<<BA;k++)tuple[k]=0;
  Cudd_ForeachCube(dd,f,gen,cube,val){
    done=0;
    while(!done){
     done=1;
     for(i=(1<<(BBV+BA))-1;i>=0;i--){
       if((i&((1<<BA)-1))<a){
        tuple[i&((1<<BA)-1)]&=~(1<<((1<<BBV)-1-(i>>BA)));
        tuple[i&((1<<BA)-1)]|=(cube[i]&1)<<((1<<BBV)-1-(i>>BA));
        if((cube[i]&2)&&done){
          done&=cube[i]&1;
          cube[i]^=1;
        }
       }
     }
     printf("<");
     for(j = 0; j < a; j++) {
       printf("%d,",tuple[j]);
     }
     printf(">\n");
    }
  }
}

/// creates the pair (p,q) in the cdd
DdNode* pair(DdManager *manager, int p, int q) {
  //printf("called pair\n");
  DdNode *pr = path(manager,p,0);
  DdNode *qr = path(manager,q,1);
  DdNode *r = Cudd_cddAnd(manager,pr,qr);
  Cudd_Ref(r);
  Cudd_RecursiveDeref(manager,pr);
  Cudd_RecursiveDeref(manager,qr);
  return r;
}

DdNode* var(DdManager *manager) {
  DdNode *one, *unk, *zero;
  one = Cudd_ReadOne(manager);
  unk = Cudd_ReadZero(manager);
  zero = Cudd_ReadLogicZero(manager);
  
  // will create the relation domain: {s}..{r,s,q}
  
  DdNode *r = pair(manager,3,2);
  DdNode *s = pair(manager,1,2);
  DdNode *q = pair(manager,5,3);
 
  DdNode *rq = Cudd_cddOr(manager,r,q);
  Cudd_Ref(rq);
  DdNode *rsq = Cudd_cddOr(manager,rq,s);
  Cudd_Ref(rsq);
  Cudd_RecursiveDeref(manager,rq);

  Cudd_RecursiveDeref(manager,r);
  Cudd_RecursiveDeref(manager,q);

  DdNode *ub = Cudd_cddAnd(manager,rsq,unk);
  Cudd_Ref(ub);
  Cudd_RecursiveDeref(manager,rsq);
  
  DdNode *lb = Cudd_cddOr(manager,s,unk);
  Cudd_Ref(lb);
  Cudd_RecursiveDeref(manager,s);
  
  DdNode *v = Cudd_cddMerge(manager,lb,ub);
  Cudd_Ref(v);
  
  printTuples(manager,v,2);
  todot(manager,v,"var.dot");

  Cudd_RecursiveDeref(manager,ub);
  Cudd_RecursiveDeref(manager,lb);

  return v;
}

DdNode* readBinRel(DdManager *dd, const char* fname) {
  int err; // error code for file handling
  int u, v; // every node read from the input
  int edges = 0;
  DdNode *ur, *vr, *t, *rel, *tmp;
  FILE *fp = fopen(fname,"r");
  if (!fp) {
    return NULL;
  }
  printf("Reading the file %s\n",fname);

  rel = Cudd_ReadLogicZero(dd);
  Cudd_Ref(rel);
  do {
    err = fscanf(fp, "%d %d", &u, &v);
    if (err == EOF) {
      printf("Closing file\n");
      fclose(fp);
      break;
    }
    ur = path(dd,u,0); assert(ur);
    vr = path(dd,v,1); assert(vr);
    t = Cudd_bddAnd(dd,ur,vr); assert(t);
    Cudd_Ref(t);
    Cudd_RecursiveDeref(dd,ur);
    Cudd_RecursiveDeref(dd,vr);

    tmp = Cudd_bddOr(dd,rel,t);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(dd,t);
    Cudd_RecursiveDeref(dd,rel);
    rel = tmp;
    edges++;
  } while (1);

  printf("Edges: %d\n",edges);
  fclose(fp);
  return rel;
}

int main(void) {
  DdManager *manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);	
  
  DdNode *one, *unk, *zero;
  one = Cudd_ReadOne(manager);
  unk = Cudd_ReadZero(manager);
  zero = Cudd_ReadLogicZero(manager);

  //DdNode *deps = readBinRel(manager,"deps.mat");
  //printf("Cardinality: %f\n", Cudd_CountMinterm(manager,deps,2<<BBV));

  // upper bound
  /* DdNode *ub = Cudd_cddAnd(manager,deps,unk); */
  /*   Cudd_Ref(ub); */

  /*   DdNode *lb = Cudd_cddOr(manager,deps,unk); */
  /*   Cudd_Ref(lb); */

//  todot(manager,deps,"deps.dot");
  
  DdNode *v = var(manager);
  Cudd_RecursiveDeref(manager,v);
  /* Cudd_RecursiveDeref(manager,ub); */
  /* Cudd_RecursiveDeref(manager,lb); */
  /* Cudd_RecursiveDeref(manager,deps); */
  /*
    DdNode *r = pair(manager,3,2);
    DdNode *s = pair(manager,1,2);
  DdNode *q = pair(manager,5,3);

  DdNode *t = Cudd_cddOr(manager,r,s);
  Cudd_Ref(t);
  DdNode *u = Cudd_cddOr(manager,t,q);
  Cudd_Ref(u);
  printTuples(manager,u,2);
  */
  //printf("Result %d\n", Cudd_cddMerge(manager,zero,one) == NULL && Cudd_ReadErrorCode(manager) == CDD_FAIL);
  //printf("Result %d\n", Cudd_cddMerge(manager,zero,unk) == zero);
  //  printf("Result %d\n", Cudd_cddMerge(manager,zero,zero) == zero); //NULL && Cudd_ReadErrorCode(manager) == CDD_FAIL);


  //printf("Result %d\n", Cudd_cddMerge(manager,r,Cudd_cddNot(manager,r)) == NULL && Cudd_ReadErrorCode(manager) == CDD_FAIL);
  //printf("Result %d\n", Cudd_cddMerge(manager,unk,r) == r);


  //  printf("Result %d\n", Cudd_cddMerge(manager,one,one) == one);

  /*
  todot(manager,p,"normal.dot");
  printf("Paths to one in p: %f from a total of %f\n",
	 Cudd_CountPathsToNonZero(p),
	 Cudd_CountPath(p));

  DdNode *q = Cudd_cddAnd(manager,p,unk);
  Cudd_Ref(q);
  Cudd_RecursiveDeref(manager,p);
  todot(manager,q,"unknown.dot");
  */
  
  //printf("Is the new path an evaluation of the bdd? %d\n",Cudd_Eval(manager,p,p1) == DD_ONE(manager));
  /*
  //todot(manager,t,"pair.dot");
  Cudd_RecursiveDeref(manager,r);
  Cudd_RecursiveDeref(manager,s);

  Cudd_RecursiveDeref(manager,t);
  Cudd_RecursiveDeref(manager,q);
  Cudd_RecursiveDeref(manager,u);
  */
  printf("This number should be zero: %d\n",Cudd_CheckZeroRef(manager));
  return 0;
}
