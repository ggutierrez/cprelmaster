#ifndef __CPREL_BDDDOMAIN_BDD_HH__
#define __CPREL_BDDDOMAIN_BDD_HH__

#include <bdddomain/manager.hh>
#include <cudd/cudd.h>

namespace MPG { namespace VarImpl {
    /**
     * \brief Returns the variables (indices) used in the bdd to encode column \a c
     * \ingroup BDDEnc
     */
    std::vector<int> domainIndices(int c);

    /**
     * \brief returns the \a then node of \a r
     *
     * The OO API of CUDD does not offer access to this operation.
     */
    inline BDD high(const BDD& r) {
      DdNode * d = r.getNode();
      DdNode *res = Cudd_T(d);
      res = Cudd_NotCond(res, Cudd_IsComplement(d));
      return BDD(factoryPtr(),res);
    }

    /**
     * \brief returns the \a else node of \a r
     *
     * The OO API of CUDD does not offer access to this operation.
     */
    inline BDD low(const BDD& r) {
      DdNode *d  = r.getNode();
      DdNode *res = Cudd_E(d);
      res = Cudd_NotCond(res, Cudd_IsComplement(d));
      return BDD(factoryPtr(),res);   
    }

    /**
     * \brief returns the \a level of variable \a r in the bdd manager
     *
     * The OO API of CUDD does not offer access to this operation.
     */
    inline int var2Level(int v) {
      return Cudd_ReadPerm(dd(),v);
    }

    /**
     * \brief returns the \a variable at a given level in the manager
     *
     * The OO API of CUDD does not offer access to this operation.
     */
    inline int level2Var(int level) {
      return Cudd_ReadInvPerm(dd(),level);
    }

    inline int var(const BDD& r) {
      return r.NodeReadIndex();
    }

    /**
     * \brief Traverses \a r and executes \a fc on every tuple represented.
     */
    template <typename Functor>
    void traverseSet(Cudd& factory, const BDD& r, Functor& fc);

    /**
     * \brief Outputs \a b to \a os in dot format.
     */
    void printDot(const BDD& b, std::ostream& os = std::cout);

    /**
     * \brief Outputs a relation represented by \a b to \a os.
     */
    void printSet(const BDD& b, std::ostream& os = std::cout);
  }
}

#include <bdddomain/bdd-visitor.hpp>
#endif
