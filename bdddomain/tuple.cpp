#include <bdddomain/tuple.hh>
#include <vector>

using std::vector;

namespace MPG { namespace CPRel {
using namespace VarImpl;

Tuple::Tuple(const std::vector<int>& v)
  : data_(v), arity_(v.size()) {}

Tuple::Tuple(const Tuple& t)
  : data_(t.data_), arity_(t.arity_) {}

Tuple::~Tuple(void) {}

DdNode* Tuple::encode(int p, int a) {
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

DdNode* Tuple::encode(void) const {
  DdNode *f = one();
  DdNode *t, *tmp;
  Cudd_Ref(f);
  int c = data_.size()-1;
  for (unsigned int i = 0; i < data_.size(); i++) {
    t = encode(data_.at(i),c);
    tmp = Cudd_bddAnd(dd(),f,t);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(dd(),t);
    Cudd_RecursiveDeref(dd(),f);
    f = tmp;
    c--;
  }
  return f;
}

vector<int> Tuple::value(void) const {
  return vector<int>(data_);
}

}}
