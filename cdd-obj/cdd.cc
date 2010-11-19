#include <cdd.hh>

namespace Cdd {
  /*
   * BDD
   */
  BDD::BDD(void) {}
  
  BDD::~BDD(void) {};
  /*
   * CDD
   */
  CDD::CDD(void) {
    c = CDD_ONE(manager);
    Cudd_Ref(c);
  }

  CDD::~CDD(void) {
    Cudd_RecursiveDeref(manager,c);
#ifndef NDEBUG 
    std::cout << "Current references: " << references() << std::endl;
#endif   
  }

#ifndef NDEBUG 
  int CDD::references(void) {
    return Cudd_CheckZeroRef(manager);
  }
#endif
  /*
   * Relation 
   */
  Relation::Relation(unsigned int a) : arity(a) {}
}
