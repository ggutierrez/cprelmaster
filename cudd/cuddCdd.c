#include "cuddCdd.h"


/* Inline function definition */
inline
DdNode* Cudd_cddNot(DdManager *dd, DdNode *f) {
  if (f == CDD_UNK(dd)) return f;
  return Cudd_Not(f);
}

inline
DdNode* Cudd_cddNotCond(DdManager *dd, DdNode *f, int c) {
  return c ? Cudd_cddNot(dd,f) : f;
}

int isOne(DdManager *dd, DdNode *f) {
  assert(f);
  DdNode * F = Cudd_Regular(f); assert(F);
  return(F == CDD_ONE(dd) && f == CDD_ONE(dd));
}

int isZero(DdManager *dd, DdNode *f) {
  assert(f);
  DdNode *F = Cudd_Regular(f); assert(F);
  return(F == CDD_ONE(dd) && f == CDD_ZERO(dd));
}

int isUnk(DdManager *dd, DdNode *f) {
  assert(f);
  DdNode *F = Cudd_Regular(f); assert(F);
  if (f == CDD_UNK(dd)) {
    assert(F == CDD_UNK(dd));
    return 1;
  }
  return 0;
}

DdNode * cuddCddDeltaRecur(DdManager *dd, DdNode *f) {
  DdNode  *one, *zero, *unk;
  DdNode *F, *fv, *fnv, *t, *e, *r;
  unsigned int topf, index;
  
  statLine(dd);
  one = CDD_ONE(dd);
  unk = CDD_UNK(dd);
  zero = CDD_ZERO(dd);
  
  F = Cudd_Regular(f); assert(F);
  
  /* Terminal cases */
  if (f == one || f == zero) // delta(one) = one
    return one;
  if (f == unk) { // delta(unk) = unk
    assert(F == unk);
    return unk;
  }

  /* Check cache. */  
  if (F->ref != 1) {
    r = cuddCacheLookup1(dd, Cudd_cddDelta, f);
    if (r != NULL) return(r);
  }
  
  /* A this point f is known to be non constant */
  assert(f != one && f != zero  && f != unk);
  topf = dd->perm[F->index];
  index = F->index;
  fv = cuddT(F); assert(fv);
  fnv = cuddE(F); assert(fnv);
  if (Cudd_IsComplement(f)) {
    fv = Cudd_cddNot(dd, fv);
    fnv = Cudd_cddNot(dd, fnv);
  }
  
  //
  // Compute the 'then'
  t = cuddCddDeltaRecur(dd,fv);
  assert(t);
  if (t == NULL)  return(NULL);
  cuddRef(t);
  
  // Compute the 'else'
  e = cuddCddDeltaRecur(dd,fnv);
  assert(e);
  if (e == NULL) {
    Cudd_IterDerefBdd(dd, t);
    return(NULL);
  }
  cuddRef(e);
  
  // assert that the result of both recursive call must not contain complemented edges.
  assert(!Cudd_IsComplement(t) && !Cudd_IsComplement(e));
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
  if (F->ref != 1)
    cuddCacheInsert1(dd, Cudd_cddDelta, f, r);
  return(r);
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

DdNode * cuddCddXorRecur(DdManager *dd, DdNode *f, DdNode *g) {
  
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
  if (t == NULL && dd->errorCode == CDD_FAIL) return NULL;
  assert(t);
  if (t == NULL)  return(NULL);
  cuddRef(t);
  
  // Compute the 'else'
  e = cuddCddMergeRecur(dd, fnv, gnv);
  if (e == NULL && dd->errorCode == CDD_FAIL) return NULL;
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
  
  
  
  /* normalize to try to increase cache efficiency. */
  if (f > g || g == one || g == unk || g == zero) {
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
  
  if (f == one || f == zero || f == unk) {
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
    assert(f != one && f != unk && f != zero);
    
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


DdNode* Cudd_cddAnd(DdManager * dd, DdNode * f, DdNode * g) {
  DdNode *res;
  do {
    dd->reordered = 0;
    res = cuddCddAndRecur(dd,f,g);
  } while (dd->reordered == 1);
  return(res);
}

DdNode* Cudd_cddDelta(DdManager * dd, DdNode * f) {
  DdNode *res;
  do {
    dd->reordered = 0;
    res = cuddCddDeltaRecur(dd,f);
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

DdNode* Cudd_cddStatus(DdManager *dd, DdNode *f, DdNode *g) {
  DdNode *res;
  do {
    dd->reordered = 0;
    res = cuddCddStatusRecur(dd,f,g);
  } while (dd->reordered == 1);
  return(res);  
}

// gg: I had to copy and paste this function because it is not visible from other
//     modules in cudd.
static int
bddCheckPositiveCube(DdManager * manager, DdNode * cube) {
  if (Cudd_IsComplement(cube)) return(0);
  if (cube == DD_ONE(manager)) return(1);
  if (cuddIsConstant(cube)) return(0);
  if (cuddE(cube) == Cudd_Not(DD_ONE(manager))) {
    return(bddCheckPositiveCube(manager, cuddT(cube)));
  }
  return(0);
}

DdNode* CuddCddExistAbstractRecur(DdManager *dd, DdNode *f, DdNode *cube) {
  DdNode *one, *unk, *F, *T, *E, *res1, *res2, *res;
  statLine(dd);
  
  one = CDD_ONE(dd);
  unk = CDD_UNK(dd);
  F = Cudd_Regular(f);
  
  if (cube == one || F == one) {
    // cube == one: to quantify on no variable, in this case f is unchanged
    // F == one: there are 2 possibilities:
    //   - f == F == one   : there are no variables to quant. the result is f 
    //   - f != F == one   : there are no variables to quant. the result is f
    return f;
  }
  
  if (F == unk) {
    // The quantification of the unk is the same unk. This is because an unk can
    // become after one or zero so is not possible to decide.
    assert(f == unk);
    return f;
  }
  
  assert(f != one && f != CDD_ZERO(dd) && f != unk);
  
  /* Abstract a variable that does not appear in f. */
  while (dd->perm[F->index] > dd->perm[cube->index]) {
    cube = cuddT(cube);
    if (cube == one) return(f);
  }
  
  /* Check the cache. */
  if (F->ref != 1 &&
      (res = cuddCacheLookup2(dd, Cudd_cddExistAbstract, f, cube)) != NULL) {
    return(res);
  }
  
  /* Compute the cofactors of f. */
  T = cuddT(F); E = cuddE(F);
  if (f != F) {
    T = Cudd_cddNot(dd,T); E = Cudd_cddNot(dd,E);
  }
  
  // At this point there are two possibilities with respect to the two top 
  // variables in F and in the cube. If the top of the cube is not part of F then
  // it does not make any sense to abstract it, it is removed from the cube. This
  // procedure is applied until either:
  //   1 reaching a variable in the cube that is the top of F
  //   2 reaching a variable in the cube that is below the top of F
  if (F->index == cube->index) { // 1
    if (T == one || E == one || T == Cudd_cddNot(dd,E)) {
	    return(one);
    }
    res1 = CuddCddExistAbstractRecur(dd, T, cuddT(cube));
    if (res1 == NULL) return(NULL);
    if (res1 == one) {
	    if (F->ref != 1)
        cuddCacheInsert2(dd, Cudd_cddExistAbstract, f, cube, one);
	    return(one);
    }
    cuddRef(res1);
    res2 = CuddCddExistAbstractRecur(dd, E, cuddT(cube));
    if (res2 == NULL) {
	    Cudd_IterDerefBdd(dd,res1);
	    return(NULL);
    }
    cuddRef(res2);
    res = cuddCddAndRecur(dd, Cudd_cddNot(dd,res1), Cudd_cddNot(dd,res2));
    if (res == NULL) {
	    Cudd_IterDerefBdd(dd, res1);
	    Cudd_IterDerefBdd(dd, res2);
	    return(NULL);
    }
    res = Cudd_cddNot(dd,res);
    cuddRef(res);
    Cudd_IterDerefBdd(dd, res1);
    Cudd_IterDerefBdd(dd, res2);
    if (F->ref != 1)
	    cuddCacheInsert2(dd, Cudd_cddExistAbstract, f, cube, res);
    cuddDeref(res);
    return(res);
  } else { // 2
    res1 = CuddCddExistAbstractRecur(dd, T, cube);
    if (res1 == NULL) return(NULL);
    cuddRef(res1);
    res2 = CuddCddExistAbstractRecur(dd, E, cube);
    if (res2 == NULL) {
	    Cudd_IterDerefBdd(dd, res1);
	    return(NULL);
    }
    cuddRef(res2);
    
    if (res1 == res2) {
      res = res1;
    } else {
      if (Cudd_IsComplement(res1)) {
        res = cuddUniqueInter(dd,(int)F->index,
                              Cudd_cddNot(dd,res1),
                              Cudd_cddNot(dd,res2));
        if (res == NULL) {
          assert(res);
          Cudd_IterDerefBdd(dd, res1);
          Cudd_IterDerefBdd(dd, res2);
          return(NULL);
        }
        res = Cudd_cddNot(dd,res);
      } else {
        if (res1 == unk && Cudd_IsComplement(res2)) {
          //printf("This is an special case\n");
          res = cuddUniqueInter(dd,(int)F->index,unk,Cudd_cddNot(dd,res2));
          if (res == NULL) {
            assert(res);
            Cudd_IterDerefBdd(dd,res2);
            return(NULL);
          }
          res = Cudd_cddNot(dd,res);
        } else {
          res = cuddUniqueInter(dd,(int)F->index,res1,res2);
          if (res == NULL) {
            assert(res);
            Cudd_IterDerefBdd(dd, res1);
            Cudd_IterDerefBdd(dd, res2);
            return(NULL);
          }
        }
      }
    }
    //
    cuddDeref(res1);
    cuddDeref(res2);
    if (F->ref != 1)
	    cuddCacheInsert2(dd, Cudd_cddExistAbstract, f, cube, res);
    return(res);
  }
}

DdNode* Cudd_cddExistAbstract(DdManager *dd, DdNode *f, DdNode *cube) {
  DdNode *res;
  
  // cube must represent all the variables in their positive form
  if (bddCheckPositiveCube(dd, cube) == 0) {
    dd->errorCode = CUDD_INVALID_ARG;
    return NULL;
  }
  
  do {
    dd->reordered = 0;
    res = CuddCddExistAbstractRecur(dd,f,cube);
  } while (dd->reordered == 1);
  
  return res;
}