#ifndef __CPREL_BDDDOMAIN_MANAGER_HH__
#define __CPREL_BDDDOMAIN_MANAGER_HH__

#include <iostream>
#include <iomanip>
#include <memory>
#include <obj/cuddObj.hh>
#include <climits>

namespace MPG { namespace VarImpl {

    namespace Limits {
      /**
       * \brief Maximum integer that can be part of a tuple in the system
       *
       * This limit must be the same as the one supported by Gecode
       * IntVar, otherwise the compatibility between domains can be
       * seriously affected.
       */
      const int max = INT_MAX - 1;
      /// Minumum allowed integer in a tuple
      const int min = 0;
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
       * \brief Number of variables in the manager for the current setup
       * \ingroup DomRepr
       */
      const int variables = bitsPerInteger * arity;
      /**
       * \brief Ensures that bitsPerInteger is enough to store an int type.
       */
      static_assert((sizeof(int) * 8 == bitsPerInteger) ,
                    "Bits per integer configuration is not enough to store an int.");
   
      /// Checks if \a v is a valid value with the consfiguration of the manager.
      inline bool checkSafeValue(int v) {
        return v >= min && v <= max;
      }
      /// Checks if \a v is a valid arity with the current configuration of the manager.
      inline bool checkSafeArity(int a) {
        return a >= 0 && a <= arity;
      }
    }

    class BddManager;
    typedef std::shared_ptr<BddManager> PManager;
    /**
       \brief Class to interact with the underlying bdd library.
       \ingroup DomRepr
     
       A common design of BDD libraries is to provide a space where
       all the bdd nodes are allocated and the operations are
       performed. The manager will abstract this in order to decouple
       the domain representation from a particular implementation.
     
       The manager is configured by two main constants that will
       define the number of bdd variables that will be used.
     
       This class is also in charge of controlling te life time of the
       bdd objects. There is one global object for all the constraint
       system and no assumption on thread safety can be made on it.
     */
    class BddManager {
    private:
      /// Only one single instance of this class is allowed at run time
      static PManager _instance;
      /// Bdd abstraction offered by CUDD
      Cudd factory_;
      /// \name Constructors and destructor
      //@{
      /// Constructor that initializes the BDD manager of CUDD
      BddManager (void)
	      : factory_(Limits::bitsPerInteger * Limits::arity
                  , 0
                  ,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS
                  ,0)
      {
      	 /// \todo: The existence of shiftLeft needs that the manager is initialized
	       /// with the number of variables. Fix that method in order to permute only existent vars
      }
      /// Creates an instace of this object
      static void create() {
	       _instance.reset( new BddManager, &deleter );
      }
      /// Destructor that releases the BDD manager of CUDD
      ~BddManager (void) {
        std::cout << "Destroyed BDD manager: " << references() << std::endl;
	      //std::cout << "Manager stats" << std::endl;
	      stats(std::cout);
      }
      //@}
      /// Method called by the managed pointer when destructed
      static void deleter(BddManager *ptr) {
        delete ptr;
      }
      int references(void) {
    	 return Cudd_CheckZeroRef(factory_.getManager());
      }
    public:
      // It does not make sens to copy the manager or to construct it from another one.
      // therefore we declared the copy constructor and the assignment as deleted.
      BddManager& operator = (const BddManager&) = delete;
      BddManager(const BddManager&) = delete; 
      /// Returns an instance of the manager
      static PManager instance(void) {
        if (_instance.get() != NULL)
	  return _instance;
	create();
	return _instance;
      }
      /// \name Access
      //@{
      /// Returns the factory
      Cudd* getFactory(void) {
	return &factory_;
      }
      /// Returns the bdd manager
      DdManager* manager(void) {
	return factory_.getManager();
      }
      /// Returns the constant \c true
      BDD one(void) {
	return factory_.bddOne();
      }
      /// Returns the constant \c false
      BDD zero(void) {
	     return factory_.bddZero();
      }
      /// Prints manager statistics to \a os
      void stats(std::ostream& os) const {
	
	auto usedMemory = Cudd_ReadMemoryInUse(factory_.getManager());
	//    os << "\tMemory in use: " <<  (usedMemory / 1024)  << " KB" << std::endl;
	auto gcTime = Cudd_ReadGarbageCollectionTime(factory_.getManager());
	//os << "\tGC time: " <<  gcTime  << " ms." << std::endl;
	auto gcs =  Cudd_ReadGarbageCollections(factory_.getManager());
	//os << "\tGC triggered " << gcs << " times" << std::endl;
	{
	  os << std::setiosflags(std::ios::left)
             << std::setw(14) << "Memory (MB)" 
             << std::setw(14) << "GC (s.)"
             << std::setw(14) << "GC steps" 
             << std::endl;
                      
          os << std::setw(14) << (usedMemory / 1048576.0) 
             << std::setw(14) << (gcTime / 1000.0) 
             << std::setw(14) << gcs 
             << std::endl;
        }
      }
      //@}
    };

    /**
     * \brief Returns the factory used for all the bdds
     * \ingroup DomRepr
     */
    inline 
    Cudd* factoryPtr(void) {
      return BddManager::instance()->getFactory();
    }
    inline
    Cudd& factory(void) {
      return *factoryPtr();
    }
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
    BDD one(void) {
      return BddManager::instance()->one();
    }
    /**
     * \brief Returns logical false
     * \ingroup DomRepr
     */
    inline
    BDD zero(void) {
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
