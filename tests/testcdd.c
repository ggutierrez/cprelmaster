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
void todot(DdManager *dd, DdNode *f, const char * fname) {
  FILE *out = fopen(fname,"w");
  DdNode *outputs[] = {f};
  char *names[] = {"f"};
  Cudd_DumpDot(dd,1,outputs,NULL,names,out);
  fclose(out);
}

// Returns the variable v. if b is 0 it will be negated
inline
DdNode* getVar(DdManager *dd, int v, int b) {
  return b ?
    Cudd_bddIthVar(dd,v) :
    Cudd_Not(Cudd_bddIthVar(dd,v));
}
inline
DdNode* path(DdManager *dd, int p, int a) {
  DdNode *tmp, *f;
  int i;
  f = Cudd_ReadOne(dd);
  Cudd_Ref(f);
  for (i = BV; i--;) {
    tmp = Cudd_cddAnd(dd,getVar(dd,(i<<BA)+a,p&1),f);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(dd,f);    
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
inline
double card(DdManager *dd, DdNode *f, int a) {
  return Cudd_CountMinterm(dd,f,a<<BBV);
}

/// creates the pair (p,q) in the cdd
inline
DdNode* pair(DdManager *dd, int p, int q) {
  DdNode *pr = path(dd,p,0);
  DdNode *qr = path(dd,q,1);
  DdNode *r = Cudd_cddAnd(dd,pr,qr);
  Cudd_Ref(r);
  Cudd_RecursiveDeref(dd,pr);
  Cudd_RecursiveDeref(dd,qr);
  return r;
}

DdNode* var(DdManager *dd) {
  DdNode *one, *unk, *zero;
  one = Cudd_ReadOne(dd);
  unk = Cudd_ReadZero(dd);
  zero = Cudd_ReadLogicZero(dd);
  
  // will create the relation domain: {s}..{r,s,q}
  
  DdNode *r = pair(dd,3,2);
  DdNode *s = pair(dd,1,2);
  DdNode *q = pair(dd,5,3);
 
  DdNode *rq = Cudd_cddOr(dd,r,q);
  Cudd_Ref(rq);
  DdNode *rsq = Cudd_cddOr(dd,rq,s);
  Cudd_Ref(rsq);
  Cudd_RecursiveDeref(dd,rq);

  Cudd_RecursiveDeref(dd,r);
  Cudd_RecursiveDeref(dd,q);

  DdNode *ub = Cudd_cddAnd(dd,rsq,unk);
  Cudd_Ref(ub);
  Cudd_RecursiveDeref(dd,rsq);
  
  DdNode *lb = Cudd_cddOr(dd,s,unk);
  Cudd_Ref(lb);
  Cudd_RecursiveDeref(dd,s);
  
  DdNode *v = Cudd_cddMerge(dd,lb,ub);
  Cudd_Ref(v);
  
  printTuples(dd,v,2);
  todot(dd,v,"var.dot");

  Cudd_RecursiveDeref(dd,ub);
  Cudd_RecursiveDeref(dd,lb);

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
    tmp = Cudd_cddOr(dd,rel,t); assert(tmp);
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

/// Creates a CDD representing the domain [glb,lub]
DdNode* domain(DdManager *dd, DdNode *glb, DdNode *lub) {
  DdNode *f, *g, *r;
  Cudd_Ref(glb);
  Cudd_Ref(lub);
  f = Cudd_cddOr(dd, glb, CDD_UNK(dd));
  Cudd_Ref(f);
  Cudd_RecursiveDeref(dd, glb);

  g = Cudd_cddAnd(dd, lub, CDD_UNK(dd));
  Cudd_Ref(g);
  Cudd_RecursiveDeref(dd, lub);
  
  r = Cudd_cddMerge(dd, f, g);
  Cudd_RecursiveDeref(dd, f);
  Cudd_RecursiveDeref(dd, g);

  return r;
}

int main(void) {
  DdManager *dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);	
  
  DdNode *one, *unk, *zero;
  one = CDD_ONE(dd);
  unk = CDD_UNK(dd);
  zero = CDD_ZERO(dd);

  DdNode *deps = readBinRel(dd, "deps.mat");
  
  DdNode *Deps = domain(dd, deps, CDD_ONE(dd));

  DdNode *DepsUnk = Cudd_cddStatus(dd, Deps, Cudd_cddNot(dd, Deps));
  //DdNode *DepsUnk = Cudd_cddStatus(dd, Deps, Deps);
  printf("Cardinality: %f\n",card(dd, DepsUnk, 2));
  //printTuples(dd, DepsUnk, 2);
  Cudd_RecursiveDeref(dd, deps);
  printf("This number should be zero: %d\n",Cudd_CheckZeroRef(dd));
  return 0;
}
