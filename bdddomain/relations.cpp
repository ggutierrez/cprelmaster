#include <bdddomain/relations.hh>

#include <bdddomain/encoding.hh>

using namespace std;

namespace MPG { namespace CPRel { namespace VarImpl {

RelationImpl create_equalXY(void) {
  vector<DdNode*> c0 = bddVars(0);
  vector<DdNode*> c1 = bddVars(1);

  return RelationImpl::create_fromBdd(
        Cudd_Xeqy(dd(),bitsPerInteger(),&c0[0],&c1[0]),2
        );
}

RelationImpl create_greaterXY(void) {
  vector<DdNode*> c0 = bddVars(0);
  vector<DdNode*> c1 = bddVars(1);

  return RelationImpl::create_fromBdd(
        Cudd_Xgty(dd(),bitsPerInteger(),NULL,&c0[0],&c1[0]),2
        );
}
}}}
