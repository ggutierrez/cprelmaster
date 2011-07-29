#ifndef __CPREL_PROP_TUPLE_HH__
#define __CPREL_PROP_TUPLE_HH__

#include <cprel/cprel.hh>
#include <cptuple/cptuple.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Propagates: \f$ \forall t : t \in A \iff t \in B \f$
 * \ingroup SetProp
 */
template <typename TupleView, typename RelView>
class Dom : public Gecode::Propagator {
protected:
  /// Tuple view
  TupleView tuple_;
  /// Relation view
  RelView rel_;
public:
  /// Constructor for the propagator \f$ equal(left,right) \f$
  Dom(Gecode::Home home, TupleView tuple, RelView rel)
    : Gecode::Propagator(home), tuple_(tuple), rel_(rel) {
    tuple_.subscribe(home,*this,CPTuple::PC_CPTUPLE_DOM);
    rel_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  }
  /// Propagator posting
  static Gecode::ExecStatus post(Gecode::Home home, TupleView tuple,
                                 RelView rel) {
    // The implemented constraint is entailed if the known part of the relation
    // already contains the domain of tuple
    if (tuple.domain().subsetEq(rel.glb())) {
      return Gecode::ES_OK;
    }
    // The constraint is failed if no element in the domain of tuple is possible
    // in the relation
    if (tuple.domain().intersect(rel.lub()).empty()) {
      return Gecode::ES_FAILED;
    }
    /// \todo entailed if the possible of tuple is a subset of the relation
    (void) new (home) Dom(home,tuple,rel);
    return Gecode::ES_OK;
  }
  /// Propagator disposal
  virtual size_t dispose(Gecode::Space& home) {
    tuple_.cancel(home,*this,CPTuple::PC_CPTUPLE_DOM);
    rel_.cancel(home,*this,CPRel::PC_CPREL_BND);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  /// Copy constructor
  Dom(Gecode::Space& home, bool share, Dom& p)
    : Gecode::Propagator(home,share,p) {
    tuple_.update(home,share,p.tuple_);
    rel_.update(home,share,p.rel_);
  }
  /// Copy
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Dom(home,share,*this);
  }
  /// Cost
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::binary(Gecode::PropCost::LO);
  }
  /// Main propagation algorithm
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    TupleSet possible = tuple_.domain().intersect(rel_.lub());
    if (possible.empty()) {
      return Gecode::ES_FAILED;
    }
    if(possible.cardinality() == 1) {
      GECODE_ME_CHECK(tuple_.exclude(home,possible.complement()));
    }

    // Propagator subsumpiton
    if (tuple_.domain().subsetEq(rel_.glb()))
      return home.ES_SUBSUMED(*this);

    return Gecode::ES_FIX;
  }
};
}}}
#endif
