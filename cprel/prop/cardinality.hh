#ifndef __CPREL_PROP_CARDINALITY_HH__
#define __CPREL_PROP_CARDINALITY_HH__

#include <gecode/int.hh>
#include <cprel/cprel.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Propagates: \f$ |A| = B\f$
 * \ingroup SetProp
 */
template <typename ViewLeft, typename ViewRight>
class Cardinality : public Gecode::Propagator {
protected:
  /// The relation
  ViewLeft left_;
  /// The integer variable with the cardinality
  ViewRight right_;
public:
  /// Constructor for the propagator \f$ |left| = right \f$
  Cardinality(Gecode::Home home, ViewLeft left, ViewRight right)
    : Gecode::Propagator(home), left_(left), right_(right) {
    // subscribe to any change in the relation
    left_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    // subscribe to any change in the cardinality
    right_.subscribe(home,*this,Gecode::Int::PC_INT_BND);
    // Is it possible to subscribe to PC_INT_DOM and to do something
    // useful in the case where elements different from the bounds are
    // changed?
  }
  /// Propagator posting
  static Gecode::ExecStatus post(Gecode::Home home,
                                 ViewLeft left, ViewRight right) {
    (void) new (home) Cardinality(home,left,right);
    return Gecode::ES_OK;
  }
  /// Propagator disposal
  virtual size_t dispose(Gecode::Space& home) {
    left_.cancel(home,*this,CPRel::PC_CPREL_BND);
    right_.cancel(home,*this,Gecode::Int::PC_INT_BND);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  /// Copy constructor
  Cardinality(Gecode::Space& home, bool share, Cardinality& p)
    : Gecode::Propagator(home,share,p) {
    left_.update(home,share,p.left_);
    right_.update(home,share,p.right_);
  }
  /// Copy
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Cardinality(home,share,*this);
  }
  /// Cost
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::binary(Gecode::PropCost::LO);
  }
  /// Main propagation algorithm
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    //std::cout << "Executing cardinality constraint" << std::endl;
    // Minimum cardinality of the relation
    double glbCard = left_.glb().cardinality();
    // Maximum cardinality of the relation
    double lubCard = left_.lub().cardinality();

    GECODE_ME_CHECK(right_.lq(home,lubCard));
    GECODE_ME_CHECK(right_.gq(home,glbCard));

    // Propagator subsumpiton
    if (glbCard == lubCard && right_.assigned())
      return home.ES_SUBSUMED(*this);

    return Gecode::ES_FIX;
  }
};
}}}
#endif
