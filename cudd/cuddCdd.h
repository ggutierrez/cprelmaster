#ifndef __CUDD_CDD_H_
#define __CUDD_CDD_H_

#include "util.h"
#include "cuddInt.h"

// Computes the negation of the CDD represented by \a f
DdNode* Cudd_cddNot(DdManager *manager, DdNode *f);
// Compute the complement of \a f if \a c is not zero
DdNode* Cudd_cddNotCond(DdManager *manager, DdNode *f, int c);
/// Compute the conjunction of two CDDs \a f and \a g
DdNode* Cudd_cddAnd(DdManager * dd, DdNode * f, DdNode * g);
/// Compute the disjuntion of two CDDs \a f and \a g
DdNode * Cudd_cddOr(DdManager *dd, DdNode *f, DdNode *g);
// Merge two Cdds \a f and \a g
DdNode* Cudd_cddMerge(DdManager *dd, DdNode *f, DdNode *g);

#endif
