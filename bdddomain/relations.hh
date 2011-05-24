#ifndef __CPREL_BDDDOMAIN_RELATIONS_HH__
#define __CPREL_BDDDOMAIN_RELATIONS_HH__

#include <bdddomain/encoding.hh>

namespace MPG { namespace CPRel { namespace VarImpl {

/**
 * \brief Creates the binary relation \f$R = \{t : t.1 = t.2 \} \f$
 * \ingroup DomRepr
 */
DdNode* create_equalXY(void);
/**
 * \brief Creates the binary relation \f$R = \{t : t.1 > t.2 \} \f$
 * \ingroup DomRepr
 */
DdNode* create_greaterXY(void);

}}}

#endif
