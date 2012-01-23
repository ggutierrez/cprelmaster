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
   * \brief Returns the upper bound of b_ permuted in a way that the follow part
   * is at the right.
   */
  GRelation PBLub(void) const {
    PermDescriptor d;
    for (int i = 0; i < f_; i++) d.permute(i,f_+i);
    for (int i = f_; i < b_.arity(); i++) d.permute(i,f_-i);
    return b_.lub().permute(d);
  }
  GRelation TransformB(GRelation r) const {
    PermDescriptor d;
    for (int i = 0; i < f_; i++) d.permute(i,f_+i);
    for (int i = f_; i < b_.arity(); i++) d.permute(i,f_-i);
    return r.permute(d);
  }
  /**
   * \brief Returns the lower bound of c_ permuted in a way that the part comming
   * from B is at the left.
   */
  GRelation PCGlb(void) const {
    PermDescriptor d;
    int columns_a = a_.arity() - f_;
    int i = 0;
    for (; i < columns_a - 1; i++)  d.permute(i,a_.arity() - f_ + i);

    int columns_b = b_.arity() - f_;
    for (; i < columns_b + columns_a; i++) d.permute(i,i + columns_b - columns_a);

    return c_.glb().permute(d);
  }

  GRelation transformC(GRelation r) const {
    PermDescriptor d;
    int columns_a = a_.arity() - f_;
    int i = 0;
    for (; i < columns_a - 1; i++)  d.permute(i,a_.arity() - f_ + i);

    int columns_b = b_.arity() - f_;
    for (; i < columns_b + columns_a; i++) d.permute(i,i + columns_b - columns_a);

    return r.permute(d);
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
    // 2) pruning A from B and C
    {
      // pruning the lower bound of A
      GRelation x = c_.glb().follow(b_.arity() - f_,PBLub()).intersect(a_.lub());
      std::vector<int> uq(f_,-1);
      for (int i = 0; i < f_; i++) uq[i] = i;
      GRelation singles = x.unique(uq).intersect(x);
      GECODE_ME_CHECK(a_.include(home,singles));

      // pruning the upper bound of A
      /*GRelation y = c_.oob().follow(b_.arity() - f_, TransformB(b_.glb()));
      GECODE_ME_CHECK(a_.exclude(home,y));*/
    }

    // 3) Pruning B from A and C
    {
      // pruning the lower bound of B
      GRelation x = TransformB(PCGlb().follow(a_.arity() - f_,a_.lub())).intersect(b_.lub());
      std::vector<int> uq(f_,-1);
      for (int i = 0; i < f_; i++) uq[i] = i;
      GRelation singles = x.unique(uq).intersect(x);
      GECODE_ME_CHECK(b_.include(home,singles));

      // pruning the upper bound of B
      /*GRelation y = transformC(c_.oob()).follow(a_.arity() - f_, a_.glb());
      GRelation b_out = TransformB(y);
      GECODE_ME_CHECK(b_.exclude(home,b_out));*/
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
