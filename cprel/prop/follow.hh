#ifndef __CPREL_PROP_FOLLOW_HH__
#define __CPREL_PROP_FOLLOW_HH__

#include <cprel/cprel.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Follow propagator between two relations
 * \ingroup RelProp
 */
template <typename ViewA, typename ViewB, typename ViewC>
class Follow : public Gecode::Propagator {
protected:
  /// First relation
  ViewA a_;
  /**
   * \brief Number of columns that must match in order to consider a tuple to
   * follow another in the relation.
   */
  int f_;
  /// Second relation
  ViewB b_;
  /// Third relation
  ViewC c_;
public:
  /// Constructor
  Follow(
    Gecode::Home home, ViewA a, int j, ViewB b,ViewC c)
    : Gecode::Propagator(home), a_(a), f_(j), b_(b), c_(c) {
    a_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    b_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    c_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  }
  /**
   * \brief Join propagator posting
   *
   * \todo What will happen if this propagator is posted with the same view as
   * two of the arguments?.
   */
  static Gecode::ExecStatus
  post(Gecode::Home home, ViewA a, int f, ViewB b, ViewC c) {
    (void) new (home) Follow(home,a,f,b,c);
    return Gecode::ES_OK;
  }
  virtual size_t dispose(Gecode::Space& home) {
    a_.cancel(home,*this,CPRel::PC_CPREL_BND);
    b_.cancel(home,*this,CPRel::PC_CPREL_BND);
    c_.cancel(home,*this,CPRel::PC_CPREL_BND);

    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  Follow(Gecode::Space& home, bool share, Follow& p)
    : Gecode::Propagator(home,share,p), a_(p.a_), f_(p.f_), b_(p.b_), c_(p.c_) {
    a_.update(home,share,p.a_);
    b_.update(home,share,p.b_);
    c_.update(home,share,p.c_);
  }
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Follow(home,share,*this);
  }
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::binary(Gecode::PropCost::LO);
  }
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    std::cout << "Follow proagaor running" << std::endl;

    // Propagator subsumption
    if (a_.assigned() && b_.assigned() && c_.assigned()) {
      /// \todo is it possible to get another subsumption condition??
      return home.ES_SUBSUMED(*this);
    }

    return Gecode::ES_NOFIX;
  }
};
}}}
#endif
