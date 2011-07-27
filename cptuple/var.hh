#ifndef __CPREL_CPTUPLE_VAR_HH__
#define __CPREL_CPTUPLE_VAR_HH__

#include <gecode/kernel.hh>
//#include <cprel/grelation.hh>
#include <cptuple/varimp.hh>


namespace MPG {
/**
 * \brief Tuple variables.
 * \ingroup CPTupleVars
 *
 * This class provides the way to create and access information of tuple
 * variables.
 */
class CPTupleVar : public Gecode::VarImpVar<CPTuple::CPTupleVarImp> {
protected:
  /// Variable implementation
  using Gecode::VarImpVar<CPTuple::CPTupleVarImp>::x;
public:
  /// \name Constructors
  //@{
  /// Default constructor
  CPTupleVar(void) {}
  /// Copy constructor
  CPTupleVar(const CPTupleVar& y)
    : Gecode::VarImpVar<CPTuple::CPTupleVarImp>(y.varimp()) {}
  /// Constructor from a variable implementation \a y
  CPTupleVar(CPTuple::CPTupleVarImp *y)
    : Gecode::VarImpVar<CPTuple::CPTupleVarImp>(y) {}
  //@}
  /// \name Variable creation
  //@{
  /**
   * \brief Creates a tuple variable with \a u as the possible set of tuples.
   *
   * \warning Throws a CPTuple::VariableEmptyDomain if the provided bound describes
   * an empty domain.
   */
  CPTupleVar(Space& home, const TupleSet& u)
    : Gecode::VarImpVar<CPTuple::CPTupleVarImp>
      (new (home) CPTuple::CPTupleVarImp(home,u)) {
    /// \todo Throw exception
//    if (l.arity() != u.arity())
//      throw CPRel::ArityMissmatch("CPTupleVar::CPTupleVar");
  }
  //@}
  /// \name Variable information
  //@{
  /// Returns the arity (number of elements that the tuple can hold)
  int arity(void) const {
    return x->arity();
  }
  //@}
  /// \name Domain ifnormation
  //@{
  /// Returns the possible set of tuples that the variable represents.
  TupleSet domain(void) const {
    return x->domain();
  }
  //@}
};

/**
 * \brief Output tuple variable \a t to \a os
 * \ingroup CPTupleVars
 */
template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const CPTupleVar& t) {
  std::basic_ostringstream<Char,Traits> s;
  s.copyfmt(os); s.width(0);

  using namespace CPTuple;
  if (t.assigned()) os << "val:";
  os << t.domain();
  return os << s.str();
}

}

#endif
