#ifndef __CPREL_BDDDOMAIN_ENCODING_HH__
#define __CPREL_BDDDOMAIN_ENCODING_HH__

#include <vector>
#include <cprel/tuple.hh>
#include <bdddomain/manager.hh>

namespace MPG { namespace CPRel { namespace VarImpl {
/**
 * \defgroup BDDEnc Ground relation encoding
 *
 *
 */

/**
 * \brief Returns the variables (indices) used in the bdd to encode column \a c
 * \ingroup BDDEnc
 */
std::vector<int> bddIndices(int c);

/**
 * \brief Returns the variables used in the bdd to encode column \a c
 * \ingroup BDDEnc
 */
std::vector<DdNode*> bddVars(int c);

DdNode* swap_columns(DdNode *r, int x, int y);
/**
 * \brief Returns a BDD representation for the encoding of \a p in column \a a
 * \ingroup BDDEnc
 */
DdNode* encode(int p, int a);
/**
 * \brief Returns a BDD representing \a tuple.
 * \ingroup BDDEnc
 */
DdNode* encode(const Tuple& tuple);

}}}
#endif
