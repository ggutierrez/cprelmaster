#ifndef __CPREL_PROP_JOIN_HH__
#define __CPREL_PROP_JOIN_HH__

#include <cprel/cprel.hh>
#include <cprel/prop/join.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Join propagator between two relations
 * \ingroup RelProp
 */
template <typename ViewA, typename ViewB, typename ViewC>
class Join : public Gecode::Propagator {
protected:
  /// First relation
  ViewA a_;
  /// Columns to join on
  int j_;
  /// Second relation
  ViewB b_;
  /// Third relation
  ViewC c_;
public:
  /// Constructor
  Join(
    Gecode::Home home, ViewA a, int j, ViewB b,ViewC c)
    : Gecode::Propagator(home), a_(a), j_(j), b_(b), c_(c) {
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
  post(Gecode::Home home, ViewA a, int j, ViewB b, ViewC c) {
    (void) new (home) Join(home,a,j,b,c);
    return Gecode::ES_OK;
  }
  virtual size_t dispose(Gecode::Space& home) {
    a_.cancel(home,*this,CPRel::PC_CPREL_BND);
    b_.cancel(home,*this,CPRel::PC_CPREL_BND);
    c_.cancel(home,*this,CPRel::PC_CPREL_BND);

    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  Join(Gecode::Space& home, bool share, Join& p)
    : Gecode::Propagator(home,share,p), a_(p.a_), j_(p.j_), b_(p.b_), c_(p.c_) {
    a_.update(home,share,p.a_);
    b_.update(home,share,p.b_);
    c_.update(home,share,p.c_);
  }
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Join(home,share,*this);
  }
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::binary(Gecode::PropCost::LO);
  }
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    std::cout << "Propagating join!" << std::endl;
    // implements: A \bowtie_{j} B = C
    // First part: C \subseteq A \bowtie_{j} B

    GRelation glbJ = a_.glb().join(j_,b_.glb());
    GECODE_ME_CHECK(c_.include(home,glbJ));

    GRelation lubJ = a_.lub().join(j_,b_.lub());
    GECODE_ME_CHECK(c_.exclude(home,lubJ.complement()));

    /// \todo Add other direction of the constraint.
    // Propagator subsumpiton
    if (a_.assigned() && b_.assigned() && c_.assigned())
      return home.ES_SUBSUMED(*this);

    return Gecode::ES_FIX;
  }
};
}}}
#endif
