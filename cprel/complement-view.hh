#ifndef __CPREL_CPREL_COMPLEMENT_VIEW_HH__
#define __CPREL_CPREL_COMPLEMENT_VIEW_HH__

#include <cprel/cprel.hh>
#include <bdddomain/grelation.hh>

namespace MPG { namespace CPRel {

/**
 * \brief Complement relation view
 * \ingroup CPRelViews
 *
 * A complement view \f$C\f$ for a relation view \f$R\f$ provides operations
 * such that \f$C\f$ behaves like the complement of \f$R\f$.
 */
template <typename View>
class ComplementView : public Gecode::DerivedView<View> {
protected:
  /// View
  using Gecode::DerivedView<View>::x;
public:
  /// \name Translate propgatation conditions and modification events
  //@{
  /// Negate a propagation condition \a pc
  static Gecode::PropCond pc_negatecprel(Gecode::PropCond pc);
  /// Negate the modification event \a me
  static Gecode::ModEvent me_negatecprel(Gecode::ModEvent me);
  //@}
  /// \name Constructors
  //@{
  /// Default constructor
  ComplementView(void);
  /// Initialize from relation view \a y
  explicit ComplementView(View& y);
  /// Copy constructor
  ComplementView(const ComplementView<View>& v)
    : Gecode::DerivedView<View>(v) {}
  //@}
  /// \name Information
  //@{
  /// Returns the arity of the view
  int arity(void) const {
    return x.arity();
  }
  //@}
  /// \name Access operations
  //@{
  /**
   * \brief Returns a relation representing the greatest lower bound of the
   * variable.
   */
  GRelation glb(void) const {
    return x.oob();
  }
  /**
   * \brief Returns a relation representing the least upper bound of the
   * variable.
   */
  GRelation lub(void) const {
    return x.glb().complement();
  }
  /**
   * \brief Unknown access.
   *
   * Returns a relation (copy) with the maximum relation that can be included in
   * the lower bound. \f$unk = lub \setminus glb \f$
   */
  GRelation unk(void) const {
    return x.unk();
  }
  /**
   * \brief Out of bound access.
   *
   * Returns a relation (copy) that is known to not be part of the set of relations
   * represented by the variable. \f$oob = O \setminus lub \f$, wher \f$ O \f$ is
   * the full relation.
   */
  GRelation oob(void) const {
    return x.glb();
  }
  //@}
  /// \name Modification operations
  //@{
  /**
   * \brief Prune the variable by doing: \f$ glb = glb \cup r \f$
   *
   */
  ModEvent include(Space& home, const GRelation& r) {
    return me_negatecprel(x.exclude(home,r));
  }
  /**
   * \brief Prune the variable by doing: \f$ lub = lub \setminus r \f$
   *
   */
  ModEvent exclude(Space& home, const GRelation& r) {
    return me_negatecprel(x.include(home,r));
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
    x.subscribe(home,p, pc_negatecprel(pc),schedule);
  }
  /// Cancel subscription of propagator \a p with propagation condition \a pc to view
  void cancel(Space& home, Propagator& p, PropCond pc) {
    x.cancel(home,p, pc_negatecprel(pc));
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
    return View::schedule(home,p,me_negatecprel(me));
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
Gecode::PropCond
ComplementView<View>::pc_negatecprel(Gecode::PropCond pc) {
  switch(pc) {
  case PC_CPREL_MIN: return PC_CPREL_MAX;
  case PC_CPREL_MAX: return PC_CPREL_MIN;
  default: return pc;
  }
}

template <typename View>
Gecode::ModEvent
ComplementView<View>::me_negatecprel(Gecode::ModEvent me) {
  switch(me){
  case ME_CPREL_MIN: return ME_CPREL_MAX;
  case ME_CPREL_MAX: return ME_CPREL_MIN;
  default: return me;
  }
}

template <typename View>
ComplementView<View>::ComplementView(void) {}

template <typename View>
ComplementView<View>::ComplementView(View& y)
  : Gecode::DerivedView<View>(y) {}


}}
#endif
