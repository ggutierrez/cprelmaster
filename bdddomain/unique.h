#ifndef __CPREL_BDDDOMAIN_UNIQUE_H__
#define __CPREL_BDDDOMAIN_UNIQUE_H__

#include <util/util.h>
#include <cudd/cuddInt.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Uniqly quantify in \a f all the variables in \a cube.
 *
 * @param manager is the bdd manager
 * @param f is the bdd representing the function to quantify
 * @param cube contains all the variables that will be quantified
 * @return a bdd representing the function resulting from the unique quantification
 * of the variables in \a cube in \a f.
 */
DdNode* Cudd_bddUniqueAbstract(DdManager* manager, DdNode* f, DdNode* cube);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif
