#ifndef __CPREL_CPREL_HH__
#define __CPREL_CPREL_HH__

#include <iostream>
#include <sstream>
#include <climits>

#include <gecode/kernel.hh>
#include <cprel/grelation.hh>

using Gecode::Exception;
using Gecode::VarArgArray;
using Gecode::VarArray;


// exceptions
namespace MPG { namespace CPRel {

/// Exception indicating invalid limits
class OutOfLimits : public Exception {
public:
  OutOfLimits(const char* l)
    : Exception(l,"Number out of limits") {}
};
/// Exception indicating empty domain declaration
class VariableEmptyDomain : public Exception {
public:
  VariableEmptyDomain(const char* l)
    : Exception(l,"Attempt to create variable with empty domain") {}
};
/// Exception indicating invalid arity
class ArityMissmatch : public Exception {
public:
  ArityMissmatch(const char* l)
    : Exception(l,"Invalid arity for operation") {}
};
/// Exception indicating invalid permutation
class InvalidPermutation : public Exception {
public:
  InvalidPermutation(const char* l)
    : Exception(l,"Invalid permutation for operation") {}
};
}}

// Variable implementation definition
#include <cprel/varimp.hh>

// Variable definition
#include <cprel/var.hh>

// array traits
namespace MPG {
class CPRelVarArgs; class CPRelVarArray;
}

namespace Gecode {
template<>
class ArrayTraits<Gecode::VarArray<MPG::CPRelVar> > {
public:
  typedef MPG::CPRelVarArray  StorageType;
  typedef MPG::CPRelVar       ValueType;
  typedef MPG::CPRelVarArgs   ArgsType;
};
template<>
class ArrayTraits<MPG::CPRelVarArray> {
public:
  typedef MPG::CPRelVarArray  StorageType;
  typedef MPG::CPRelVar       ValueType;
  typedef MPG::CPRelVarArgs   ArgsType;
};
template<>
class ArrayTraits<Gecode::VarArgArray<MPG::CPRelVar> > {
public:
  typedef MPG::CPRelVarArgs   StorageType;
  typedef MPG::CPRelVar       ValueType;
  typedef MPG::CPRelVarArgs   ArgsType;
};
template<>
class ArrayTraits<MPG::CPRelVarArgs> {
public:
  typedef MPG::CPRelVarArgs  StorageType;
  typedef MPG::CPRelVar      ValueType;
  typedef MPG::CPRelVarArgs  ArgsType;
};

}
// variable arrays
namespace MPG {
class CPRelVarArgs : public VarArgArray<CPRelVar> {
public:
  CPRelVarArgs(void) {}
  explicit CPRelVarArgs(int n) : VarArgArray<CPRelVar>(n) {}
  CPRelVarArgs(const CPRelVarArgs& a) : VarArgArray<CPRelVar>(a) {}
  CPRelVarArgs(const VarArray<CPRelVar>& a) : VarArgArray<CPRelVar>(a) {}
  CPRelVarArgs(Space& home, int n,
               const CPRel::GRelation& l, const CPRel::GRelation& u)
    : VarArgArray<CPRelVar>(n) {
    for (int i=0; i<n; i++)
      (*this)[i] = CPRelVar(home,l,u);
  }
};
class CPRelVarArray : public VarArray<CPRelVar> {
public:
  CPRelVarArray(void) {}
  CPRelVarArray(const CPRelVarArray& a)
    : VarArray<CPRelVar>(a) {}
  CPRelVarArray(Space& home, int n,
                const CPRel::GRelation& l, const CPRel::GRelation& u)
    : VarArray<CPRelVar>(home,n) {
    for (int i=0; i<n; i++)
      (*this)[i] = CPRelVar(home,l,u);
  }
};

}

// View implementation
#include <cprel/view.hh>

// Descriptors
#include <bdddomain/perm-descriptor.hh>
#include <bdddomain/proj-descriptor.hh>

namespace MPG {
/**
 * \brief Posts: \f$ A = B \f$
 * \ingroup SetProp
 */
void equal(Gecode::Space& home, CPRelVar A, CPRelVar B);

/**
 * \brief Posts: \f$ A = \overline{B} \f$
 * \ingroup SetProp
 */
void complement(Gecode::Space& home, CPRelVar A, CPRelVar B);
/**
 * \brief Posts: \f$ A \cap B = C \f$
 * \ingroup SetProp
 */
void intersect(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C);
/**
 * \brief Posts: \f$ A \cup B = C \f$
 * \ingroup SetProp
 */
void Union(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C);
/**
 * \brief Posts: \f$ A \subseteq B \f$
 * \ingroup SetProp
 */
void subset(Gecode::Space& home, CPRelVar A, CPRelVar B);
/**
 * \brief Posts: \f$ disjoint(A,B)\f$.
 * \ingroup SetProp
 *
 * Enforces that \a A and \a B are two disjoint relations.
 */
void disjoint(Gecode::Space& home, CPRelVar A, CPRelVar B);
/**
 * \brief Posts the constraint: \f$ \forall t : t\in A \Rightarrow t\in B \iff t \in C \f$
 * \ingroup SetProp
 */
void implies(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C);
/**
 * \brief Posts the constraint: \f$ A = \pi_{desc} B \f$
 * \ingroup RelProp
 */
void permutation(Gecode::Space& home, CPRelVar A, CPRelVar B, const CPRel::PermDescriptor& desc);
/**
 * \brief Posts the constraint: \f$ \Pi_{p} A = B \f$
 * \ingroup RelProp
 */
void projection(Gecode::Space& home, const CPRel::ProjDescriptor& p, CPRelVar A, CPRelVar B);

void branch(Gecode::Home home, CPRelVar x);
}
#endif
