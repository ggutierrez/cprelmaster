#include <bdddomain/relations.hh>

#include <bdddomain/rel-impl.hh>

using namespace std;

namespace MPG { namespace CPRel { namespace VarImpl {

DdNode* create_equalXY(void) {
  vector<DdNode*> c0 = bddVars(0);
  vector<DdNode*> c1 = bddVars(1);

  return Cudd_Xeqy(dd(),bitsPerInteger(),&c0[0],&c1[0]);
}

DdNode* create_greaterXY(void) {
  vector<DdNode*> c0 = bddVars(0);
  vector<DdNode*> c1 = bddVars(1);

  return Cudd_Xgty(dd(),bitsPerInteger(),NULL,&c0[0],&c1[0]);
}
}}}
