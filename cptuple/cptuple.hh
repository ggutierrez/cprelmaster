#ifndef __CPREL_CPTUPLE_HH__
#define __CPREL_CPTUPLE_HH__

#include <gecode/kernel.hh>
#include <cprel/grelation.hh>

using Gecode::Exception;
using Gecode::VarArgArray;
using Gecode::VarArray;


// exceptions
namespace MPG { namespace CPTuple {


/// Exception indicating invalid limits
//class OutOfLimits : public Exception {
//public:
//  OutOfLimits(const char* l)
//    : Exception(l,"Number out of limits") {}
//};
/// Exception indicating empty domain declaration
//class VariableEmptyDomain : public Exception {
//public:
//  VariableEmptyDomain(const char* l)
//    : Exception(l,"Attempt to create variable with empty domain") {}
//};
}}

// Variable implementation definition
#include <cptuple/varimp.hh>

// Variable definition
#include <cptuple/var.hh>

// array traits
namespace MPG {
class CPTupleVarArgs; class CPTupleVarArray;
}

namespace Gecode {
template<>
class ArrayTraits<Gecode::VarArray<MPG::CPTupleVar> > {
public:
  typedef MPG::CPTupleVarArray  StorageType;
  typedef MPG::CPTupleVar       ValueType;
  typedef MPG::CPTupleVarArgs   ArgsType;
};
template<>
class ArrayTraits<MPG::CPTupleVarArray> {
public:
  typedef MPG::CPTupleVarArray  StorageType;
  typedef MPG::CPTupleVar       ValueType;
  typedef MPG::CPTupleVarArgs   ArgsType;
};
template<>
class ArrayTraits<Gecode::VarArgArray<MPG::CPTupleVar> > {
public:
  typedef MPG::CPTupleVarArgs   StorageType;
  typedef MPG::CPTupleVar       ValueType;
  typedef MPG::CPTupleVarArgs   ArgsType;
};
template<>
class ArrayTraits<MPG::CPTupleVarArgs> {
public:
  typedef MPG::CPTupleVarArgs  StorageType;
  typedef MPG::CPTupleVar      ValueType;
  typedef MPG::CPTupleVarArgs  ArgsType;
};

}
// variable arrays
namespace MPG {
class CPTupleVarArgs : public VarArgArray<CPTupleVar> {
public:
  CPTupleVarArgs(void) {}
  explicit CPTupleVarArgs(int n) : VarArgArray<CPTupleVar>(n) {}
  CPTupleVarArgs(const CPTupleVarArgs& a) : VarArgArray<CPTupleVar>(a) {}
  CPTupleVarArgs(const VarArray<CPTupleVar>& a) : VarArgArray<CPTupleVar>(a) {}
  /**
   * \brief Constructor for an array of \a n variables initialized with \a u as
   * the set of possible tuples.
   */
  CPTupleVarArgs(Space& home, int n, const TupleSet& u)
    : VarArgArray<CPTupleVar>(n) {
    for (int i=0; i<n; i++)
      (*this)[i] = CPTupleVar(home,u);
  }
};
class CPTupleVarArray : public VarArray<CPTupleVar> {
public:
  CPTupleVarArray(void) {}
  CPTupleVarArray(const CPTupleVarArray& a)
    : VarArray<CPTupleVar>(a) {}
  /**
   * \brief Constructor for an array of \a n variables initialized with \a u as
   * the set of possible tuples.
   */
  CPTupleVarArray(Space& home, int n, const TupleSet& u)
    : VarArray<CPTupleVar>(home,n) {
    for (int i=0; i<n; i++)
      (*this)[i] = CPTupleVar(home,u);
  }
};
}

// View implementation
#include <cptuple/view.hh>

#endif
