#ifndef __CPREL_BDDDOMAIN_MANAGER_HH__
#define __CPREL_BDDDOMAIN_MANAGER_HH__

#include <iostream>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <cudd/cuddInt.h>

namespace MPG { namespace CPRel { namespace VarImpl {

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
const int bbv = 5;
/**
 * \brief Number of bits of the maximum arity that can be represented
 * \ingroup DomRepr
 *
 * The maximum arity that a relation can have is 2^ba.
 * - Setting this attribute to 3 will allow to represent up to 8-ary relations
 */
const int ba = 3;
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
 * \brief Tests whether \a v can be encoded with the current setup of the
 * manager.
 * \ingroup DomRepr
 */
inline
bool fits(int v) {
  return ((~((1LL << bitsPerInteger)-1)) & v) == 0;
}
}

class BddManager;
typedef boost::shared_ptr<BddManager> PManager;
/**
 * \brief Class to handle the creation and destruction of Cudd related objects.
 * \ingroup DomRepr
 *
 * The idea of having this class is to create a global object that
 * is in charge of the resource deallocation of Cudd entities. For
 * this reason only one manager is used and this class implements the
 * singleton pattern.
 */
class BddManager : private boost::noncopyable {
private:
  /// Only one single instance of this class is allowed at run time
  static PManager _instance;
  /// Pointer to the Bdd manager
  DdManager *dd;
  /// Constant true
  DdNode *one_;
  /// Constant false
  DdNode *zero_;
  /// \name Constructors and destructor
  //@{
  /// Constructor that initializes the BDD manager of CUDD
  BddManager (void)
    : dd(Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0))
    , one_(DD_ONE(dd))
    , zero_(Cudd_Not(DD_ONE(dd)))
  {
    std::cout << "Created bdd manager" << std::endl;
  }
  /// Creates an instace of this object
  static void create() {
    _instance.reset( new BddManager, &deleter );
  }
  /// Destructor that releases the BDD manager of CUDD
  ~BddManager (void) {
    std::cout << "Called destructor: " << references() << std::endl;
    Cudd_Quit(dd);
  }
  //@}
  /// Method called by the managed pointer when destructed
  static void deleter(BddManager *ptr) {
    delete ptr;
  }
  int references(void) {
    return Cudd_CheckZeroRef(dd);
  }
public:
  /// Returns an instance of the manager
  static PManager instance(void) {
    if (_instance.get() != NULL)
      return _instance;
    create();
    return _instance;
  }
  /// \name Access
  //@{
  /// Returns the bdd manager
  DdManager* manager(void) {
    return dd;
  }
  /// Returns the constant \c true
  DdNode* one(void) {
    return one_;
  }
  /// Returns the constant \c false
  DdNode* zero(void) {
    return zero_;
  }
  //@}
};

/**
 * \brief Returns the manager
 * \ingroup DomRepr
 */
inline
DdManager* dd(void) {
  return BddManager::instance()->manager();
}

/**
 * \brief Returns logical true
 * \ingroup DomRepr
 */
inline
DdNode* one(void) {
  return BddManager::instance()->one();
}

/**
 * \brief Returns logical false
 * \ingroup DomRepr
 */
inline
DdNode* zero(void) {
  return BddManager::instance()->zero();
}
}}}
#endif
