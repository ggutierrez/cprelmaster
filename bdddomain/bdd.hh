#ifndef __BDDDOMAIN_BDD_HH__
#define __BDDDOMAIN_BDD_HH__

#include <obj/cuddObj.hh>
#include <vector>

namespace MPG { namespace VarImpl {

    // Forward declaration
    class CUDDBDDFactory;

    /**
     * \brief Class to abstract the bdd representation across several
     * libraries. For now we consider only cudd.
     */
    class Bdd {
      friend class CUDDBDDFactory;
    private:
      /// Reference to the internal bdd representation
      BDD bdd_;
      /**
       * \brief Constructor from an internal object. Only the factory
       * should call this method.
       */
      Bdd(BDD bdd);
    public:
      /// Defasult constructor
      Bdd(void);
      /// Copy constructor
      Bdd(const Bdd& other);
      /**
       * \brief Computes the complement of the represented bdd
       */
      Bdd operator ! (void) const;
      /**
       * \brief Returns the variable
       */
      unsigned int var(void) const;
      /**
       * \brief Returns the bdd of the "then"
       */
      Bdd high(void) const;
      /**
       * \brief Returns the bdd of the "else"
       */
      Bdd low(void) const;
      /**
       * \brief Tests if this node is terminal one
       */
      bool isOne(void) const;
      /**
       * \brief Tests if this node is terminal zero
       */
      bool isZero(void) const;
      /**
       * \brief Returns the number of nodes in the current bdd
       */
      int nodeCount(void) const;
      /**
       * \brief Returns a copy of the represented bdd
       */
      Bdd id(void) const;
      /**
       * \brief Returns a bdd for ite(node,bddThen,bddElse)
       */
      Bdd ite(const Bdd& bddThen, const Bdd& bddElse) const;
      /**
       * \brief Modify the current bdd to be the and between it and \a other.
       */
      void andWith(const Bdd& other);
      /**
       * \brief Modify the current bdd to be the or between it and \a other.
       */
      void orWith(const Bdd& other);
      /**
       * \brief Returns the result of universaly quenatifying this bdd
       * with respect to \a cube
       */
      Bdd forAll(const Bdd& cube);
      /**
       * \brief Returns the result of existentially quenatifying this
       * bdd with respect to \a cube
       */
      Bdd exists(const Bdd& cube);
      /**
       * \brief Returns a bdd representing the permutation of the two
       * sets of variables \a s0 and \a s1.
       *
       * Both sets are cubes of varibles and are assumed to be disjoint.
       */
      Bdd permute(const Bdd& s0, const Bdd& s1);
      /**
       * \brief Outputs the bdd in dot format to \a os
       */
      void printDot(std::ostream& os) const;
    private:
      /**
       * \brief Helper function to print the bdd in dot format
       */
      int printdot_rec(std::ostream& os, int current, std::vector<bool>& visited, std::vector<int>& names) const;

    };
    
  }}

#endif
