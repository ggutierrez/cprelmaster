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

DdNode* swap_columns(DdNode *r, int x, int y) {
  vector<DdNode*> vx(bddVars(x));
  vector<DdNode*> vy(bddVars(y));

  return Cudd_bddSwapVariables(dd(),r,&vx[0],&vy[0],vx.size());
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

DdNode* encode(int p, int a) {
  DdNode *f = one();
  DdNode *v, *tmp;
  Cudd_Ref(f);
  for (int i = bitsPerInteger(); i--;) {
    v = Cudd_bddIthVar(dd(),(i << ba())+a);
    tmp = Cudd_bddAnd(dd(), p&1 ? v : Cudd_Not(v),f);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(dd(),f);
    f = tmp;
    p >>= 1;
  }
  return f;
}

DdNode* encode(const Tuple& tuple) {
  DdNode *f = one();
  DdNode *t, *tmp;
  Cudd_Ref(f);
  int c = tuple.arity()-1;
  for (int i = 0; i < tuple.arity(); i++) {
    t = encode(tuple.at(i),c);
    tmp = Cudd_bddAnd(dd(),f,t);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(dd(),t);
    Cudd_RecursiveDeref(dd(),f);
    f = tmp;
    c--;
  }
  return f;
}
}}}
