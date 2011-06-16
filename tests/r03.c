#include "util.h"
#include "cuddInt.h"

// 32 bits integers max 8ary 
// #define BBV 5
// #define BA 3

// 4 bits integers and binary relations
#define BBV 2
#define BA 1

#define BV 1<<BBV

enum TST {
  TST_T, // true
  TST_U, // unknown
  TST_F  // false
};

// Output in dot format
void todot(DdManager *manager, DdNode *f, const char * fname) {
  FILE *out = fopen(fname,"w");
  DdNode *outputs[] = {f};
  char *names[] = {"f"};
  Cudd_DumpDot(manager,1,outputs,NULL,names,out);
  fclose(out);
}
// Complement (negation)
DdNode* Cudd_cddNot(DdManager *manager, DdNode *f) {
  DdNode * unk = Cudd_ReadZero(manager);
  if (f == unk) return f;
  return Cudd_Not(f);
}

// Returns the variable v. if b is 0 it will be negated
DdNode* getVar(DdManager *manager, int v, int b) {
  //	assert(v < BV);
  return b ?
    Cudd_bddIthVar(manager,v) :
    Cudd_cddNot(manager,Cudd_bddIthVar(manager,v));
}



DdNode * cuddCddAndRecur(DdManager *manager, DdNode *f, DdNode *g) {
  DdNode *F, *fv, *fnv, *G, *gv, *gnv;
  DdNode *one, *zero, *unk, *r, *t, *e;
  unsigned int topf, topg, index;

  statLine(manager);
  one = Cudd_ReadOne(manager);
  unk = Cudd_ReadZero(manager);
  zero = Cudd_ReadLogicZero(manager);

  /* Terminal cases. */
  F = Cudd_Regular(f); assert(F);
  G = Cudd_Regular(g); assert(G);
  
  if (F == G)
    if (f == g) return(f); // f \land f = f
 
  if (F == unk) {
    assert(f == unk);
    if (G == one) {
      if (g == one) return unk; // unk \land true = unk 
      else
	return zero; // unk \land false = false
    }
    // unk \land unk = unk
    if (G == unk) {
      assert(g == unk);
      return unk;
    }
  }
  if (G == unk) {
    assert(g == unk);
    if (F == one) {
      if (f == one) return unk; // true \land unk = unk
      else
	return zero; // false \land unk = false
    }
    if (F == unk) {
      assert(f == unk);
      return unk;
    }
  }
  if (F == one) {
    if (f == one) return(g);  // true \land g = g
    else return(zero);        // false \land g = false
  }
  if (G == one) { 
    if (g == one) return(f); // f \land true = f
    else return(zero);       // f \land false = false
  }
  
  if ( g == unk || f > g) { /* normalize to try to increase cache efficiency. */
    printf("Swaping!!!\n");
    DdNode *tmp = f;
    f = g;
    g = tmp;
    assert(f);
    assert(g);
    F = Cudd_Regular(f); assert(F);
    G = Cudd_Regular(g); assert(G);
  }

  /* Check cache. */
  
  if (F->ref != 1 || G->ref != 1) {
    printf("HIT!!!!!!!\n");
    r = cuddCacheLookup2(manager, Cudd_bddAnd, f, g);
    if (r != NULL) return(r);
  }
  
  // At this point there are still two cases in which a terminal can appear:
  // f \land unk   unk \land g
  // because of the normalization unk will be in as the first argument
  if (f == unk) {
    assert(g != unk);
    // only g contains a non terminal expression
    topg = manager->perm[G->index];
    index = G->index;
    gv = cuddT(G); assert(gv);
    gnv = cuddE(G); assert(gnv);
    if (Cudd_IsComplement(g)) {
      gv = Cudd_cddNot(manager,gv);
      gnv = Cudd_cddNot(manager,gnv);
    }
    fv = fnv = unk;
  } else {
    /* Here we can skip the use of cuddI, because the operands are known
    ** to be non-constant.
    */
    topf = manager->perm[F->index];
    topg = manager->perm[G->index];

    /* Compute cofactors. */
    if (topf <= topg) {
      index = F->index;
      fv = cuddT(F);
      fnv = cuddE(F);
      assert(fv);
      assert(fnv);
      if (Cudd_IsComplement(f)) {
	fv = Cudd_cddNot(manager,fv);
	fnv = Cudd_cddNot(manager,fnv);
      }
    } else {
      index = G->index;
      fv = fnv = f;
    }
    
    if (topg <= topf) {
      gv = cuddT(G);
      gnv = cuddE(G);
      assert(gv);
      assert(gnv);
      if (Cudd_IsComplement(g)) {
	gv = Cudd_cddNot(manager,gv);
	gnv = Cudd_cddNot(manager,gnv);
      }
    } else {
      gv = gnv = g;
    } 
  }
  
  // Compute the 'then'
  t = cuddCddAndRecur(manager, fv, gv);
  assert(t);
  if (t == NULL)  return(NULL);
  cuddRef(t);
  
  // Compute the 'else'
  e = cuddCddAndRecur(manager, fnv, gnv);
  assert(e);
  if (e == NULL) {
    Cudd_IterDerefBdd(manager, t);
    return(NULL);
  }
  cuddRef(e);

  // Reduce the result if possible
  if (t == e) {
    r = t;
  } else {
    if (Cudd_IsComplement(t)) {
      r = cuddUniqueInter(manager,(int)index,Cudd_cddNot(manager,t),Cudd_cddNot(manager,e));
      if (r == NULL) {
	assert(r);
	Cudd_IterDerefBdd(manager, t);
	Cudd_IterDerefBdd(manager, e);
	return(NULL);
      }
      r = Cudd_cddNot(manager,r);
    } else {
      if (t == unk && Cudd_IsComplement(e)) {
	printf("This is an special case\n");
	r = cuddUniqueInter(manager,(int)index,unk,Cudd_cddNot(manager,e));
	if (r == NULL) {
	  assert(r);
	  Cudd_IterDerefBdd(manager,e);
	  return(NULL);
	}
	r = Cudd_cddNot(manager,r);
      } else {
	r = cuddUniqueInter(manager,(int)index,t,e);
	if (r == NULL) {
	  assert(r);
	  Cudd_IterDerefBdd(manager, t);
	  Cudd_IterDerefBdd(manager, e);
	  return(NULL);
	}
      }
    }
  }
  cuddDeref(e);
  cuddDeref(t);
  if (F->ref != 1 || G->ref != 1)
    cuddCacheInsert2(manager, Cudd_bddAnd, f, g, r);
  return(r);
}
DdNode* Cudd_cddAnd(DdManager * dd, DdNode * f, DdNode * g) {
  DdNode *res;
  do {
    dd->reordered = 0;
    res = cuddCddAndRecur(dd,f,g);
  } while (dd->reordered == 1);
  return(res);
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

// Returns a function that evaluates to terminal \a term for the 
// representation of p in column a
//DdNode* path(DdManager *manager, int p, int a) {
//  DdNode *tmp, *f, *var;
  /*  
  f = Cudd_ReadOne(manager);
  Cudd_Ref(f);
  
  for (int i = 3; i >= 0; i--) {
    var = Cudd_bddIthVar(manager,i);
    tmp = Cudd_bddAnd(manager,var,f);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(manager,f);
    f = tmp;
  }
  */
  /*
    f = Cudd_ReadOne(manager);
    Cudd_Ref(f);
    for (int i = BV; i--;) {
    printf("current i%d\n",i);
    tmp = Cudd_bddAnd(manager,getVar(manager,(i<<BA)+a,p&1),f);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(manager,f);    
    f = tmp;
    p >>= 1; 
    }
  */
// return f;
//}


/*
DdNode* pair(DdManager *manager, int p, int q, enum TST s) {
  DdNode *pr = path(manager,p,0,s);
  DdNode *qr = path(manager,q,1,s);
  DdNode *u = Cudd_cddAnd(manager,pr,qr);
  Cudd_RecursiveDeref(manager,pr);	
  Cudd_RecursiveDeref(manager,qr);	
  return u;
}
*/
int main(void) {
  DdManager *manager = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);		
  
  //DdNode *one, *unk, *zero;
  /*
  one = Cudd_ReadOne(manager);
  unk = Cudd_ReadZero(manager);
  zero = Cudd_ReadLogicZero(manager);
  */
  //printf("Constants one: %d, unk:%d, zero:%d\n",one,unk,zero);
  
  int d[] = {1,0,0,0};
  DdNode *p = path(manager,d,4);
  //DdNode *p = path(manager,4,1);
  //Cudd_Ref(p);
  //Cudd_RecursiveDeref(manager,p);

  printf("This number should be zero: %d\n",Cudd_CheckZeroRef(manager));
  return 0;
}
