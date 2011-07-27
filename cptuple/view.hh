#ifndef __CPREL_CPTUPLE_VIEW_HH__
#define __CPREL_CPTUPLE_VIEW_HH__

#include <gecode/kernel.hh>
#include <cptuple/var.hh>

namespace MPG { namespace CPTuple {
/**
 * \brief View for tuple variables.
 * \ingroup CPTupleViews
 *
 * Normal view on variable implementation of tuple variable.
 */
class CPTupleView : public Gecode::VarImpView<CPTupleVar> {
protected:
  /// Variable
  using Gecode::VarImpView<CPTupleVar>::x;
public:
  /// \name Constructors
  //@{
  /// Default constructor
  CPTupleView(void) {}
  /// Constructor from a variable
  CPTupleView(const CPTupleVar& y)
    : Gecode::VarImpView<CPTupleVar>(y.varimp()) {}
  /// Constructor from a variable implementation
  CPTupleView(CPTupleVarImp* y)
    : Gecode::VarImpView<CPTupleVar>(y) {}
  //@}
  /// \name Information
  //@{
  /// Returns the arity of the view
  int arity(void) const {
    return x->arity();
  }
  //@}
  /// \name Access operations
  //@{
  /// Returns the set of possible tuples in the view
  GRelation domain(void) const {
    return x->domain();
  }
  //@}
  /// \name Modification operations
  //@{
  ModEvent assign(Space& home, const Tuple& t) {
    return x->assign(home,t);
  }
  /// Excludes \a t as one of the possible tuples represented
  ModEvent exclude(Space& home, const Tuple& t) {
    return x->exclude(home,t);
  }
  /// Excludes the set of tuples \a s from the represented tuples
  ModEvent exclude(Space& home, const TupleSet& s) {
    return x->exclude(home,s);
  }

  //@}
  // delta information
  int min(const Delta& d) const {
    return CPTupleVarImp::min(d);
  }
  int max(const Delta& d) const {
    return CPTupleVarImp::max(d);
  }
};

/**
 * \brief Output tuple view \a t to \a os
 * \ingroup CPTupleViews
 */
template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator<<(std::basic_ostream<Char,Traits>& os, const CPTupleView& t) {

  std::basic_ostringstream<Char,Traits> s;
  s.copyfmt(os); s.width(0);
  using namespace CPTuple;
  if (t.assigned()) s << "val: ";
  s << t.domain();
  return os << s.str();
}

}}

//#include <cprel/const-view.hh>
//#include <cprel/complement-view.hh>
//#include <cprel/permuted-view.hh>

#endif
