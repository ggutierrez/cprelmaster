#ifndef __CPREL_CPREL_VAR_HH__
#define __CPREL_CPREL_VAR_HH__

#include <gecode/kernel.hh>
#include <cprel/grelation.hh>
#include <cprel/varimp.hh>

/**
 * \defgroup CPRelVars Relation variables
 *
 * This module defines the interface to model constraint problems using relation
 * variables.
 */

/**
 * \defgroup CPRelViews Relation views
 *
 * This module defines the interface to implement propagators and brancher on
 * relation variables.
 */

/**
 * \defgroup SetProp Set propagators on relations
 *
 * A relation can be seen as a set of tuples. From this point of view different
 * constraints can be defined. These propagators are presented in this module.
 */

/**
 * \defgroup RelBranch Branchers (distribution strategies) on relations
 *
 * This module contains the branchers that can be used on relation domains.
 */

namespace MPG {
/**
 * \brief Relation variables.
 * \ingroup CPRelVars
 *
 * This class provides the way to create and access information of relation
 * variables.
 */
class CPRelVar : public Gecode::VarImpVar<CPRel::CPRelVarImp> {
protected:
  /// Variable implementation
  using Gecode::VarImpVar<CPRel::CPRelVarImp>::x;
public:
  /// \name Constructors
  //@{
  /// Default constructor
  CPRelVar(void) {}
  /// Copy constructor
  CPRelVar(const CPRelVar& y)
    : Gecode::VarImpVar<CPRel::CPRelVarImp>(y.varimp()) {}
  /// Constructor from a variable implementation \a y
  CPRelVar(CPRel::CPRelVarImp *y)
    : Gecode::VarImpVar<CPRel::CPRelVarImp>(y) {}
  //@}
  /// \name Variable creation
  //@{
  /**
   * \brief Creates a relation variable with \a l as the lower bound relation
   * and \a u as the upper bound.
   *
   * \warning Throws a CPRel::VariableEmptyDomain if the provided bounds describe
   * an empty domain.
   */
  CPRelVar(Space& home, const CPRel::GRelation& l, const CPRel::GRelation& u)
    : Gecode::VarImpVar<CPRel::CPRelVarImp>
      (new (home) CPRel::CPRelVarImp(home,l,u)) {
    if (!l.subsetEq(u))
      throw CPRel::VariableEmptyDomain("CPRelVar::CPRelVar");
  }
  //@}
  /// \name Relation information
  //@{
  /// Returns the arity
  int arity(void) const {
    return x->arity();
  }
  //@}
  /// \name Domain ifnormation
  //@{
  /**
   * \brief Returns a relation representing the greatest lower bound of the
   * variable.
   *
   * This relation contains the tuples that are known to be part of the variable.
   */
  CPRel::GRelation glb(void) const {
    return x->glb();
  }
  /**
   * \brief Returns a relation representing the least upper bound of the
   * variable.
   *
   * This relation contains the tuples that are possible part of the variable.
   */
  CPRel::GRelation lub(void) const {
    return x->lub();
  }
  /**
   * \brief Unknown access.
   *
   * Returns a relation (copy) with the maximum relation that can be included in
   * the lower bound. \f$unk = lub \setminus glb \f$
   */
  CPRel::GRelation unk(void) const {
    return x->unk();
  }
  /**
   * \brief Out of bound access.
   *
   * Returns a relation (copy) that is known to not be part of the set of relations
   * represented by the variable. \f$oob = O \setminus lub \f$, wher \f$ O \f$ is
   * the full relation.
   */
  CPRel::GRelation oob(void) const {
    return x->oob();
  }
  //@}
};

/**
 * \brief Output relation variable \a R to \a os
 * \ingroup CPRelVars
 *
 * \a R is printed as three different groups: the lower bound (\a glb), the unknown
 * Which is equivalent to \f$ lub \setminus glb \f$ and the out of bound (\f$\mathcal{U}_{k}\setminus lub \f$
 * ), where \f$\mathcal{U}_{k}\f$ is the universe containing all tuples of arity
 * \f$k\f$.
 */
template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const CPRelVar& R) {
  std::basic_ostringstream<Char,Traits> s;
  s.copyfmt(os); s.width(0);

  using namespace CPRel;
  if (R.assigned()) s << "val:{";
  else s << "glb:{";

  GRelation glb(R.glb());
  s  << glb << "}#" << glb.cardinality();

  if(!R.assigned()) {
    GRelation unk(R.unk());
    s << ", unk:{" << unk << "}#" << unk.cardinality();
    /// The oob
    GRelation oob(R.oob());
    s << ", oob:{...}#" << oob.cardinality();
  }
  return os << s.str();
}

}

#endif
