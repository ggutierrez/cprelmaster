#ifndef __CPREL_PROP_EQUALITY_HH__
#define __CPREL_PROP_EQUALITY_HH__

#include <cprel/cprel.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Equality propagator between two relations
 * \ingroup SetProp
 */
template <typename ViewLeft, typename ViewRight>
class Equal : public Gecode::Propagator {
protected:
  ViewLeft left_;
  ViewRight right_;
public:
  Equal(Gecode::Home home, ViewLeft left, ViewRight right)
    : Gecode::Propagator(home), left_(left), right_(right) {
    left_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    right_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  }
  static Gecode::ExecStatus post(Gecode::Home home,
                                 ViewLeft left, ViewRight right) {
    if (Gecode::same(left,right))
      return Gecode::ES_OK;
    (void) new (home) Equal(home,left,right);
    return Gecode::ES_OK;
  }
  virtual size_t dispose(Gecode::Space& home) {
    left_.cancel(home,*this,CPRel::PC_CPREL_BND);
    right_.cancel(home,*this,CPRel::PC_CPREL_BND);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  Equal(Gecode::Space& home, bool share, Equal& p)
    : Gecode::Propagator(home,share,p) {
    left_.update(home,share,p.left_);
    right_.update(home,share,p.right_);
  }
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Equal(home,share,*this);
  }
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::binary(Gecode::PropCost::LO);
  }
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    // The lower bounds of both variables must contain the same elements
    GECODE_ME_CHECK(left_.include(home,right_.glb()));
    GECODE_ME_CHECK(right_.include(home,left_.glb()));

//    std::cout << "Caard after 1(left): " << left_.glb().cardinality() << std::endl;
//    std::cout << "Caard after 1(RIGHT): " << right_.glb().cardinality() << std::endl;

//    std::cout << "Elems right.lub: " << right_.lub().cardinality() << std::endl;
//    std::cout << "Elems left.lub: " << left_.lub().cardinality() << std::endl;
//    std::cout << "Elems inter: " << right_.lub().intersect(left_.lub()) << std::endl;
    // Both variables must have the same possible elements
    GRelation commonUpperBound(left_.lub().intersect(right_.lub()));
//    std::cout << "performed: " << commonUpperBound << std::endl;
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
}}

/**
 * \brief Posts: \f$ A = B \f$
 * \ingroup SetProp
 */
void equal(Gecode::Space& home, CPRelVar A, CPRelVar B);

/**
 * \brief Posts: \f$ A = \overline{B} \f$
 * \ingroup SetProp
 */
void complement(Gecode::Space& home, CPRelVar A, CPRelVar B);

}
#endif
