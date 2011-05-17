#ifndef __CPREL_CPREL_CONST_VIEW_HH__
#define __CPREL_CPREL_CONST_VIEW_HH__

#include <gecode/kernel.hh>
#include <cprel/cprel.hh>
#include <cprel/grelation.hh>

namespace MPG { namespace CPRel {

/// Constant view on a relation variable
class ConstCPRelView : public Gecode::ConstView<CPRelView> {
protected:
  /// Constant relation
  GRelation r_;
  /// Avoid default constructor
  ConstCPRelView(void);
public:
  /// \name Constructors
  //@{
  /// Constuctor from a ground relation
  ConstCPRelView(const GRelation& r) : r_(r) {}
  //@}
  /// \name Access operations
  //@{
  /**
   * \brief Returns the wrapped relation as lower bound.
   */
  GRelation glb(void) const {
    return r_;
  }
  /**
   * \brief Returns the wrapped relation as upper bound.
   */
  GRelation lub(void) const {
    return r_;
  }
  /**
   * \brief Unknown access.
   */
  GRelation unk(void) const {
    return GRelation(r_.arity());
  }
  /**
   * \brief Out of bound access.
   */
  GRelation oob(void) const {
    return r_.complement();
  }
  //@}
  /// \name Modification operations
  //@{
  /**
   * \brief Prune the variable by doing: \f$ glb = glb \cup r \f$
   *
   */
  ModEvent include(Space&, const GRelation& r) {
    return r_.subsetEq(r) ? ME_CPREL_NONE : ME_CPREL_FAILED;
  }
  /**
   * \brief Prune the variable by doing: \f$ lub = lub \setminus r \f$
   *
   */
  ModEvent exclude(Space&, const GRelation& r) {
    return r_.disjoint(r) ? ME_CPREL_NONE : ME_CPREL_FAILED;
  }
  //@}
  // delta information
  int min(const Delta&) const {
    GECODE_NEVER; return 0;
  }
  int max(const Delta&) const {
    GECODE_NEVER; return 0;
  }
  /// Update during cloning
  void update(Space& home, bool share, ConstCPRelView& y) {
    Gecode::ConstView<CPRelView>::update(home,share,y);
    r_ = y.r_;
  }
};
// view tests
inline bool same(const ConstCPRelView& x, const ConstCPRelView& y) {
  return x.glb().eq(y.glb());
}
inline bool before(const ConstCPRelView& x, const ConstCPRelView& y) {
  return y.glb().superset(x.glb());
}

template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const ConstCPRelView& x) {
  return os << x.glb();
}
}}

#endif
