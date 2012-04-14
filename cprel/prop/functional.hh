#ifndef __CPREL_PROP_FUNCTIONAL_HH__
#define __CPREL_PROP_FUNCTIONAL_HH__

#include <cprel/cprel.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Propagates that a relation \a rel is a function
 * \ingroup FuncProp
 */
template <typename ViewRel>
class Function : public Gecode::Propagator {
protected:
  /// The relation
  ViewRel rel_;
public:
  /// Constructor for the propagator \f$ |left| = right \f$
  Function(Gecode::Home home, ViewRel rel)
    : Gecode::Propagator(home), rel_(rel) {
    // subscribe to any change in the relation
    rel_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  }
  /// Propagator posting
  static Gecode::ExecStatus post(Gecode::Home home, ViewRel rel) {
    (void) new (home) Function(home,rel);
    return Gecode::ES_OK;
  }
  /// Propagator disposal
  virtual size_t dispose(Gecode::Space& home) {
    rel_.cancel(home,*this,CPRel::PC_CPREL_BND);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  /// Copy constructor
  Function(Gecode::Space& home, bool share, Function& p)
    : Gecode::Propagator(home,share,p) {
    rel_.update(home,share,p.rel_);
  }
  /// Copy
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Function(home,share,*this);
  }
  /// Cost
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::unary(Gecode::PropCost::LO);
  }
  /// Main propagation algorithm
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    // the lower bound has a function
    GRelation u = unique(rel_.glb(),{1});
    if (u.cardinality() != rel_.glb().cardinality())
      return Gecode::ES_FAILED;
    
    // remove from the upper bound what cannot be there because there
    // is already something with the first element
    GRelation toRemove = u.unique(0).difference(u);
    GECODE_ME_CHECK(rel_.exclude(home,toRemove));

    // subsumes when the upper bound is a function
    if (unique(rel_.lub(),{1}).cardinality() == rel_.lub().cardinality()) {
      return home.ES_SUBSUMED(*this);
    }
    
    return Gecode::ES_FIX;
  }
};
}}}
#endif
