#ifndef __CDD_HH__
#define __CDD__HH__

#include <iostream>
#include "cuddCDD.h" 

namespace Cdd {
  
  /// A global manager for all the BDDs and CDDs represented
  static DdManager * manager;
  /**
   * \brief BDD abstraction
   */
  class BDD {
  public:
    /// Constructor
    BDD(void);
    /// Destructor
    ~BDD(void);
  };

  /**
   * \brief CDD abstraction
   */
  class CDD : public BDD {
  protected:
    DdNode *c;
  public:
    /// Constant ONE
    /// Constant UNK
    /// Constant ZERO
    
    /// Constructor of CDD with terminal ONE
    CDD(void);
    /// Destructor
    virtual ~CDD(void);
#ifndef NDEBUG
    /// Return the number of referenced nodes in the manager
    static int references(void);
#endif
  };

  class Relation {
  private:
    /// Representation
    CDD r;
    /// Arity
    unsigned int arity;
  public:
    /// Constructor for an empty relation of arity \a a
    Relation(unsigned int a);
  };
}

#endif
