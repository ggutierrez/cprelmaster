#ifndef __CPREL_PROP_PROJECTION_HH__
#define __CPREL_PROP_PROJECTION_HH__

#include <cprel/cprel.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Propagates: \f$ \Pi_{p}A = B \f$.
 *
 * \f$ A \subseteq \mathcal{U}_{n},\; B \subseteq \mathcal{U}_{n-p} \f$ and \f$p\f$
 * is the number of righ most columns to project on.
 * \ingroup RelProp
 */
template <typename ViewLeft, typename ViewRight>
class Project : public Gecode::Propagator {
protected:
  /// Left relation of the contraint
  ViewLeft left_;
  /// Right relation of the constraint
  ViewRight right_;
  /// Number of columns (on the right) to project on.
  int p_;
public:
  /// Constructor
  Project(Gecode::Home home, int p, ViewLeft left, ViewRight right)
    : Gecode::Propagator(home), left_(left), right_(right), p_(p) {
    left_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    right_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  }
  /// Propagator posting
  static Gecode::ExecStatus post(Gecode::Home home, int p, ViewLeft left,
                                 ViewRight right) {

    // check that the relations has the right arities
    if (left.arity()-p != right.arity()) {
      std::cerr << "Invalid arity for the relations or the size of the descriptor"
                << std::endl;
      return Gecode::ES_FAILED;
    }

    // If left and right are the same variable then the constraint is eihter trivially
    // entailed (p refers to all the columns) or failed as there is no way to satisfy
    // it.
    if (same(left,right)) {
      if (p != 0) return Gecode::ES_FAILED;
      return Gecode::ES_OK;
    }

    (void) new (home) Project(home,p,left,right);
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
  Project(Gecode::Space& home, bool share, Project& p)
    : Gecode::Propagator(home,share,p), p_(p.p_) {
    left_.update(home,share,p.left_);
    right_.update(home,share,p.right_);
  }
  /// Copy
  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
    return new (home) Project(home,share,*this);
  }
  /// Cost
  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const {
    return Gecode::PropCost::binary(Gecode::PropCost::LO);
  }
public:
  /// Main propagation algorithm
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {
    // implements: \Pi_{p}A = B
    // First part: \Pi_{p}A \implies B
    //    \Pi_{p}glb(A) \subseteq B
    GRelation glbLeft_p = left_.glb().project(p_);
    GECODE_ME_CHECK(right_.include(home,glbLeft_p));

    //    \Pi_{p}lub(A) \supseteq B
    GRelation lubLeft_p = left_.lub().project(p_);
    GECODE_ME_CHECK(right_.exclude(home,lubLeft_p.complement()));

    // lub(A) \subseteq (U_{B.arity()-p}\times lub(B)) \cap lub(B)
    GRelation maxIntersection =
        right_.lub().timesU(left_.arity()-p_,true).intersect(left_.lub());

    GECODE_ME_CHECK(left_.exclude(home,maxIntersection.complement()));
    /// \todo Missing the other direction of the propagator

    // Propagator subsumpiton
    if (left_.assigned() && right_.assigned())
      return home.ES_SUBSUMED(*this);

    return Gecode::ES_FIX;
  }
};
}}}
#endif
