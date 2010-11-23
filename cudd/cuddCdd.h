#ifndef __CUDD_CDD_H_
#define __CUDD_CDD_H_

#include "util.h"
#include "cuddInt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CDD_ONE(dd)  DD_ONE((dd))
#define CDD_UNK(dd)  DD_ZERO((dd))
#define CDD_ZERO(dd) Cudd_Not(CDD_ONE((dd)))
  
  // Computes the negation of the CDD represented by \a f
  DdNode* Cudd_cddNot(DdManager *dd, DdNode *f);
  // Compute the complement of \a f if \a c is not zero
  DdNode* Cudd_cddNotCond(DdManager *dd, DdNode *f, int c);
  /// Compute the conjunction of two CDDs \a f and \a g
  DdNode* Cudd_cddAnd(DdManager * dd, DdNode * f, DdNode * g);
  /// Compute the disjuntion of two CDDs \a f and \a g
  DdNode * Cudd_cddOr(DdManager *dd, DdNode *f, DdNode *g);
  // Merge two Cdds \a f and \a g
  DdNode* Cudd_cddMerge(DdManager *dd, DdNode *f, DdNode *g);
  // Returns a relation representing the agreement between \a f and \a g
  DdNode* Cudd_cddStatus(DdManager *dd, DdNode *f, DdNode *g);
  // Returns the relation resulting by quantifying the variables represented in
  // \a cube in \a f
  DdNode* Cudd_cddExistAbstract(DdManager *dd, DdNode *f, DdNode *cube);
  
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
