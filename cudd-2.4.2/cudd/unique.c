#include <cudd/unique.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Checks whether cube is an BDD representing the product of
 * positive literals.
 *
 * \warning This function is a copy and paste from CUDD sources. It is here because
 * it is declared static inside cudd/cuddBddAbs.c
 */
static int bddCheckPositiveCube(DdManager* manager, DdNode* cube) {
  if (Cudd_IsComplement(cube)) return(0);
  if (cube == DD_ONE(manager)) return(1);
  if (cuddIsConstant(cube)) return(0);
  if (cuddE(cube) == Cudd_Not(DD_ONE(manager))) {
    return(bddCheckPositiveCube(manager, cuddT(cube)));
  }
  return(0);

}

/**
 * \brief Performs the recursive steps of Cudd_bddUniqueAbstract.
 *
 * Performs the recursive steps of Cudd_bddExistAbstract. Returns the BDD
 * obtained by abstracting the variables of cube from f if successful; NULL
 * otherwise.
 *
 * \warning This function is based on the Cudd_bddExistAbstractReucr present in
 * the CUDD implementation at cudd/cuddBddAbs.c
 */
DdNode* cuddBddUniqueAbstractRecur(DdManager* manager, DdNode* f, DdNode* cube) {
  DdNode	*F, *T, *E, *res, *res1, *res2, *one, *zero;

  statLine(manager);
  one = DD_ONE(manager);
  zero = Cudd_Not(one);
  F = Cudd_Regular(f);

  /* Cube is guaranteed to be a cube at this point. */
  
  /* base case */
  if (F == one || cube == one) {
    return(f);
  }
  
  /* From now on, f and cube are non-constant. */
  
  /* abstracting a variable that does not exist */
  if (manager->perm[F->index] > manager->perm[cube->index])
    return zero;
  
  /* Check the cache. */
  if (F->ref != 1 &&
      (res = cuddCacheLookup2(manager, Cudd_bddUniqueAbstract, f, cube)) != NULL) {
    return(res);
  }

  /* Compute the cofactors of f. */
  T = cuddT(F); E = cuddE(F);
  if (f != F) {
    T = Cudd_Not(T); E = Cudd_Not(E);
  }

  /* If the two indices are the same, so are their levels. */
  if (F->index == cube->index) {
    res1 = cuddBddUniqueAbstractRecur(manager, T, cuddT(cube));
    if (res1 == NULL) return(NULL);
    cuddRef(res1);
    res2 = cuddBddUniqueAbstractRecur(manager, E, cuddT(cube));
    if (res2 == NULL) {
      Cudd_IterDerefBdd(manager,res1);
      return(NULL);
    }
    cuddRef(res2);
    res = cuddBddXorRecur(manager, res1, res2);
    if (res == NULL) {
      Cudd_IterDerefBdd(manager, res1);
      Cudd_IterDerefBdd(manager, res2);
      return(NULL);
    }
    cuddRef(res);
    Cudd_IterDerefBdd(manager, res1);
    Cudd_IterDerefBdd(manager, res2);
    if (F->ref != 1)
      cuddCacheInsert2(manager, Cudd_bddUniqueAbstract, f, cube, res);
    cuddDeref(res);
    return(res);
  } else { /* if (cuddI(manager,F->index) < cuddI(manager,cube->index)) */
    res1 = cuddBddUniqueAbstractRecur(manager, T, cube);
    if (res1 == NULL) return(NULL);
    cuddRef(res1);
    res2 = cuddBddUniqueAbstractRecur(manager, E, cube);
    if (res2 == NULL) {
      Cudd_IterDerefBdd(manager, res1);
      return(NULL);
    }
    cuddRef(res2);
    /* ITE takes care of possible complementation of res1 and of the
     ** case in which res1 == res2. */
    res = cuddBddIteRecur(manager, manager->vars[F->index], res1, res2);
    if (res == NULL) {
      Cudd_IterDerefBdd(manager, res1);
      Cudd_IterDerefBdd(manager, res2);
      return(NULL);
    }
    cuddDeref(res1);
    cuddDeref(res2);
    if (F->ref != 1)
      cuddCacheInsert2(manager, Cudd_bddUniqueAbstract, f, cube, res);
    return(res);
  }

} /* end of cuddBddExistAbstractRecur */


DdNode* Cudd_bddUniqueAbstract(DdManager* manager, DdNode* f, DdNode* cube) {
  DdNode *res;

  if (bddCheckPositiveCube(manager, cube) == 0) {
    (void) fprintf(manager->err,
                   "Error: Can only abstract positive cubes\n");
    manager->errorCode = CUDD_INVALID_ARG;
    return(NULL);
  }

  do {
    manager->reordered = 0;
    res = cuddBddUniqueAbstractRecur(manager, f, cube);
  } while (manager->reordered == 1);

  return(res);

}

#ifdef __cplusplus
} /* end of extern "C" */
#endif
