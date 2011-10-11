#ifndef __BDDDOMAIN_CUDD_FACTORY_HH__
#define __BDDDOMAIN_CUDD_FACTORY_HH__

#include <bdddomain/bdd.hh>
#include <vector>

namespace MPG { namespace VarImpl {

    namespace Limits {
      /**
       * \brief Number of bits to represent an element inside a relation
       * tuple.
       * \ingroup DomRepr
       *
       * The maximum number that can be part of relation's tuple is 2^bbv.
       * - Setting this attribute to 5 will allow to represent positive integers
       *   of 32 bits.
       */
      const int bbv = 2;
      /**
       * \brief Number of bits of the maximum arity that can be represented
       * \ingroup DomRepr
       *
       * The maximum arity that a relation can have is 2^ba.
       * - Setting this attribute to 3 will allow to represent up to 8-ary relations
       */
      const int ba = 2;
      /**
       * \brief Number of bits used to represent each element of a tuple
       * \ingroup DomRepr
       */
      const int bitsPerInteger = 1 << bbv;
      /**
       * \brief Number of bits used to represent the arity
       * \ingroup DomRepr
       */
      const int arity = 1 << ba;

      /**
       * \brief Ensures that the library is compiled on an architecture in which the
       * bits in an integer fits in the bdd representation. This is because most of the
       * operations on tuples or anything that has a bdd representation behind use the
       * int type to input and output values.
       * \ingroup DomRepr
       */

      //  BOOST_STATIC_ASSERT(std::numeric_limits<int>::digits <= bitsPerInteger
      //                      && "The BDD configuration is wrong for this built");
    }

    class BDDDomain;
    /**
     * \brief Class to abstract the way bdd managers are handled by
     * CUDD.
     */
    class CUDDBDDFactory {
    private:
      /// Reference to the internal representation of the manager
      Cudd cudd;
    public:
      /** \brief Constructor with initial values for the number of
       * nodes and the size of the cache
       */
      CUDDBDDFactory(int nodeNum, int cacheSize)
	: cudd(nodeNum,0,0,cacheSize) {
	std::cout << "Initialized bdd factory " << std::endl;
      }
      /// Destructor
      ~CUDDBDDFactory(void) {
	auto references = Cudd_CheckZeroRef(cudd.getManager());
	std::cout << "Destroyed bdd factory: " <<  references << std::endl;
      }
      /// Returns a bdd representing zero
      Bdd zero(void) {
	return cudd.bddZero();
      }
      /// Returns a bdd representing one
      Bdd one(void) {
	return cudd.bddOne();
      }
      /// Returns the ith bdd variable
      Bdd ithVar(int i) {
	return cudd.bddVar(i);
      }
      /**
       * \brief Return a bdd representing the negation of the ith bdd
       * variable
       */
      Bdd nithVar(int i) {
	return !ithVar(i);
      }
      /**
       * \brief Returns the level of variable \a var
       */
      int var2level(int var) const {
	return cudd.ReadPerm(var);
      }
      /**
       * \brief Returns the variable at a given level \a level
       */
      int level2var(int level) const {
	return cudd.ReadInvPerm(level);
      }
      /**
       * \brief Returns a cube of the variables for the given array of indices.
       *
       * The function in Cudd takes a c array, to avoid const problems
       * we do not take a reference to the vector but a copy of it.
       */
      Bdd cube(std::vector<int> indices) {
	return cudd.IndicesToCube(&indices[0],indices.size());
      }
    };
    
  }}

#endif
