#ifndef __CPREL_PROP_EQUALITY_HH__
#define __CPREL_PROP_EQUALITY_HH__

#include <cprel/cprel.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Propagates: \f$ \forall t : t \in A \iff t \in B \f$
 * \ingroup SetProp
 */
template <typename ViewLeft, typename ViewRight>
class Equal : public Gecode::Propagator {
protected:
  /// Left relation of the contraint
  ViewLeft left_;
  /// Right relation of the constraint
  ViewRight right_;
public:
  /// Constructor for the propagator \f$ equal(left,right) \f$
  Equal(Gecode::Home home, ViewLeft left, ViewRight right)
    : Gecode::Propagator(home), left_(left), right_(right) {
    left_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    right_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  }
  /// Propagator posting
  static Gecode::ExecStatus post(Gecode::Home home,
                                 ViewLeft left, ViewRight right) {
    if (Gecode::same(left,right))
      return Gecode::ES_OK;
    (void) new (home) Equal(home,left,right);
    return Gecode::ES_OK;
  }
  /// Propagator disposal
  virtual size_t dispose(Gecode::Space& home) {
    left_.cancel(home,*this,CPRel::PC_CPREL_BND);
    right_.cancel(home,*this,CPRel::PC_CPREL_BND);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  /// Copy constructor
  Equal(Gecode::Space& home, bool share, Equal& p)
    : Gecode::Propagator(home,share,p) {
    left_.update(home,share,p.left_);
    right_.update(home,share,p.right_);
  }
  /// Copy
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Equal(home,share,*this);
  }
  /// Cost
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::binary(Gecode::PropCost::LO);
  }
  /// Main propagation algorithm
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    // The lower bounds of both variables must contain the same elements
    GECODE_ME_CHECK(left_.include(home,right_.glb()));
    GECODE_ME_CHECK(right_.include(home,left_.glb()));

    GRelation commonUpperBound(left_.lub().intersect(right_.lub()));
    GECODE_ME_CHECK(
          left_.exclude(home,commonUpperBound.complement())
          );

    GECODE_ME_CHECK(
          right_.exclude(home,commonUpperBound.complement())
          );

    // Propagator subsumpiton
    if (left_.assigned() && right_.assigned())
      return home.ES_SUBSUMED(*this);

    return Gecode::ES_FIX;
  }
};
}}}
#endif
