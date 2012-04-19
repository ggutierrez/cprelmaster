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
  /**
   * \brief Return a relation with the parts of \a b_ swapped.
   *
   * \a b_ has two parts: the follow part on the left and the other on
   * the right. This method swap them.
   */
  GRelation swapB(const GRelation& r) const {
	//if arity of B is equal to f, no need to swap
	if(b_.arity() == f_)
		return r;
    GRelation followPart = r.shiftRight(r.arity()-f_);
    GRelation yPart = r.project(r.arity()-f_);
    GRelation x = yPart.timesURight(f_);
    GRelation y = followPart.timesULeft(r.arity()-f_);
    return x.intersect(y);
  }
  /**
   * \brief Transforms \a r into a relation that has the follow part
   * on the left and the other on the right.
   */
  GRelation TransformB(const GRelation& r) const {
	//if arity of B is equal to f, no need to swap
	if(b_.arity() == f_)
		return r;
    GRelation followPart = r.project(f_);
    GRelation yPart = r.shiftRight(f_);
    GRelation x = followPart.timesURight(r.arity()-f_);
    GRelation y = yPart.timesULeft(f_);
    return x.intersect(y);
  }
  /**
   * \brief Returns \a r in a way that the part comming from B is at
   * the left.
   */
  GRelation swapC(const GRelation& r) const {
	//if arity of B is equal to f, no need to swap
	if(b_.arity() == f_)
		return r;
    GRelation xPart = r.shiftRight(b_.arity()-f_);
    GRelation yPart = r.project(b_.arity()-f_);
    GRelation y = yPart.timesURight(a_.arity()-f_);
    GRelation x = xPart.timesULeft(b_.arity()-f_);
    return y.intersect(x);
  }

  /**
   * \brief Main propagation algorithm.
   *
   * Implements: \f$A_{\smile_{f}}B=C\f$
   *
   * Where:
   * - \f$A \subseteq \mathcal{U}_{n}\f$
   * - \f$B \subseteq \mathcal{U}_{m}\f$
   * - \f$f \in \{x : 1 \leq x \leq \text{min}(n,m)\}\f$
   * - \f$C \subseteq \mathcal{U}_{m+n-2f}\f$
   *
   * Conventions:
   * - \f$\mathcal{U}_{n}\f$ denotes the universe of all the tuples with arity \f$n\f$
   * - \f$A_{\text{glb}} = \text{glb}(A)\f$
   * - \f$A_{\text{lub}} = \text{lub}(A)\f$
   * - \f$R_{\smile_{f}^{*}}S\f$ denotes the follow operation on ground relations
   *   \f$R\f$ and \f$S\f$
   *
   * Propagation rules:
   * - \f${A_{\text{glb}}}_{\smile_{f}^{*}}B_{\text{glb}}\subseteq C_{\text{glb}}\f$
   * - \f${A_{\text{lub}}}_{\smile_{f}^{*}}B_{\text{lub}}\supseteq C_{\text{lub}}\f$
   * Where:
   *
   * \todo Find a good latex symbol for the operation and complete the missing
   * four rules
   */
  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&)  {

    // 1) pruning C from A and B
    {
      // C must have atleast the follow of what is known in A and B
      GRelation glbF = a_.glb().follow(f_,b_.glb());
      GECODE_ME_CHECK(c_.include(home,glbF));

      // C can have atmost the join of what is possible in A and B
      GRelation max_possile_c = a_.lub().follow(f_,b_.lub());
      GECODE_ME_CHECK(c_.exclude(home,max_possile_c.complement()));
      }
    
    //2) pruning A from B and C
     {
     		// pruning the lower bound of A
             GRelation x = c_.glb().follow(b_.arity() - f_,swapB(b_.lub())).intersect(a_.lub());
			if(a_.arity() == f_) ////if arity of A is equal to f, no need to quantify on f columns of A
				GECODE_ME_CHECK(a_.include(home,x));
			else
			{
             std::vector<int> uq(f_,-1);
             for (int i = 0; i < f_; i++) uq[i] = i;
             GRelation singles = x.unique(uq).intersect(x);
             GECODE_ME_CHECK(a_.include(home,singles));
            }
             /*
             // pruning the upper bound of A
             GRelation y = c_.oob().follow(b_.arity() - f_, TransformB(b_.glb()));
             GECODE_ME_CHECK(a_.exclude(home,y));
             */
           }
         
           // 3) Pruning B from A and C
           {
             // pruning the lower bound of B
             //GRelation x = TransformB(PCGlb().follow(a_.arity() - f_,a_.lub())).intersect(b_.lub());
             GRelation x = TransformB(swapC(c_.glb()).follow(a_.arity() - f_,a_.lub())).intersect(b_.lub());
			if(b_.arity() == f_) ////if arity of B is equal to f, no need to quantify on f columns of B
				GECODE_ME_CHECK(b_.include(home,x));
			else
			{
             std::vector<int> uq(f_,-1);
             for (int i = 0; i < f_; i++) uq[i] = i;
             GRelation singles = x.unique(uq).intersect(x);
             GECODE_ME_CHECK(b_.include(home,singles));
			}
            
             
             // pruning the upper bound of B
             /*
               GRelation y = swapC(c_.oob()).follow(a_.arity() - f_, a_.glb());
               GRelation b_out = TransformB(y);
               std::cout << "will fail: " << b_out.intersect(b_.glb()) << std::endl;
               //GECODE_ME_CHECK(b_.exclude(home,b_out));
               */
           }
    
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
