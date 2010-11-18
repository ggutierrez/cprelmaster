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
    tmp = Cudd_cddAnd(manager,getVar(manager,(i<<BA)+a,p&1),f);
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

/// Returns the number of tuples of width \a a represented in \a f
double card(DdManager *dd, DdNode *f, int a) {
  return Cudd_CountMinterm(dd,f,a<<BBV);
}

/// creates the pair (p,q) in the cdd
DdNode* pair(DdManager *manager, int p, int q) {
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
  int err;
  int u, v;
  int edges = 0;
  DdNode *t, *rel, *tmp;
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
    t = pair(dd,u,v); assert(t);
    tmp = Cudd_cddOr(dd,rel,t);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(dd,t);
    Cudd_RecursiveDeref(dd,rel);
    rel = tmp;
    edges++;
  } while (1);

  printf("Edges: %d card: %f\n",edges,card(dd,rel,2));
  fclose(fp);
  return rel;
}

int main(void) {
  DdManager *manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);	
  
  DdNode *one, *unk, *zero;
  one = Cudd_ReadOne(manager);
  unk = Cudd_ReadZero(manager);
  zero = Cudd_ReadLogicZero(manager);

  DdNode *deps = readBinRel(manager, "deps.mat");
  Cudd_RecursiveDeref(manager, deps);
  printf("This number should be zero: %d\n",Cudd_CheckZeroRef(manager));
  return 0;
}
