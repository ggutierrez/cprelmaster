#ifndef __CPREL_BDDDOMAIN_RELATIONS_HH__
#define __CPREL_BDDDOMAIN_RELATIONS_HH__

#include <bdddomain/rel-impl.hh>

namespace MPG { namespace VarImpl {

/**
 * \brief Creates the binary relation \f$R = \{t : t.1 = t.2 \} \f$
 * \ingroup DomRepr
 */
RelationImpl create_equalXY(void);
/**
 * \brief Creates the binary relation \f$R = \{t : t.1 > t.2 \} \f$
 * \ingroup DomRepr
 */
RelationImpl create_greaterXY(void);
}}

#endif
