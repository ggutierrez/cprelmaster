#ifndef __CPREL_CPREL_CONST_VIEW_HH__
#define __CPREL_CPREL_CONST_VIEW_HH__

#include <gecode/kernel.hh>
#include <cprel/cprel.hh>
#include <bdddomain/grelation.hh>

namespace MPG { namespace CPRel {

/**
 * \brief Constant view on a relation variable
 * \ingroup CPRelViews
 *
 * A constant view \f$C\f$ for a ground relation \f$G\f$ provides operations
 * such that \f$C\f$ behaves like the relation view with domain \f$[G,G]\f$.
 *
 * - To create a full view of arity n:
 *   \code
 *   GRelation ur = create_full(n);
 *   ConstView u(ur);
 *   \endcode
 * - To create an empty view of arity n:
 *   \code
 *   GRelation er(n);
 *   ConstView e(er);
 *   \endcode
 */
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
  /// Copy constructor
  ConstCPRelView(const ConstCPRelView& v) : r_(v.r_) {}
  //@}
  /// \name Information
  //@{
  /// Returns the arity of the view
  int arity(void) const {
    return r_.arity();
  }
  //@}
  /// \name Access operations
  //@{
  /**
   * \brief Returns the lower bound.
   *
   * As the view is constant, this returns a copy of the relation itself.
   */
  GRelation glb(void) const {
    return r_;
  }
  /**
   * \brief Returns the upper bound.
   *
   * As the view is constant, this returns a copy of the relation itself.
   */
  GRelation lub(void) const {
    return r_;
  }
  /**
   * \brief Unknown access.
   *
   * As a constant view is considered assigned this method retuns an empty ground
   * relation as the unknown.
   */
  GRelation unk(void) const {
    return GRelation(r_.arity());
  }
  /**
   * \brief Out of bound access.
   *
   * The elements that are not part of the relation in the case of a constant view
   * correspond to the complement of the relation.
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
