#ifndef __CPREL_PROP_CHANNEL_HH__
#define __CPREL_PROP_CHANNEL_HH__

#include <gecode/set.hh>
#include <cprel/cprel.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Propagates: \f$ \forall t : t \in A \iff t \in B \f$
 * \ingroup SetProp
 * \todo Documentation!
 */
template <typename RelView, typename SetView>
class Channel : public Gecode::Propagator {
protected:
  /// Relation
  RelView rel_;
  /// Set
  SetView set_;
public:
  /// Constructor for the propagator \f$ channel(r,s) \f$
  Channel(Gecode::Home home, RelView rel, SetView set)
    : Gecode::Propagator(home), rel_(rel), set_(set) {
    rel_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    /// \todo What will happen if set_ is a complement view and we subscribe to
    /// any change on it?
    set_.subscribe(home,*this,Gecode::Set::PC_SET_ANY);
  }
  /// Propagator posting
  static Gecode::ExecStatus post(Gecode::Home home, RelView rel, SetView set) {
    (void) new (home) Channel(home,rel,set);
    return Gecode::ES_OK;
  }
  /// Propagator disposal
  virtual size_t dispose(Gecode::Space& home) {
    rel_.cancel(home,*this,CPRel::PC_CPREL_BND);
    set_.cancel(home,*this,Gecode::Set::PC_SET_ANY);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  /// Copy constructor
  Channel(Gecode::Space& home, bool share, Channel& p)
    : Gecode::Propagator(home,share,p) {
    rel_.update(home,share,p.rel_);
    set_.update(home,share,p.set_);
  }
  /// Copy
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Channel(home,share,*this);
  }
  /// Cost
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::binary(Gecode::PropCost::LO);
  }
  /// Main propagation algorithm
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    std::cout << "Propagating channel constraint" << std::endl;

    /// \todo  The current implementation of this constraint iterates on the
    /// tuples of the relation. It does not take any advantage from the domain
    /// representation. It would be better if this can be improved.

    // Pruning the set based on the information in the relation
    // 1a Every knwon element in the relation must be known in the set
    for (GRelationIter relKnown(rel_.glb()); relKnown(); ++relKnown) {
      std::cout << "Should be known in set " << relKnown.val() << std::endl;
    }

    // 1b Every possible element in the relation must be possible in the set
    for (GRelationIter relUnkwnon(rel_.unk()); relUnkwnon(); ++relUnkwnon) {
      std::cout << "Should be possible in set " << relUnkwnon.val() << std::endl;
    }

    // Pruning the relation based on the information in the set
    // 2a Every known element in the set must be known in the relation
    auto knownRangesSet = Gecode::Set::GlbRanges<SetView>(set_);
    auto knownValuesSet =
        Gecode::Iter::Ranges::ToValues<decltype(knownRangesSet)>(knownRangesSet);
    for (; knownValuesSet(); ++knownValuesSet) {
      std::cout << "Should be possible in rel " << knownValuesSet.val() << std::endl;
    }
    // 2b Every possible element in the set must be possible in the relation
    auto unknownRangesSet = Gecode::Set::UnknownRanges<SetView>(set_);
    auto unknownValuesSet =
        Gecode::Iter::Ranges::ToValues<decltype(unknownRangesSet)>(unknownRangesSet);
    for (; unknownValuesSet(); ++unknownValuesSet) {
      std::cout << "Should be possible in rel " << unknownValuesSet.val() << std::endl;
    }


    // Propagator subsumpiton
    if (set_.assigned()) {
      assert(rel_.assigned());
      return home.ES_SUBSUMED(*this);
    }
    if (rel_.assigned()) {
      assert(set_.assigned());
      return home.ES_SUBSUMED(*this);
    }

    return Gecode::ES_FIX;
  }
};
}}}
#endif
