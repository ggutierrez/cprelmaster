#ifndef __CPREL_CPTUPLE_VARIMP_HH__
#define __CPREL_CPTUPLE_VARIMP_HH__

#include <gecode/kernel.hh>
#include <cprel/grelation.hh>

using Gecode::Advisor;
using Gecode::Delta;
using Gecode::ModEvent;
using Gecode::Propagator;
using Gecode::PropCond;
using Gecode::Space;

namespace MPG {
/**
 * \brief Conceptually a ground relation is a set of tuples.
 */
typedef CPRel::GRelation TupleSet;
/**
 * \brief Iterator on set of tuples
 */
typedef CPRel::GRelationIter TupleSetIter;

/**
 * \brief The tuple concept is common to both systems and getting rid of the
 * namespace make the operations simpler to understand
 */
//typedef CPRel::Tuple Tuple;

namespace CPTuple {
// limits
namespace Limits {
const int max = (INT_MAX / 2) - 1;
const int min = -max;
}
// delta for advisors
class CPTupleDelta : public Delta {
private:
  int l, u;
public:
  CPTupleDelta(int min, int max) : l(min), u(max) {}
  int min(void) const {
    return l;
  }
  int max(void) const {
    return u;
  }
};

/**
 * \brief Tuple domain.
 *
 * A tuple domain is represented by the set of tuples that are possible in the
 * variable.
 */
class CPTupleVarImp : public CPTupleVarImpBase {
protected:
  /// Domain representation
  TupleSet dom_;
public:
  /// \a Constructors and disposer
  //@{
  /// Constructor for a variable with empty lower bound and
  CPTupleVarImp(Space& home, const TupleSet& u);
  /// Resources disposal
  void dispose(Space&);
  //@}
  /// \name Relation information
  //@{
  int arity(void) const { return dom_.arity(); }
  //@}
  /// \name Bound access
  //@{
  /// Returns the set of possible tuples in the variable.
  TupleSet domain(void) const { return dom_; }
  //@}
  /// \name Pruning operations
  //@{
  /// Assigns the variable to represent \a t
  ModEvent assign(Space& home, const CPRel::Tuple& t);
  /// Excludes \a t as one of the possible tuples represented
  ModEvent exclude(Space& home, const CPRel::Tuple& t);
  /// Excludes the set of tuples \a s from the represented tuples
  ModEvent exclude(Space& home, const TupleSet& s);
  //@}
  /// \name Domain tests
  //@{
  /// Tests for assignment
  bool assigned(void) const {
    return dom_.cardinality() == 1;
  }
  //@}
  /// \name Subscriptions handling
  //@{
  // subscriptions
  void subscribe(Space& home, Propagator& p, PropCond pc, bool schedule=true) {
    CPTupleVarImpBase::subscribe(home,p,pc,assigned(),schedule);
  }
  void subscribe(Space& home, Advisor& a) {
    CPTupleVarImpBase::subscribe(home,a,assigned());
  }
  void cancel(Space& home, Propagator& p, PropCond pc) {
    CPTupleVarImpBase::cancel(home,p,pc,assigned());
  }
  void cancel(Space& home, Advisor& a) {
    CPTupleVarImpBase::cancel(home,a,assigned());
  }
  //@}
  /// \name Copying
  //@{
  CPTupleVarImp(Space& home, bool share, CPTupleVarImp& y)
    : CPTupleVarImpBase(home,share,y), dom_(y.dom_) {}

  CPTupleVarImp* copy(Space& home, bool share) {
    if (copied())
      return static_cast<CPTupleVarImp*>(forward());
    else
      return new (home) CPTupleVarImp(home,share,*this);
  }
  //@}
  // delta information
  static int min(const Delta& d) {
    return static_cast<const CPTupleDelta&>(d).min();
  }
  static int max(const Delta& d) {
    return static_cast<const CPTupleDelta&>(d).max();
  }
};
}}

#endif
