#ifndef __CPREL_CPREL_VARIMP_HH__
#define __CPREL_CPREL_VARIMP_HH__

#include <gecode/kernel.hh>
#include <cprel/grelation.hh>

using Gecode::Advisor;
using Gecode::Delta;
using Gecode::ModEvent;
using Gecode::Propagator;
using Gecode::PropCond;
using Gecode::Space;

namespace MPG { namespace CPRel {

// limits
namespace Limits {
const int max = (INT_MAX / 2) - 1;
const int min = -max;
}
// delta for advisors
class CPRelDelta : public Delta {
private:
  int l, u;
public:
  CPRelDelta(int min, int max) : l(min), u(max) {}
  int min(void) const {
    return l;
  }
  int max(void) const {
    return u;
  }
};

/**
 * \brief Relation domain.
 *
 * A relation domain is approximated by means of a greatest lower
 * bound \c glb and a least upper bound \c lub.
 */
class CPRelVarImp : public CPRelVarImpBase {
protected:
  /// Greatest lower bound
  GRelation glb_;
  /// Least upper bound
  GRelation lub_;
public:
  /// \a Constructors and disposer
  //@{
  /// Constructor for a variable with empty lower bound and
  CPRelVarImp(Space& home, const GRelation& l, const GRelation& u)
    : CPRelVarImpBase(home), glb_(l), lub_(u) {}
  /// Resources disposal
  void dispose(Space&) {
    //    std::cout << "Starting disposal" << std::endl;
    glb_.~GRelation();
    //    std::cerr << "--disposing--" << std::endl;
    lub_.~GRelation();
    //    std::cout << "Finishing disposal" << std::endl;
  }
  //@}
  /// \name Bound access
  //@{
  /**
   * \brief Returns a relation representing the greatest lower bound of the
   * variable.
   *
   * This relation contains the tuples that are known to be part of the variable.
   */
  GRelation glb(void) const {
    return glb_;
  }
  /**
   * \brief Returns a relation representing the least upper bound of the
   * variable.
   *
   * This relation contains the tuples that are possible part of the variable.
   */
  GRelation lub(void) const {
    return lub_;
  }
  /**
   * \brief Unknown access.
   *
   * Returns a relation (copy) with the maximum relation that can be included in
   * the lower bound. \f$unk = lub \setminus glb \f$
   */
  GRelation unk(void) const {
    return lub_.difference(glb_);
  }
  /**
   * \brief Out of bound access.
   *
   * Returns a relation (copy) that is known to not be part of the set of relations
   * represented by the variable. \f$oob = O \setminus lub \f$, wher \f$ O \f$ is
   * the full relation.
   */
  GRelation oob(void) const {
    GRelation o(GRelation::create_full(glb_.arity()));
    return o.difference(lub_);
  }
  //@}
  /// \name Pruning operations
  //@{
  /**
   * \brief Prune the variable by doing: \f$ glb = glb \cup r \f$
   *
   */
  ModEvent include(Space& home, const GRelation& r) {
    if (r.superset(lub_)) return ME_CPREL_FAILED;
    if (r.subsetEq(glb_)) return ME_CPREL_NONE;
    glb_.unionAssign(r);
    CPRelDelta d(1,2);
    return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MIN, d);
  }
  /**
   * \brief Prune the variable by doing: \f$ lub = lub \setminus r \f$
   *
   */
  ModEvent exclude(Space& home, const GRelation& r) {
    if (!r.disjoint(glb_)) return ME_CPREL_FAILED;
    if (r.disjoint(lub_)) return ME_CPREL_NONE;
    lub_.differenceAssign(r);
    CPRelDelta d(1,2);
    return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MAX, d);
  }
  //@}
  /// \name Domain tests
  //@{
  /// Tests for assignment \f$ glb = lub \f$
  bool assigned(void) const {
    return glb_.eq(lub_);
  }
  //@}
  /// \name Subscriptions handling
  //@{
  // subscriptions
  void subscribe(Space& home, Propagator& p, PropCond pc,
                 bool schedule=true) {
    CPRelVarImpBase::subscribe(home,p,pc,assigned(),schedule);
  }
  void subscribe(Space& home, Advisor& a) {
    CPRelVarImpBase::subscribe(home,a,assigned());
  }
  void cancel(Space& home, Propagator& p, PropCond pc) {
    CPRelVarImpBase::cancel(home,p,pc,assigned());
  }
  void cancel(Space& home, Advisor& a) {
    CPRelVarImpBase::cancel(home,a,assigned());
  }
  //@}
  /// \name Copying
  //@{
  CPRelVarImp(Space& home, bool share, CPRelVarImp& y)
    : CPRelVarImpBase(home,share,y), glb_(y.glb_), lub_(y.lub_) {}

  CPRelVarImp* copy(Space& home, bool share) {
    if (copied())
      return static_cast<CPRelVarImp*>(forward());
    else
      return new (home) CPRelVarImp(home,share,*this);
  }
  //@}
  // delta information
  static int min(const Delta& d) {
    return static_cast<const CPRelDelta&>(d).min();
  }
  static int max(const Delta& d) {
    return static_cast<const CPRelDelta&>(d).max();
  }
};

}}


#endif
