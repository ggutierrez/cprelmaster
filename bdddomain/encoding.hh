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

/// Returns the variables (indices) used in the bdd to encode column \a c
std::vector<int> bddVars(int c);

DdNode* swap_columns(DdNode *r, int x, int y);

DdNode* encode(int p, int a);

DdNode* encode(const Tuple& tuple);

}}}
#endif
