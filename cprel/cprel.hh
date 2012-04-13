#ifndef __CPREL_CPREL_HH__
#define __CPREL_CPREL_HH__

#include <iostream>
#include <sstream>
#include <climits>

#include <gecode/kernel.hh>
#include <gecode/int.hh> // for cardinality constraint
#include <gecode/set.hh> // for channel constraint
//#include <cptuple/cptuple.hh> // for the dom constraint
#include <rel/grelation.hh>

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
		 const GRelation& l, const GRelation& u)
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
    CPRelVarArray(Space& home, int n, const GRelation& l, const GRelation& u)
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

/**
 * \defgroup SetProp Set propagators on relations
 *
 * A relation can be seen as a set of tuples. From this point of view different
 * constraints can be defined. These propagators are presented in this module.
 */

/**
 * \defgroup RelProp Basic relation propagators
 *
 * The propagators presented in this module extend common operations on relations
 * to constraints.  As opposed to databases, columns in the relation decission
 * variables are identified by their possition. This is, a relation of arity \a
 * n has columns \f$0,\ldots,n-1\f$ being the column identified with 0 the right
 * most one.
 *
 * As the notion of "colum names" is merely numeric, some of the constraints that
 * work with column information take numbers identifying the columns as parameters.
 * Moreover, constraints like \a join and \a projection rely on some conventions
 * regarding this imput.
 */

/**
 * \defgroup RelBranch Branchers (distribution strategies) on relations
 *
 * This module contains the branchers that can be used on relation domains.
 */

/**
 * \defgroup ChannelProp Channel propagators on relation variables.
 *
 * Constraints that provide interaction between relation
 * decision variables and variables from other constraint systems.
 */

namespace MPG {
  /**
   * \brief Posts: \f$ A = B \f$
   * \ingroup SetProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{n}\f$
   *
   * \see MPG::CPRel::Prop::Equal
   */
  void equal(Gecode::Space& home, CPRelVar A, CPRelVar B);
  /**
   * \brief Posts: \f$ A = \overline{B} \f$
   * \ingroup SetProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{n}\f$
   */
  void complement(Gecode::Space& home, CPRelVar A, CPRelVar B);
  /**
   * \brief Posts: \f$ A \cap B = C \f$
   * \ingroup SetProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{n}\f$
   * @param C a relation decision variable: \f$C\subseteq\mathcal{U}_{n}\f$
   *
   * \see MPG::CPRel::Prop::Intersect
   */
  void intersect(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C);
  /**
   * \brief Posts: \f$ A \cup B = C \f$
   * \ingroup SetProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{n}\f$
   * @param C a relation decision variable: \f$C\subseteq\mathcal{U}_{n}\f$
   */
  void Union(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C);
  /**
   * \brief Posts: \f$ A \subseteq B \f$
   * \ingroup SetProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{n}\f$
   */
  void subset(Gecode::Space& home, CPRelVar A, CPRelVar B);
  /**
   * \brief Posts: \f$ disjoint(A,B)\f$.
   * \ingroup SetProp
   *
   * Enforces that \a A and \a B are two disjoint relations.
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{n}\f$
   */
  void disjoint(Gecode::Space& home, CPRelVar A, CPRelVar B);
  /**
   * \brief Posts the constraint: \f$ \forall t : t\in A \Rightarrow t\in B \iff t \in C \f$
   * \ingroup SetProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{n}\f$
   * @param C a relation decision variable: \f$C\subseteq\mathcal{U}_{n}\f$
   */
  void implies(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C);
  /**
   * \brief Posts the constraint: \f$ A = B \leftrightarrow_{\text{desc}}\f$.
   * \ingroup RelProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B  a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param desc an object describing the permutation. It represents a map from
   *        columns of \a A to columns of \a B.
   *
   * @see PermDescriptor
   */
  void permutation(Gecode::Space& home, CPRelVar A, CPRelVar B,
		   std::vector<std::pair<int,int> > desc);
  /**
   * \brief Posts the constraint: \f$ \Pi_{p} A = B \f$.
   * \ingroup RelProp
   *
   * @param p is the number of columns on the right of \a A to project on.
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{n-p}\f$
   *
   * @see MPG::CPRel::Prop::Project
   */
  void projection(Gecode::Space& home, int p, CPRelVar A, CPRelVar B);
  /**
   * \brief Posts the constraint: \f$ A\;\bowtie_{j}\;B = C \f$
   * \ingroup RelProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param j is the number of columns on the right of \a A
   * and on the left of \a B to join on.
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{m}\f$
   * @param C a relation decision variable: \f$C\subseteq\mathcal{U}_{n+m-j}\f$
   *
   * @see MPG::CPRel::Prop::Join
   */
  void join(Gecode::Space& home, CPRelVar A, int j, CPRelVar B, CPRelVar C);
  void restrJoinAll(Gecode::Space& home, CPRelVar A, int j, CPRelVar B, CPRelVar C);
  /**
   * \brief Posts the constraint: \f$ A_{\smile_{f}}B = C \f$
   * \ingroup RelProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param f is the number of columns on the right of \a A
   * and on the left of \a B that a match must contain to be consider as a follow.
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{m}\f$
   * @param C a relation decision variable: \f$C\subseteq\mathcal{U}_{n+m-2j}\f$
   *
   * @see MPG::CPRel::Prop::Follow
   */
  void follow(Gecode::Space& home, CPRelVar A, int f, CPRelVar B, CPRelVar C);
  /**
   * \brief Posts the constraint: \f$ A \%_{d}B = C \f$
   * \ingroup RelProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param d is the number of columns on the right of \a A
   * and on the left of \a B that a match must contain to be consider for the division
   * @param B a relation decision variable: \f$B\subseteq\mathcal{U}_{m}\f$
   * @param C a relation decision variable: \f$C\subseteq\mathcal{U}_{n+m-2j}\f$
   *
   * @see MPG::CPRel::Prop::Follow
   */
  void followAll(Gecode::Space& home, CPRelVar A, int d, CPRelVar B, CPRelVar C);
  /**
   * \brief Posts the constraint: \f$ R = S\f$
   * \ingroup ChannelProp
   *
   * @param R a unary relation decision variable: \f$A\subseteq\mathcal{U}_{1}\f$
   * @param S a set decision variable.
   *
   * \warning Throws an exception MPG::CPRel::ArityMissmatch if the arity of \a R
   * is not 1.
   */
  void channel(Gecode::Space& home, CPRelVar R, Gecode::SetVar S);
  /**
   * \brief Posts: \f$ |A| = B \f$
   * \ingroup SetProp
   *
   * @param A a relation decision variable: \f$A\subseteq\mathcal{U}_{n}\f$
   * @param B an integer decision variable: \f$[0..k]\f$
   *
   * \see MPG::CPRel::Prop::Cardinality
   */
  void cardinality(Gecode::Space& home, CPRelVar A, Gecode::IntVar B);
  /**
   * \brief Posts the constraint: \f$ t \in R\f$
   * \ingroup ChannelProp
   *
   * @param t a tuple decision variable which domain is a subset of \f$\mathcal{U}_{n}\f$
   * @param R a relation decision variable: \f$R \subseteq \mathcal{U}_{n}\f$.
   *
   * \warning Throws an exception MPG::CPRel::ArityMissmatch if the arity of \a R
   * and the arity of \a t is not the same.
   */
  //void included(Gecode::Space& home, CPTupleVar t, CPRelVar R);
  /**
   * \brief Posts the constraint: \f$ t \notin R\f$
   * \ingroup ChannelProp
   *
   * @param t a tuple decision variable which domain is a subset of \f$\mathcal{U}_{n}\f$
   * @param R a relation decision variable: \f$R \subseteq \mathcal{U}_{n}\f$.
   *
   * \warning Throws an exception MPG::CPRel::ArityMissmatch if the arity of \a R
   * and the arity of \a t is not the same.
   */
  //void excluded(Gecode::Space& home, CPTupleVar t, CPRelVar R);

  void include(Gecode::Space& home, CPRelVar A, GRelation R);

  void exclude(Gecode::Space& home, CPRelVar A, GRelation R); 
  /**
   * \brief Simple branching on relations.
   * \ingroup RelBranch
   *
   * Branches on relation  \a R by selecting a tuple in it and creating a choice with
   * the tuple as part of the relation for the first alternative and the tuple as
   * excluded from the relation as the second alternative.
   *
   * @param R a relation decision variable.
   *
   */
  void branch(Gecode::Home home, CPRelVar R);
}
#endif
