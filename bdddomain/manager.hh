#ifndef __CPREL_BDDDOMAIN_MANAGER_HH__
#define __CPREL_BDDDOMAIN_MANAGER_HH__

#include <iostream>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <bdddomain/domainSpace.hh>
#include <bdddomain/bdd.hh>


namespace MPG { namespace VarImpl {

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
  static PManager instance_;
  /// Domain space for relations
  DomainSpace ds_;
  /// \name Constructors and destructor
  //@{
  /// Constructor that initializes the BDD manager of CUDD
  BddManager (void)
  : ds_(8,3)
  {
    std::cout << "Created bdd manager" << std::endl;
  }
  /// Creates an instace of this object
  static void create() {
    instance_.reset( new BddManager, &deleter );
  }
  /// Destructor that releases the BDD manager of CUDD
  ~BddManager (void) {
    std::cout << "Destructing the manager " << std::endl;
  }
  //@}
  /// Method called by the managed pointer when destructed
  static void deleter(BddManager *ptr) {
    delete ptr;
  }
  int references(void) {
    return 0;//Cudd_CheckZeroRef(dd);
  }
public:
  /// Returns an instance of the manager
  static PManager instance(void) {
    if (instance_.get() != NULL)
      return instance_;
    create();
    return instance_;
  }
  /// \name Access
  //@{
  /// Returns the constant \c true
  Bdd one(void) {
    return ds_.one();
  }
  /// Returns the constant \c false
  Bdd zero(void) {
    return ds_.zero();
  }
  /// Prints manager statistics to \a os
  void stats(std::ostream& os) const {
    //auto usedMemory = Cudd_ReadMemoryInUse(dd);
    //    os << "\tMemory in use: " <<  (usedMemory / 1024)  << " KB" << std::endl;
    //auto gcTime = Cudd_ReadGarbageCollectionTime(dd);
    //os << "\tGC time: " <<  gcTime  << " ms." << std::endl;
    //auto gcs =  Cudd_ReadGarbageCollections(dd);
    //os << "\tGC triggered " << gcs << " times" << std::endl;
    /*
      This will print a table in the form:
      |-----------------+----------+----------------|
      | Memmory (Bytes) | GC (ms.) | Number of GC's |
      |-----------------+----------+----------------|
     */
    //os << "|" <<  usedMemory   << "|" << gcTime << "|" << gcs << "|" << std::endl;
  }
  //@}
};

/**
 * \brief Returns logical true
 * \ingroup DomRepr
 */
inline
Bdd one(void) {
  return BddManager::instance()->one();
}

/**
 * \brief Returns logical false
 * \ingroup DomRepr
 */
inline
Bdd zero(void) {
  return BddManager::instance()->zero();
}

/**
 * \brief Oututs manager statistics to \a os
 * \ingroup DomRepr
 */
inline
void stats(std::ostream& os) {
  BddManager::instance()->stats(os);
}
}}
#endif
