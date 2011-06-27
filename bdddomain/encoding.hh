#ifndef __CPREL_BDDDOMAIN_ENCODING_HH__
#define __CPREL_BDDDOMAIN_ENCODING_HH__

#include <vector>
#include <bdddomain/manager.hh>
#include <bdddomain/perm-descriptor.hh>

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
 * \brief Decodes \a cube, extracted from the BDD and returns the tuple it
 * represents.
 * \ingroup BDDEnc
 */
std::vector<int> decodeCube(int* cube, int arity);

/**
 * \brief Returns the relation resulting from swapping \a r according to the
 * swapping description \a swapDesc.
 * \ingroup BDDEnc
 * \warning The content of the description is the responsability of the caller.
 */
 DdNode* swap_columns(DdNode *r, const PermDescriptor& swapDesc);

 /**
  * \brief Existensial quantification of column \a c on relation \a r.
  * \ingroup BDDEnc
  */
 DdNode* exists(int c, DdNode* r);

 /**
  * \brief Universal quantification of column \a c on relation \a r.
  * \ingroup BDDEnc
  */
 DdNode* forall(int c, DdNode* r);

 /**
  * \brief Unique quantification of column \a c on relation \a r.
  * \ingroup BDDEnc
  */
 DdNode* unique(int c, DdNode* r);

  /**
   * \brief Unique quantification on the columns in \a c on relation \a r.
   * \ingroup BDDEnc
   */
  DdNode* unique(const std::vector<int>& c, DdNode* r);

}}}
#endif
