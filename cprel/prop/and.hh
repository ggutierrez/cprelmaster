#ifndef __CPREL_PROP_AND_HH__
#define __CPREL_PROP_AND_HH__

#include <cprel/cprel.hh>
#include <cprel/prop/equality.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Intersection propagator between two relations
 * \ingroup SetProp
 */
class Intersect : public Gecode::Propagator {
protected:
  CPRel::CPRelView a_, b_, c_;
public:
  Intersect(
    Gecode::Home home, CPRel::CPRelView a, CPRel::CPRelView b,CPRel::CPRelView c)
    : Gecode::Propagator(home), a_(a), b_(b), c_(c) {
    a_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    b_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    c_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  }
  static Gecode::ExecStatus
  post(Gecode::Home home,
       CPRel::CPRelView a, CPRel::CPRelView b, CPRel::CPRelView c) {
    using CPRel::CPRelView;

    if (Gecode::same(a,b)) {
      return Equal<CPRelView,CPRelView>::post(home,a,c);
    }
    if (Gecode::same(a,c)) {
      return Equal<CPRelView,CPRelView>::post(home,b,c);
    }
    if (Gecode::same(b,c)) {
      return Equal<CPRelView,CPRelView>::post(home,a,c);
    }
    (void) new (home) Intersect(home,a,b,c);
    return Gecode::ES_OK;
  }
  virtual size_t dispose(Gecode::Space& home) {
    a_.cancel(home,*this,CPRel::PC_CPREL_BND);
    b_.cancel(home,*this,CPRel::PC_CPREL_BND);
    c_.cancel(home,*this,CPRel::PC_CPREL_BND);

    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  Intersect(Gecode::Space& home, bool share, Intersect& p)
    : Gecode::Propagator(home,share,p) {
    a_.update(home,share,p.a_);
    b_.update(home,share,p.b_);
    c_.update(home,share,p.c_);
  }
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Intersect(home,share,*this);
  }
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::binary(Gecode::PropCost::LO);
  }
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    // glb(a_) \cap glb(b_) \subseteq glb(c_)
    GECODE_ME_CHECK(
          c_.include(home,a_.glb().intersect(b_.glb()))
          );
    // lub(_c) \subseteq lub(_a) \cap lub(b)
    GECODE_ME_CHECK(
          c_.exclude(home,a_.lub().intersect(b_.lub()).complement())
          );
    // glb(_c) \subseteq glb(_a)
    GECODE_ME_CHECK(
          a_.include(home,c_.glb())
          );
    // lub(_a) \subseteq lub(_c)
    GECODE_ME_CHECK(
          a_.exclude(home,c_.lub().complement())
          );

    // glb(_c) \subseteq glb(_b)
    GECODE_ME_CHECK(
          b_.include(home,c_.glb())
          );
    // lub(_b) \subseteq lub(_c)
    GECODE_ME_CHECK(
          b_.exclude(home,c_.lub().complement())
          );

    // Propagator subsumpiton
    if (a_.assigned() && b_.assigned() && c_.assigned())
      return home.ES_SUBSUMED(*this);

    return Gecode::ES_FIX;
  }
};
}}

/**
 * \brief Posts: \f$ A \cap B = C \f$
 * \ingroup SetProp
 */
void intersect(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C);

}
#endif
