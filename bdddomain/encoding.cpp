#include <bdddomain/encoding.hh>

namespace MPG { namespace CPRel { namespace VarImpl {

using std::pair;
using std::vector;

vector<int> bddIndices(int c) {
  vector<int> vars;
  vars.reserve(bitsPerInteger());

  for (int i = (1 << bbv()); i--;) {
    vars.push_back((i << ba())+c);
  }
  return vars;
}

vector<DdNode*> bddVars(int c) {
  vector<DdNode*> vars;
  vars.reserve(bitsPerInteger());

  for (int i = bitsPerInteger(); i--;) {
    int var = (i << ba())+c;
    vars.push_back(Cudd_bddIthVar(dd(),var));
  }
  return vars;
}

DdNode* swap_columns(DdNode *r, const PermDescriptor& swapDesc) {
  vector<DdNode*> orig, perm;
  int descSize = static_cast<int>(swapDesc.size());
  orig.reserve(descSize * bitsPerInteger());
  perm.reserve(descSize * bitsPerInteger());

  typedef vector<pair<int,int> >::const_iterator It;
  vector<DdNode*> tempOrig, tempPerm;
  tempOrig.reserve(bitsPerInteger());
  tempPerm.reserve(bitsPerInteger());
  for (DescIterator i(swapDesc); i(); ++i) {
    tempOrig = bddVars(i.val().first);
    tempPerm = bddVars(i.val().second);
    std::copy(tempOrig.begin(), tempOrig.end(), back_inserter(orig));
    std::copy(tempPerm.begin(), tempPerm.end(), back_inserter(perm));
    tempOrig.clear();
    tempPerm.clear();
  }

  assert(static_cast<int>(orig.size()) == descSize * bitsPerInteger() &&
         static_cast<int>(perm.size()) == descSize * bitsPerInteger() &&
         "size of the bddVar vector is not correct");

  return Cudd_bddSwapVariables(dd(),r,&orig[0],&perm[0],orig.size());
}

DdNode* exists(DdNode* r, int c) {
  std::vector<int> indices = bddIndices(c);
  DdNode *cube = Cudd_IndicesToCube(dd(),&indices[0],indices.size());
  Cudd_Ref(cube);
  DdNode *q = Cudd_bddExistAbstract(dd(),r,cube);
  Cudd_RecursiveDeref(dd(),cube);
  return q;
}


}}}
