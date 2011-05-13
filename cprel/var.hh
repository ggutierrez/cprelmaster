#ifndef __CPREL_CPREL_VAR_HH__
#define __CPREL_CPREL_VAR_HH__

#include <gecode/kernel.hh>
#include <cprel/grelation.hh>
#include <cprel/varimp.hh>

using Gecode::VarImpVar;

// variable
namespace MPG {
/// Relation variables
class CPRelVar : public VarImpVar<CPRel::CPRelVarImp> {
protected:
  using VarImpVar<CPRel::CPRelVarImp>::x;
public:
  CPRelVar(void) {}
  CPRelVar(const CPRelVar& y)
    : VarImpVar<CPRel::CPRelVarImp>(y.varimp()) {}
  CPRelVar(CPRel::CPRelVarImp *y)
    : VarImpVar<CPRel::CPRelVarImp>(y) {}

  // variable creation
  CPRelVar(Space& home, const CPRel::GRelation& l, const CPRel::GRelation& u)
    : VarImpVar<CPRel::CPRelVarImp>
      (new (home) CPRel::CPRelVarImp(home,l,u)) {
    if (!l.subsetEq(u))
      throw CPRel::VariableEmptyDomain("CPRelVar::CPRelVar");
  }
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

template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const CPRelVar& x) {
  std::basic_ostringstream<Char,Traits> s;
  s.copyfmt(os); s.width(0);

  using namespace CPRel;
  if (x.assigned()) s << "val:{";
  else s << "glb:{";

  GRelation glb(x.glb());
  s  << glb << "}#" << glb.cardinality();

  if(!x.assigned()) {
    GRelation unk(x.unk());
    s << ", unk:{" << unk << "}#" << unk.cardinality();
    /// The oob
    GRelation oob(x.oob());
    s << ", oob:{...}#" << oob.cardinality();
  }
  return os << s.str();
}

}



#endif
