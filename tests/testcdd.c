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

/*
 * Internal functions used to represent relations in a CDD
 */

// Returns the variable \a v of the CDD. if b is 0 the variable will be negated.
DdNode* getVar(DdManager *dd, int v, int b) {
  return b ?
  Cudd_bddIthVar(dd,v) :
  Cudd_Not(Cudd_bddIthVar(dd,v));
}
// Returns a CDD with the encoding of \a p. The arity \a a is needed because of 
// the way \a p is encoded.
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

/// Returns the number of tuples of width \a a represented in \a f
double card(DdManager *dd, DdNode *f, int a) {
  return Cudd_CountMinterm(dd,f,a<<BBV);
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

/*
 * Output of CDDs
 */
// Output in dot format
void todot(DdManager *dd, DdNode *f, const char * fname) {
  FILE *out = fopen(fname,"w");
  DdNode *outputs[] = {f};
  char *names[] = {"f"};
  Cudd_DumpDot(dd,1,outputs,NULL,names,out);
  fclose(out);
}
// Prints all the tuples of arity \a a in the CDD \a f
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


/*
 * Utility functions to manipulate binary relations represented in a CDD
 */
/// creates the pair (p,q) in the cdd
DdNode* pair(DdManager *dd, int p, int q) {
  DdNode *pr = path(dd,p,0);
  DdNode *qr = path(dd,q,1);
  DdNode *r = Cudd_cddAnd(dd,pr,qr);
  Cudd_Ref(r);
  Cudd_RecursiveDeref(dd,pr);
  Cudd_RecursiveDeref(dd,qr);
  return r;
}

/// Returns a CDD representing the binary relation stored in the file \a fname.
/// The relation has to be specified one pair per line
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


DdNode * small(DdManager *dd) {
  int d[] = {3, 3, 3, 3, 3, 3, 3};
  int r[] = {89572, 71799, 81066, 81183, 116335, 131302, 131591};
  int c = 7;
  int i;
  DdNode *rel = CDD_ZERO(dd);
  Cudd_Ref(rel);
  DdNode *p, *tmp;
  for (i = 0; i < c; i++) {
    p = pair(dd, d[i], r[i]);
    tmp = Cudd_cddOr(dd, p, rel);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(dd, p);
    Cudd_RecursiveDeref(dd, rel);
    rel = tmp;
  }
  assert(card(dd, rel, 2) == (double)c);
  return rel;
}

int ex1(void) {
  DdManager *dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);

  DdNode *one, *unk, *zero;
  one = CDD_ONE(dd);
  unk = CDD_UNK(dd);
  zero = CDD_ZERO(dd);

  DdNode *deps = readBinRel(dd, "deps.mat");
  DdNode *Deps = domain(dd, deps, CDD_ONE(dd));

  DdNode *conf = readBinRel(dd, "conf.mat");
  DdNode *Conf = domain(dd, conf, CDD_ONE(dd));

  DdNode *prov = readBinRel(dd, "pro.mat");
  DdNode *Prov = domain(dd, CDD_ZERO(dd), prov);
  
  DdNode *rels[] = {deps, conf, prov};
  

  int ds = Cudd_DagSize(deps); double dm = card(dd, deps, 2);
  int cs = Cudd_DagSize(conf); double cm = card(dd, conf, 2);
  int ps = Cudd_DagSize(prov); double pm = card(dd, prov, 2);
  
  printf("Statistics:\n");
  printf("\tdependencies: Dagsize: %d Minterms: %f\n", ds, dm);
  printf("\tconflicts: Dagsize: %d Minterms: %f\n", cs, cm);
  printf("\tprovides: Dagsize: %d Minterms: %f\n", ps, pm);
  printf("\ttotal minterms: %f\n", dm + cm + pm);
  printf("\ttotal number of nodes: %d\n", ds + cs + ps);
  printf("\tsharing (among vars) %d\n",Cudd_SharingSize(rels, 3));
  
  Cudd_RecursiveDeref(dd, deps);
  Cudd_RecursiveDeref(dd, Deps);
  Cudd_RecursiveDeref(dd, conf);
  Cudd_RecursiveDeref(dd, Conf);
  Cudd_RecursiveDeref(dd, prov);
  Cudd_RecursiveDeref(dd, Prov);
  printf("This number should be zero: %d\n",Cudd_CheckZeroRef(dd));
  return 0;
}


int main(void) {
  DdManager *dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
    
  DdNode *one, *unk, *zero;
  one = CDD_ONE(dd);
  unk = CDD_UNK(dd);
  zero = CDD_ZERO(dd);

  // f(x,y,z) = x'y'z + xz' + xy
  DdNode *x = Cudd_bddIthVar(dd,0);
  DdNode *y = Cudd_bddIthVar(dd,1);
  DdNode *z = Cudd_bddIthVar(dd,2);
  
  //Cudd_Ref(x);Cudd_Ref(y);Cudd_Ref(z);
  
  //tmp = xy
  DdNode *tmp = Cudd_bddAnd(dd, x, y);
  Cudd_Ref(tmp);
  
  //tmp2 = xz'
  DdNode *tmp2 = Cudd_bddAnd(dd, x,Cudd_Not(z));
  Cudd_Ref(tmp2);
  
  // tmp3 = x'y'
  DdNode *tmp3 = Cudd_bddAnd(dd, Cudd_Not(x), Cudd_Not(y));
  Cudd_Ref(tmp3);
  
  // tmp4 = tmp3 z
  DdNode *tmp4 = Cudd_bddAnd(dd, tmp3, z);
  Cudd_Ref(tmp4);
  
  Cudd_RecursiveDeref(dd, tmp3);
  
  DdNode *f = Cudd_bddOr(dd, tmp, tmp2);
  Cudd_Ref(f);
  Cudd_RecursiveDeref(dd, tmp);
  Cudd_RecursiveDeref(dd, tmp2);  
  
  tmp = Cudd_bddOr(dd, f, tmp4);
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd, f);
  Cudd_RecursiveDeref(dd, tmp4);
  
  f = tmp;
  Cudd_Ref(f);
  
  // create a cube containing z
  DdNode *cube = Cudd_bddAnd(dd, z, one);
  Cudd_Ref(cube);
  
  //DdNode *abse = Cudd_bddExistAbstract(dd, f, cube);
  DdNode *abse = Cudd_cddExistAbstract(dd, f, cube);
  Cudd_Ref(abse);
  
  todot(dd, abse, "cdd-abstract-exist.dot");
  
  Cudd_RecursiveDeref(dd, tmp);
  Cudd_RecursiveDeref(dd, abse);
  Cudd_RecursiveDeref(dd, cube);
  Cudd_RecursiveDeref(dd, f);
  printf("This number should be zero: %d\n",Cudd_CheckZeroRef(dd));
  return 0;
}