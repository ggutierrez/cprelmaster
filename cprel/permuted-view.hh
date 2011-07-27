#ifndef __CPREL_CPREL_PERMUTED_VIEW_HH__
#define __CPREL_CPREL_PERMUTED_VIEW_HH__

#include <cprel/cprel.hh>
#include <rel/grelation.hh>

namespace MPG { namespace CPRel {

/**
 * \brief Permuted relation view
 * \ingroup CPRelViews
 *
 * A permuted view \f$Xp\f$ for a relation view \f$R\f$ provides operations
 * such that \f$Xp\f$ behaves like the permutation of \f$R\f$ defined by the
 * permutation description \f$ desc \f$.
 */
template <typename View>
class PermutedView : public Gecode::DerivedView<View> {
protected:
  /// View
  using Gecode::DerivedView<View>::x;

  /// \todo inverse_ can be computed on the flight but it is needed for the
  /// basic constraints performed on the views. Is it better to have it as a
  /// member or compute it every time is needed?

  /// Associated descriptor
  const PermDescriptor desc_;
  /// Inverse descriptor
  const PermDescriptor inverse_;
public:
  /// \name Constructors
  //@{
  /// Default constructor
  PermutedView(void);
  /// Initialize from relation view \a y and a descriptor \a d
  explicit PermutedView(View& y, const PermDescriptor& d);
  /// Copy constructor
  PermutedView(const PermutedView<View>& v)
    : Gecode::DerivedView<View>(v), desc_(v.desc_), inverse_(v.inverse_) {}
  //@}
  /// \name Access operations
  //@{
  /**
   * \brief Returns a relation representing the greatest lower bound of the
   * variable.
   */
  GRelation glb(void) const {
    return x.glb().permute(desc_);
  }
  /**
   * \brief Returns a relation representing the least upper bound of the
   * variable.
   */
  GRelation lub(void) const {
    return x.lub().permute(desc_);
  }
  /**
   * \brief Unknown access.
   *
   * Returns a relation (copy) with the maximum relation that can be included in
   * the lower bound. \f$unk = lub \setminus glb \f$
   */
  GRelation unk(void) const {
    return x.unk().permute(desc_);
  }
  /**
   * \brief Out of bound access.
   *
   * Returns a relation (copy) that is known to not be part of the set of relations
   * represented by the variable. \f$oob = O \setminus lub \f$, wher \f$ O \f$ is
   * the full relation.
   */
  GRelation oob(void) const {
    return x.oob().permute(desc_);
  }
  //@}
  /// \name Modification operations
  //@{
  /**
   * \brief Prune the variable by doing: \f$ glb = glb \cup r \f$
   *
   */
  ModEvent include(Space& home, const GRelation& r) {
    GRelation s = r.permute(inverse_);
    return x.include(home,s);
  }
  /**
   * \brief Prune the variable by doing: \f$ lub = lub \setminus r \f$
   *
   */
  ModEvent exclude(Space& home, const GRelation& r) {
    GRelation s = r.permute(inverse_);
    return x.exclude(home,s);
  }
  //@}
  /// \name Dependencies
  //@{
  /**
   * \brief Subscribe propagator \a p with propagation condition \a pc to view
   *
   * In case \a schedule is false, the propagator is just subscribed but
   * not scheduled for execution (this must be used when creating
   * subscriptions during propagation).
   */
  void subscribe(Space& home, Propagator& p, PropCond pc, bool schedule = true) {
    x.subscribe(home,p,pc,schedule);
  }
  /// Cancel subscription of propagator \a p with propagation condition \a pc to view
  void cancel(Space& home, Propagator& p, PropCond pc) {
    x.cancel(home,p,pc);
  }
  /// Subscribe advisor \a a to view
  void subscribe(Space& home, Advisor& a) {
    x.subscribe(home,a);
  }
  /// Cancel subscription of advisor \a a
  void cancel(Space& home, Advisor& a) {
    x.cancel(home,a);
  }
  //@}
  /// \name View-dependent propagator support
  //@{
  /// Schedule propagator \a p with modification event \a me
  static void schedule(Space& home, Propagator& p, ModEvent me) {
    return View::schedule(home,p,me);
  }
  /// Return modification event for view type in \a med
  //static ModEvent me(const ModEventDelta& med);
  /// Translate modification event \a me to modification event delta for view
  //static ModEventDelta med(ModEvent);
  //@}
};
}}

namespace MPG { namespace CPRel {

template <typename View>
PermutedView<View>::PermutedView(void) {}

template <typename View>
PermutedView<View>::PermutedView(View& y, const PermDescriptor& d)
  : Gecode::DerivedView<View>(y), desc_(d), inverse_(d.inverse()) {

  if (!desc_.valid(y.arity()))
    throw InvalidPermutation("permutation constraint (invalid especification)");
}


}}
#endif
