#include "cuddCdd.h"

DdNode* Cudd_cddNot(DdManager *manager, DdNode *f) {
  DdNode * unk = Cudd_ReadZero(manager);
  if (f == unk) return f;
  return Cudd_Not(f);
}

DdNode* Cudd_cddNotCond(DdManager *manager, DdNode *f, int c) {
  return c ? Cudd_cddNot(manager,f) : f;
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
    //printf("Swaping!!!\n");
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
      r = cuddUniqueInter(manager,(int)index,
                          Cudd_cddNot(manager,t),
                          Cudd_cddNot(manager,e));
      if (r == NULL) {
        assert(r);
        	Cudd_IterDerefBdd(manager, t);
        	Cudd_IterDerefBdd(manager, e);
        	return(NULL);
      }
      r = Cudd_cddNot(manager,r);
    } else {
      if (t == unk && Cudd_IsComplement(e)) {
        //printf("This is an special case\n");
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


int isOne(DdManager *manager, DdNode *f) {
  assert(f);
  DdNode *one, *F;
  one = Cudd_ReadOne(manager);
  F = Cudd_Regular(f); assert(F);
  
  return(F == one && f == one) ;
}

int isZero(DdManager *manager, DdNode *f) {
  assert(f);
  DdNode *one, *unk, *zero, *F;
  one = Cudd_ReadOne(manager);
  zero = Cudd_ReadLogicZero(manager);
  F = Cudd_Regular(f); assert(F);
  
  return(F == one && f == zero) ;
}

int isUnk(DdManager *manager, DdNode *f) {
  assert(f);
  DdNode *unk, *F;
  unk = Cudd_ReadZero(manager);
  F = Cudd_Regular(f); assert(F);
  if (f == unk) {
    assert(F == unk);
    return 1;
  }
  return 0;
}


DdNode* cuddCddMergeRecur(DdManager * manager, DdNode * f, DdNode * g) {
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

  if (F == G) {
    if (f == g) return(f); // f \merge f = f
    else { // f \merge \lnot f = failure
      manager->errorCode = CDD_FAIL;
      //printf("Failed merge\n");
      return NULL;
    }
  }
  
  if (isOne(manager,f)) {
    if (isZero(manager,g)) {
      manager->errorCode = CDD_FAIL;
      //printf("Failed merge\n");
      return NULL;
    } 
    if (isOne(manager,g) || isUnk(manager,g)) {
      return one;
    }
  } else if (isZero(manager,f)) {
    if (isOne(manager,g)) {
      manager->errorCode = CDD_FAIL;
      //printf("Failed merge\n");
      return NULL;
    } 
    if (isZero(manager,g) || isUnk(manager,g)) {
      return zero;
    }
  }

  if (isOne(manager,g)) {
    if (isZero(manager,f)) {
      manager->errorCode = CDD_FAIL;
      //printf("Failed merge\n");
      return NULL;
    } 
    if (isOne(manager,f) || isUnk(manager,f)) {
      return one;
    }
  } else if (isZero(manager,g)) {
    if (isOne(manager,f)) {
      manager->errorCode = CDD_FAIL;
      //printf("Failed merge\n");
      return NULL;
    } 
    if (isZero(manager,f) || isUnk(manager,f)) {
      return zero;
    }
  }

  if (isUnk(manager,f) && isUnk(manager,g)) {
    return unk;
  }
  
  if (isUnk(manager,f)) return g;
  if (isUnk(manager,g)) return f;
 
  if ( (g == one || g == zero) || f > g) { /* normalize to try to increase cache efficiency. */
    printf("Swaping!!!\n");
    DdNode *tmp = f;
    f = g;
    g = tmp;
    assert(f);
    assert(g);
    F = Cudd_Regular(f); assert(F);
    G = Cudd_Regular(g); assert(G);
  }

  assert(g != one && g != unk && g != zero);
  // at this point the possible cases are:
  // one \merge exp
  // zero \merge exp
  // exp \merge exp

   /* Check cache. */
  
  if (F->ref != 1 || G->ref != 1) {
    r = cuddCacheLookup2(manager, Cudd_cddMerge, f, g);
    if (r != NULL) return(r);
  }

  if (f == one || f == zero) {
    // only g contains a non terminal expression
    topg = manager->perm[G->index];
    index = G->index;
    gv = cuddT(G); assert(gv);
    gnv = cuddE(G); assert(gnv);
    if (Cudd_IsComplement(g)) {
      gv = Cudd_cddNot(manager,gv);
      gnv = Cudd_cddNot(manager,gnv);
    }
    fv = fnv = f;
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
  t = cuddCddMergeRecur(manager, fv, gv);
  assert(t);
  if (t == NULL)  return(NULL);
  cuddRef(t);
  
  // Compute the 'else'
  e = cuddCddMergeRecur(manager, fnv, gnv);
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
	//printf("This is an special case\n");
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
    cuddCacheInsert2(manager,  Cudd_cddMerge, f, g, r);
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



DdNode * Cudd_cddOr(DdManager *dd, DdNode *f, DdNode *g) {
  DdNode *res;
  do {
    dd->reordered = 0;
    res = cuddCddAndRecur(dd,Cudd_cddNot(dd,f),Cudd_cddNot(dd,g));
  } while (dd->reordered == 1);
  res = Cudd_cddNotCond(dd,res,res != NULL);
  return(res);
}

DdNode* Cudd_cddMerge(DdManager *dd, DdNode *f, DdNode *g) {
  DdNode *res;
  do {
    dd->reordered = 0;
    res = cuddCddMergeRecur(dd,f,g);
  } while (dd->reordered == 1);
  return(res);  
}

