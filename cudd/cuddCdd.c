#include "cuddCdd.h"

DdNode* Cudd_cddNot(DdManager *dd, DdNode *f) {
  if (f == CDD_UNK(dd)) return f;
  return Cudd_Not(f);
}

DdNode* Cudd_cddNotCond(DdManager *dd, DdNode *f, int c) {
  return c ? Cudd_cddNot(dd,f) : f;
}

DdNode * cuddCddAndRecur(DdManager *dd, DdNode *f, DdNode *g) {
  DdNode *F, *fv, *fnv, *G, *gv, *gnv;
  DdNode *one, *zero, *unk, *r, *t, *e;
  unsigned int topf, topg, index;
  
  statLine(dd);
  one = CDD_ONE(dd);
  unk = CDD_UNK(dd);
  zero = CDD_ZERO(dd);
  
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
    r = cuddCacheLookup2(dd, Cudd_bddAnd, f, g);
    if (r != NULL) return(r);
  }
  
  // At this point there are still two cases in which a terminal can appear:
  // f \land unk   unk \land g
  // because of the normalization unk will be in as the first argument
  if (f == unk) {
    assert(g != unk);
    // only g contains a non terminal expression
    topg = dd->perm[G->index];
    index = G->index;
    gv = cuddT(G); assert(gv);
    gnv = cuddE(G); assert(gnv);
    if (Cudd_IsComplement(g)) {
      gv = Cudd_cddNot(dd,gv);
      gnv = Cudd_cddNot(dd,gnv);
    }
    fv = fnv = unk;
  } else {
    /* Here we can skip the use of cuddI, because the operands are known
     ** to be non-constant.
     */
    topf = dd->perm[F->index];
    topg = dd->perm[G->index];
    
    /* Compute cofactors. */
    if (topf <= topg) {
      index = F->index;
      fv = cuddT(F);
      fnv = cuddE(F);
      assert(fv);
      assert(fnv);
      if (Cudd_IsComplement(f)) {
        fv = Cudd_cddNot(dd,fv);
        fnv = Cudd_cddNot(dd,fnv);
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
        gv = Cudd_cddNot(dd,gv);
        gnv = Cudd_cddNot(dd,gnv);
      }
    } else {
      gv = gnv = g;
    } 
  }
  
  // Compute the 'then'
  t = cuddCddAndRecur(dd, fv, gv);
  assert(t);
  if (t == NULL)  return(NULL);
  cuddRef(t);
  
  // Compute the 'else'
  e = cuddCddAndRecur(dd, fnv, gnv);
  assert(e);
  if (e == NULL) {
    Cudd_IterDerefBdd(dd, t);
    return(NULL);
  }
  cuddRef(e);
  
  // Reduce the result if possible
  if (t == e) {
    r = t;
  } else {
    if (Cudd_IsComplement(t)) {
      r = cuddUniqueInter(dd,(int)index,
                          Cudd_cddNot(dd,t),
                          Cudd_cddNot(dd,e));
      if (r == NULL) {
        assert(r);
        Cudd_IterDerefBdd(dd, t);
        Cudd_IterDerefBdd(dd, e);
        return(NULL);
      }
      r = Cudd_cddNot(dd,r);
    } else {
      if (t == unk && Cudd_IsComplement(e)) {
        //printf("This is an special case\n");
        r = cuddUniqueInter(dd,(int)index,unk,Cudd_cddNot(dd,e));
        if (r == NULL) {
          assert(r);
          Cudd_IterDerefBdd(dd,e);
          return(NULL);
        }
        r = Cudd_cddNot(dd,r);
      } else {
        r = cuddUniqueInter(dd,(int)index,t,e);
        if (r == NULL) {
          assert(r);
          Cudd_IterDerefBdd(dd, t);
          Cudd_IterDerefBdd(dd, e);
          return(NULL);
        }
      }
    }
  }
  cuddDeref(e);
  cuddDeref(t);
  if (F->ref != 1 || G->ref != 1)
    cuddCacheInsert2(dd, Cudd_bddAnd, f, g, r);
  return(r);
}

int isOne(DdManager *dd, DdNode *f) {
  assert(f);
  DdNode *one, *F;
  one = CDD_ONE(dd);
  F = Cudd_Regular(f); assert(F);
  
  return(F == one && f == one) ;
}

int isZero(DdManager *dd, DdNode *f) {
  assert(f);
  DdNode *one, *zero, *F;
  one = CDD_ONE(dd);
  zero = CDD_ZERO(dd);
  F = Cudd_Regular(f); assert(F);
  
  return(F == one && f == zero) ;
}

int isUnk(DdManager *dd, DdNode *f) {
  assert(f);
  DdNode *unk, *F;
  unk = CDD_UNK(dd);
  F = Cudd_Regular(f); assert(F);
  if (f == unk) {
    assert(F == unk);
    return 1;
  }
  return 0;
}

DdNode* cuddCddMergeRecur(DdManager * dd, DdNode * f, DdNode * g) {
  DdNode *F, *fv, *fnv, *G, *gv, *gnv;
  DdNode *one, *zero, *unk, *r, *t, *e;
  unsigned int topf, topg, index;
  
  statLine(dd);
  one = CDD_ONE(dd);
  unk = CDD_UNK(dd);
  zero = CDD_ZERO(dd);
  
  /* Terminal cases. */
  F = Cudd_Regular(f); assert(F);
  G = Cudd_Regular(g); assert(G);
  
  if (F == G) {
    if (f == g) return(f); // f \merge f = f
    else { // f \merge \lnot f = failure
      dd->errorCode = CDD_FAIL;
      return NULL;
    }
  }
  
  if (isOne(dd,f)) {
    if (isZero(dd,g)) {
      dd->errorCode = CDD_FAIL;
      return NULL;
    } 
    if (isOne(dd,g) || isUnk(dd,g)) {
      return one;
    }
  } else if (isZero(dd,f)) {
    if (isOne(dd,g)) {
      dd->errorCode = CDD_FAIL;
      return NULL;
    } 
    if (isZero(dd,g) || isUnk(dd,g)) {
      return zero;
    }
  }
  
  if (isOne(dd,g)) {
    if (isZero(dd,f)) {
      dd->errorCode = CDD_FAIL;
      return NULL;
    } 
    if (isOne(dd,f) || isUnk(dd,f)) {
      return one;
    }
  } else if (isZero(dd,g)) {
    if (isOne(dd,f)) {
      dd->errorCode = CDD_FAIL;
      return NULL;
    } 
    if (isZero(dd,f) || isUnk(dd,f)) {
      return zero;
    }
  }
  
  if (isUnk(dd,f) && isUnk(dd,g)) {
    return unk;
  }
  
  if (isUnk(dd,f)) return g;
  if (isUnk(dd,g)) return f;
  
  assert(f != unk);
  assert(g != unk);
  
   /* normalize to try to increase cache efficiency. */
  if ( (g == one || g == zero) || f > g) {
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
    r = cuddCacheLookup2(dd, Cudd_cddMerge, f, g);
    if (r != NULL) return(r);
  }
  
  if (f == one || f == zero) {
    // only g contains a non terminal expression
    topg = dd->perm[G->index];
    index = G->index;
    gv = cuddT(G); assert(gv);
    gnv = cuddE(G); assert(gnv);
    if (Cudd_IsComplement(g)) {
      gv = Cudd_cddNot(dd,gv);
      gnv = Cudd_cddNot(dd,gnv);
    }
    fv = fnv = f;
  } else {
    /* Here we can skip the use of cuddI, because the operands are known
     ** to be non-constant.
     */
    topf = dd->perm[F->index];
    topg = dd->perm[G->index];
    
    /* Compute cofactors. */
    if (topf <= topg) {
      index = F->index;
      fv = cuddT(F);
      fnv = cuddE(F);
      assert(fv);
      assert(fnv);
      if (Cudd_IsComplement(f)) {
        fv = Cudd_cddNot(dd,fv);
        fnv = Cudd_cddNot(dd,fnv);
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
        gv = Cudd_cddNot(dd,gv);
        gnv = Cudd_cddNot(dd,gnv);
      }
    } else {
      gv = gnv = g;
    } 
  }
  
  // Compute the 'then'
  t = cuddCddMergeRecur(dd, fv, gv);
  assert(t);
  if (t == NULL)  return(NULL);
  cuddRef(t);
  
  // Compute the 'else'
  e = cuddCddMergeRecur(dd, fnv, gnv);
  assert(e);
  if (e == NULL) {
    Cudd_IterDerefBdd(dd, t);
    return(NULL);
  }
  cuddRef(e);
  
  // Reduce the result if possible
  if (t == e) {
    r = t;
  } else {
    if (Cudd_IsComplement(t)) {
      r = cuddUniqueInter(dd,(int)index,Cudd_cddNot(dd,t),Cudd_cddNot(dd,e));
      if (r == NULL) {
        assert(r);
        Cudd_IterDerefBdd(dd, t);
        Cudd_IterDerefBdd(dd, e);
        return(NULL);
      }
      r = Cudd_cddNot(dd,r);
    } else {
      if (t == unk && Cudd_IsComplement(e)) {
        r = cuddUniqueInter(dd,(int)index,unk,Cudd_cddNot(dd,e));
        if (r == NULL) {
          assert(r);
          Cudd_IterDerefBdd(dd,e);
          return(NULL);
        }
        r = Cudd_cddNot(dd,r);
      } else {
        r = cuddUniqueInter(dd,(int)index,t,e);
        if (r == NULL) {
          assert(r);
          Cudd_IterDerefBdd(dd, t);
          Cudd_IterDerefBdd(dd, e);
          return(NULL);
        }
      }
    }
  }
  cuddDeref(e);
  cuddDeref(t);
  if (F->ref != 1 || G->ref != 1)
    cuddCacheInsert2(dd,Cudd_cddMerge, f, g, r);
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


DdNode* cuddCddStatusRecur(DdManager *dd, DdNode *f, DdNode *g) {
  DdNode *F, *fv, *fnv, *G, *gv, *gnv;
  DdNode *one, *zero, *unk, *r, *t, *e;
  unsigned int topf, topg, index;
  
  statLine(dd);
  one = CDD_ONE(dd);
  unk = CDD_UNK(dd);
  zero = CDD_ZERO(dd);
  
  /* Terminal cases. */
  F = Cudd_Regular(f); assert(F);
  G = Cudd_Regular(g); assert(G);
  
  if (F == G)
    if (f == g) return(one); // f \status f = true

  if (isOne(dd,f)) {
    if (isOne(dd, g)) return one;
    if (isUnk(dd, g) || isZero(dd, g)) return zero;
  }
  
  if (isUnk(dd,f)) {
    if (isUnk(dd, g)) return one;
    if (isOne(dd, g) || isZero(dd, g)) return zero;
  }
  
  if (isZero(dd,f)) {
    if (isZero(dd, g)) return one;
    if (isOne(dd, g) || isUnk(dd, g)) return zero;
  }
  
  assert(f != one && f != unk && f != zero);
  assert(g != one && g != unk && g != zero);
  
  /* normalize to try to increase cache efficiency. */
  if (f > g) {
    DdNode *tmp = f;
    f = g;
    g = tmp;
    assert(f);
    assert(g);
    F = Cudd_Regular(f); assert(F);
    G = Cudd_Regular(g); assert(G);
  }
  
  // at this point therer are no constants
  
  /* Check cache. */
  
  if (F->ref != 1 || G->ref != 1) {
    r = cuddCacheLookup2(dd, Cudd_cddStatus, f, g);
    if (r != NULL) return(r);
  }
  
  /* Here we can skip the use of cuddI, because the operands are known
   ** to be non-constant.
   */
  topf = dd->perm[F->index];
  topg = dd->perm[G->index];
  
  /* Compute cofactors. */
  if (topf <= topg) {
    index = F->index;
    fv = cuddT(F);
    fnv = cuddE(F);
    assert(fv);
    assert(fnv);
    if (Cudd_IsComplement(f)) {
      fv = Cudd_cddNot(dd,fv);
      fnv = Cudd_cddNot(dd,fnv);
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
      gv = Cudd_cddNot(dd,gv);
      gnv = Cudd_cddNot(dd,gnv);
    }
  } else {
    gv = gnv = g;
  } 
  
  // Compute the 'then'
  t = cuddCddStatusRecur(dd, fv, gv);
  assert(t);
  if (t == NULL)  return(NULL);
  cuddRef(t);
  
  // Compute the 'else'
  e = cuddCddStatusRecur(dd, fnv, gnv);
  assert(e);
  if (e == NULL) {
    Cudd_IterDerefBdd(dd, t);
    return(NULL);
  }
  cuddRef(e);
  
  // Reduce the result if possible
  if (t == e) {
    r = t;
  } else {
    if (Cudd_IsComplement(t)) {
      r = cuddUniqueInter(dd,(int)index,Cudd_cddNot(dd,t),Cudd_cddNot(dd,e));
      if (r == NULL) {
        assert(r);
        Cudd_IterDerefBdd(dd, t);
        Cudd_IterDerefBdd(dd, e);
        return(NULL);
      }
      r = Cudd_cddNot(dd,r);
    } else {
      if (t == unk && Cudd_IsComplement(e)) {
        r = cuddUniqueInter(dd,(int)index,unk,Cudd_cddNot(dd,e));
        if (r == NULL) {
          assert(r);
          Cudd_IterDerefBdd(dd,e);
          return(NULL);
        }
        r = Cudd_cddNot(dd,r);
      } else {
        r = cuddUniqueInter(dd,(int)index,t,e);
        if (r == NULL) {
          assert(r);
          Cudd_IterDerefBdd(dd, t);
          Cudd_IterDerefBdd(dd, e);
          return(NULL);
        }
      }
    }
  }
  cuddDeref(e);
  cuddDeref(t);
  if (F->ref != 1 || G->ref != 1)
    cuddCacheInsert2(dd,Cudd_cddStatus, f, g, r);
  assert(r);
  return(r);
}

DdNode* Cudd_cddStatus(DdManager *dd, DdNode *f, DdNode *g) {
  DdNode *res;
  do {
    dd->reordered = 0;
    res = cuddCddStatusRecur(dd,f,g);
  } while (dd->reordered == 1);
  return(res);  
}

