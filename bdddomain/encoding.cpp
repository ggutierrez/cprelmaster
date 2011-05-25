#include <bdddomain/encoding.hh>

namespace MPG { namespace CPRel { namespace VarImpl {

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
  // Get the indices of the variables used by columns x and y
  vector<int> ix(bddIndices(x));
  vector<int> iy(bddIndices(y));

  typedef vector<int>::iterator it;

  DdNode*  vx[ix.size()];
  int index = 0;
  for (it i = ix.begin(); i != ix.end(); ++i) {
    vx[index] = Cudd_bddIthVar(dd(),*i);
    index++;
  }
  DdNode*  vy[iy.size()];
  index = 0;
  for (it i = iy.begin(); i != iy.end(); ++i) {
    vy[index] = Cudd_bddIthVar(dd(),*i);
    index++;
  }

  DdNode **vx_ = vx;
  DdNode **vy_ = vy;
  return Cudd_bddSwapVariables(dd(),r,vx_,vy_,ix.size());
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
  for (Tuple::iterator i = tuple.cbegin(); i != tuple.cend(); ++i) {
    t = encode(*i,c);
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
