#ifndef __CPREL_PROP_PROJECTION_HH__
#define __CPREL_PROP_PROJECTION_HH__

#include <cprel/cprel.hh>

namespace MPG { namespace CPRel { namespace Prop {
      /**
       * \brief Propagates: \f$ \Pi_{p}A = B \f$.
       * \ingroup RelProp
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

	  /// \todo If the arity of a is j then this constraint should reduce to equality??
	  // check that the relations has the right arities
	  if (p > a.arity() || b.arity() != p) {
	    std::cerr << "Invalid arity for the relations" << std::endl;
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
	/**
	 * \brief Main propagation algorithm.
	 *
	 * Implements: \f$\Pi_{p}A=B\f$
	 *
	 * Where:
	 * - \f$A \subseteq \mathcal{U}_{n}\f$
	 * - \f$p \in \{x : 0 \leq x \leq n\}\f$
	 * - \f$B \subseteq \mathcal{U}_{p}\f$
	 *
	 * Conventions:
	 * - \f$\mathcal{U}_{n}\f$ denotes the universe of all the tuples with arity \f$n\f$
	 * - \f$A_{\text{glb}} = \text{glb}(A)\f$
	 * - \f$A_{\text{lub}} = \text{lub}(A)\f$
	 * - \f$\Pi_{p}^{*}R\f$ denotes the projection operation on ground relation \f$R\f$
	 *
	 * Propagation rules:
	 * - \f$\Pi_{p}^{*}A_{\text{glb}} \subseteq B \subseteq \Pi_{p}^{*}A_{\text{lub}}\f$
	 * - \f$(U_{n-p}\times B_{\text{lub}}) \cap A_{\text{lub}} \supseteq A\f$
	 * - \f$A \subseteq U_{q}\cap (U_{n-p}\times B_{\text{glb}})\f$
	 *
	 * Where:
	 * -\f$q \subseteq \{x:p\leq x \leq n-1\}\f$
	 * -\f$U_{q} = A_{\text{lub}}\cap\exists !_{q}A_{\text{lub}}\f$
	 */
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

	  // lub(A) \subseteq (U_{A.arity()-p}\times lub(B)) \cap lub(A)
	  GRelation maxIntersection =
	    b_.lub().timesULeft(a_.arity()-p_).intersect(a_.lub());
	  GECODE_ME_CHECK(a_.exclude(home,maxIntersection.complement()));

	  // prune the lower bound of A by unique quantification

	  GRelation lubA = a_.lub();
	  std::vector<int> q;
	  q.reserve(a_.arity()-p_);
	  for (int i = p_; i < a_.arity(); i++) {
	    q.push_back(i);
	  }

	  GRelation Uq = lubA.unique(q).intersect(a_.lub());
	  if (! Uq.empty()) {
	    GRelation to_include = Uq.intersect(b_.glb().timesULeft(a_.arity()-p_));
	    GECODE_ME_CHECK(a_.include(home,to_include));
	  }

	  // Propagator subsumpiton
	  if (a_.assigned() && b_.assigned())
	    /// \todo b can be assigned before a!
	    return home.ES_SUBSUMED(*this);

	  return Gecode::ES_FIX;
	}
      };
    }}}
#endif
