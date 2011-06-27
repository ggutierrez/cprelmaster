#ifndef __CPREL_PROP_PROJECTION_HH__
#define __CPREL_PROP_PROJECTION_HH__

#include <cprel/cprel.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Propagates: \f$ \Pi_{p}A = B \f$.
 * \ingroup RelProp
 *
 * \f$ A \subseteq \mathcal{U}_{n},\; B \subseteq \mathcal{U}_{n-p} \f$ and \f$p\f$
 * is the number of columns on the right (counting from zero) to project on.
 */
template <typename ViewA, typename ViewB>
class Project : public Gecode::Propagator {
protected:
  /// Left relation of the contraint
  ViewA a_;
  /// Right relation of the constraint
  ViewB b_;
  /// Number of columns (on the right) to project on.
  int p_;
public:
  /// Constructor
  Project(Gecode::Home home, int p, ViewA a, ViewB b)
    : Gecode::Propagator(home), a_(a), b_(b), p_(p) {
    a_.subscribe(home,*this,CPRel::PC_CPREL_BND);
    b_.subscribe(home,*this,CPRel::PC_CPREL_BND);
  }
  /// Propagator posting
  static Gecode::ExecStatus post(Gecode::Home home, int p, ViewA a, ViewB b) {

    // check that the relations has the right arities
    if (a.arity()-p != b.arity()) {
      std::cerr << "Invalid arity for the relations or the size of the descriptor"
                << std::endl;
      return Gecode::ES_FAILED;
    }

    // If left and right are the same variable then the constraint is eihter trivially
    // entailed (p refers to all the columns) or failed as there is no way to satisfy
    // it.
    if (same(a,b)) {
      if (p != 0) return Gecode::ES_FAILED;
      return Gecode::ES_OK;
    }

    (void) new (home) Project(home,p,a,b);
    return Gecode::ES_OK;
  }
  /// Propagator disposal
  virtual size_t dispose(Gecode::Space& home) {
    a_.cancel(home,*this,CPRel::PC_CPREL_BND);
    b_.cancel(home,*this,CPRel::PC_CPREL_BND);
    (void) Propagator::dispose(home);
    return sizeof(*this);
  }
  /// Copy constructor
  Project(Gecode::Space& home, bool share, Project& p)
    : Gecode::Propagator(home,share,p), p_(p.p_) {
    a_.update(home,share,p.a_);
    b_.update(home,share,p.b_);
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
    GRelation glbA_p = a_.glb().project(p_);
    GECODE_ME_CHECK(b_.include(home,glbA_p));

    //    \Pi_{p}lub(A) \supseteq B
    GRelation lubA_p = a_.lub().project(p_);
    GECODE_ME_CHECK(b_.exclude(home,lubA_p.complement()));

    // lub(A) \subseteq (U_{B.arity()-p}\times lub(B)) \cap lub(B)
    GRelation maxIntersection =
        b_.lub().timesU(a_.arity()-p_,true).intersect(a_.lub());
    GECODE_ME_CHECK(a_.exclude(home,maxIntersection.complement()));

    /// \todo Missing 1/4 of the propagator: unique quantifier.
    
    // prune the lower bound of a by unique quantification
    GRelation lubA = a_.lub();
    std::vector<int> q;
    for (int i = a_.arity()-1; i >= p_; i++)
      q.push_back(i);
    
    GRelation QLubA = lubA.unique(q).intersect(lubA);
    GRelation to_include = QLubA.intersect(b_.glb().timesU(a_.arity()-p_,true));
    GECODE_ME_CHECK(a_.include(home,to_include));
    

    // Propagator subsumpiton
    if (a_.assigned() && b_.assigned())
      return home.ES_SUBSUMED(*this);

    return Gecode::ES_FIX;
  }
};
}}}
#endif
