#ifndef __CPREL_PROP_PERMUTATION_HH__
#define __CPREL_PROP_PERMUTATION_HH__

#include <cprel/cprel.hh>
#include <bdddomain/perm-descriptor.hh>

namespace MPG { namespace CPRel { namespace Prop {
      /**
       * \brief Propagates: \f$ \forall t : t \in A \iff t \in B \f$
       * \ingroup SetProp
       * \todo Fix deinition of the comment
       */
      class Permute : public Gecode::Propagator {
      protected:
	/// Left relation of the contraint
	CPRelView left_;
	/// Right relation of the constraint
	CPRelView right_;
	/// Permutation descrition
	const PermDescriptor permutation_;
      public:
	/// Constructor for the propagator \f$ equal(left,right) \f$
	Permute(Gecode::Home home, CPRelView left, CPRelView right, const PermDescriptor& permutation)
	  : Gecode::Propagator(home), left_(left), right_(right), permutation_(permutation) {
	  left_.subscribe(home,*this,CPRel::PC_CPREL_BND);
	  right_.subscribe(home,*this,CPRel::PC_CPREL_BND);
	}
	/// Propagator posting
	static Gecode::ExecStatus post(Gecode::Home home,
				       CPRelView left, CPRelView right, const PermDescriptor& permutation) {
	  if (Gecode::same(left,right))
	    return Gecode::ES_OK;
	  (void) new (home) Permute(home,left,right,permutation);
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
	Permute(Gecode::Space& home, bool share, Permute& p)
	  : Gecode::Propagator(home,share,p), permutation_(p.permutation_) {
	  left_.update(home,share,p.left_);
	  right_.update(home,share,p.right_);
	}
	/// Copy
	virtual Gecode::Propagator* copy(Gecode::Space& home, bool share) {
	  return new (home) Permute(home,share,*this);
	}
	/// Cost
	virtual Gecode::PropCost cost(const Gecode::Space&,
				      const Gecode::ModEventDelta&) const {
	  return Gecode::PropCost::binary(Gecode::PropCost::LO);
	}
	/// Main propagation algorithm
	virtual Gecode::ExecStatus propagate(Gecode::Space& home,
					     const Gecode::ModEventDelta&)  {

	  // The lower bounds of both variables must contain the same elements
	  GECODE_ME_CHECK(left_.include(home,right_.glb().permute(permutation_.inverse())));
	  GECODE_ME_CHECK(right_.include(home,left_.glb().permute(permutation_)));
	  
	  GRelation commonUpperBound(left_.lub().intersect(right_.lub().permute(permutation_.inverse())));
	  GECODE_ME_CHECK(
			  left_.exclude(home,commonUpperBound.complement())
			  );
	  
	  GRelation commonUpperBoundR(left_.lub().permute(permutation_).intersect(right_.lub()));
	  GECODE_ME_CHECK(
			  right_.exclude(home,commonUpperBoundR.complement())
			  );
	  
	  // Propagator subsumpiton
	  if (left_.assigned() && right_.assigned())
	    return home.ES_SUBSUMED(*this);
	  
	  return Gecode::ES_FIX;
	}
      };
    }}}
#endif
