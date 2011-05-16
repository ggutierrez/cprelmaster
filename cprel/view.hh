#ifndef __CPREL_CPREL_VIEW_HH__
#define __CPREL_CPREL_VIEW_HH__

#include <gecode/kernel.hh>
#include <cprel/var.hh>

namespace MPG { namespace CPRel {
/// View for relation variables
class CPRelView : public Gecode::VarImpView<CPRelVar> {
protected:
  /// Variable
  using VarImpView<CPRelVar>::x;
public:
  /// \name Constructors
  //@{
  /// Default constructor
  CPRelView(void) {}
  /// Constructor from a variable
  CPRelView(const CPRelVar& y)
    : Gecode::VarImpView<CPRelVar>(y.varimp()) {}
  /// Constructor from a variable implementation
  CPRelView(CPRelVarImp* y)
    : Gecode::VarImpView<CPRelVar>(y) {}
  //@}
  /// \name Access operations
  //@{
  /**
   * \brief Returns a relation representing the greatest lower bound of the
   * variable.
   *
   * This relation contains the tuples that are known to be part of the variable.
   */
  GRelation glb(void) const {
    return x->glb();
  }
  /**
   * \brief Returns a relation representing the least upper bound of the
   * variable.
   *
   * This relation contains the tuples that are possible part of the variable.
   */
  GRelation lub(void) const {
    return x->lub();
  }
  /**
   * \brief Unknown access.
   *
   * Returns a relation (copy) with the maximum relation that can be included in
   * the lower bound. \f$unk = lub \setminus glb \f$
   */
  GRelation unk(void) const {
    return x->unk();
  }
  /**
   * \brief Out of bound access.
   *
   * Returns a relation (copy) that is known to not be part of the set of relations
   * represented by the variable. \f$oob = O \setminus lub \f$, wher \f$ O \f$ is
   * the full relation.
   */
  GRelation oob(void) const {
    return x->oob();
  }
  //@}
  /// \name Modification operations
  //@{
  /**
   * \brief Prune the variable by doing: \f$ glb = glb \cup r \f$
   *
   */
  ModEvent include(Space& home, const GRelation& r) {
    return x->include(home,r);
  }
  /**
   * \brief Prune the variable by doing: \f$ lub = lub \setminus r \f$
   *
   */
  ModEvent exclude(Space& home, const GRelation& r) {
    return x->exclude(home,r);
  }
  //@}
  // delta information
  int min(const Delta& d) const {
    return CPRelVarImp::min(d);
  }
  int max(const Delta& d) const {
    return CPRelVarImp::max(d);
  }
};

/**
 * \brief Output relation view \a R to \a os
 * \ingroup CPRelViews
 *
 * \a R is printed as three different groups: the lower bound (\a glb), the unknown
 * Which is equivalent to \f$ lub \setminus glb \f$ and the out of bound (\f$\mathcal{U}_{k}\setminus lub \f$
 * ), where \f$\mathcal{U}_{k}\f$ is the universe containing all tuples of arity
 * \f$k\f$.
 */
template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator<<(std::basic_ostream<Char,Traits>& os, const CPRelView& R) {

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

}}

// constant integer view
namespace MPG { namespace CPRel {
/// Constant view on a relation variable
class ConstCPRelView : public Gecode::ConstView<CPRelView> {
protected:
  int x;
public:
  ConstCPRelView(void) : x(0) {}
  ConstCPRelView(int n) : x(n) {}

  int min(void) const {
    return x;
  }
  int max(void) const {
    return x;
  }
  ModEvent lq(Space&, int n) {
    return (x <= n) ? ME_CPREL_NONE : ME_CPREL_FAILED;
  }
  ModEvent gq(Space&, int n) {
    return (x >= n) ? ME_CPREL_NONE : ME_CPREL_FAILED;
  }
  // delta information
  int min(const Delta&) const {
    GECODE_NEVER; return 0;
  }
  int max(const Delta&) const {
    GECODE_NEVER; return 0;
  }
  /// Update during cloning
  void update(Space& home, bool share, ConstCPRelView& y) {
    ConstView<CPRelView>::update(home,share,y);
    x = y.x;
  }
};
// view tests
inline bool same(const ConstCPRelView& x, const ConstCPRelView& y) {
  return x.min() == y.min();
}
inline bool before(const ConstCPRelView& x, const ConstCPRelView& y) {
  return x.min() < y.min();
}

template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const ConstCPRelView& x) {
  return os << x.min();
}

}

void branch(Gecode::Home home, CPRelVar x);
}



#endif
