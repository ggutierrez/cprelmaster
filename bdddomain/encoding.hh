#ifndef __CPREL_BDDDOMAIN_ENCODING_HH__
#define __CPREL_BDDDOMAIN_ENCODING_HH__

#include <vector>
#include <bdddomain/manager.hh>
#include <cprel/descriptor.hh>

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

/**
 * \brief Returns the relation resulting from swapping \a r according to the
 * swapping description \a swapDesc.
 * *
 * \warning The content of the description is the responsability of the caller.
 */
 DdNode* swap_columns(DdNode *r, const PermDescriptor& swapDesc);

 DdNode* exists(DdNode* r, int c);
}}}
#endif
