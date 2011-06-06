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
 *
 * The description is a vector of pairs. The first component of every
 * pair defines the initial column and the other component defines the
 * new possition for that column.
 *
 * \warning No checking is performed on \a swapDesc but the following
 * is assumed:
 *
 * - It specifies valid columns in the relation \a r
 * - It does not contain the same column in defferent pairs.
 */
 DdNode* swap_columns(DdNode *r, const PermDescriptor& swapDesc);
}}}
#endif
