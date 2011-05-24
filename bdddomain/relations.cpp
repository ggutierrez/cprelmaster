#include <bdddomain/relations.hh>

#include <bdddomain/rel-impl.hh>

using namespace std;

namespace MPG { namespace CPRel { namespace VarImpl {

DdNode* create_equalXY(void) {
  vector<DdNode*> c0 = bddVars(0);
  vector<DdNode*> c1 = bddVars(1);

  DdNode *f = Cudd_Xeqy(dd(),bitsPerInteger(),&c0[0],&c1[0]);

  //  RelationImpl r = RelationImpl::create_fromBdd(f,2);
  return f;
}

DdNode* create_greaterXY(void) {
  vector<DdNode*> c0 = bddVars(0);
  vector<DdNode*> c1 = bddVars(1);

  vector<int> n = bddIndices(1);
  for (int i = 0; i < n.size();i++) {
    std::cout << "Index: " << i << " " << n[i] << std::endl;
  }

  DdNode *f = Cudd_Xeqy(dd(),bitsPerInteger(),&c0[0],&c1[0]);

  RelationImpl r = RelationImpl::create_fromBdd(f,2);
  std::cout << "The relation: " << r << std::endl;
  return f;
}

}}}
