#include <bdddomain/tuple.hh>
#include <vector>

using std::vector;

namespace MPG { namespace CPRel {
using namespace VarImpl;

Tuple::Tuple(const std::vector<int>& v)
  : data_(encode(v)), arity_(v.size()) {
  //Cudd_Ref(data_);
}

Tuple::Tuple(const Tuple& t)
  : data_(t.data_), arity_(t.arity_) {
  Cudd_Ref(data_);
}

Tuple::~Tuple(void) {
  Cudd_RecursiveDeref(dd(),data_);
}

Tuple& Tuple::operator = (const Tuple& t) {
  if (this == &t) return *this;
  assert(false && "FIXME!!!");
  std::cerr << t.arity();
  arity_ = t.arity_;
  Cudd_RecursiveDeref(dd(),data_);
  data_ = t.data_;
  Cudd_Ref(data_);
  return *this;
}

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

DdNode* Tuple::encode(const std::vector<int>& v) const {
  DdNode *f = one();
  DdNode *t, *tmp;
  Cudd_Ref(f);
  int c = v.size()-1;
  for (unsigned int i = 0; i < v.size(); i++) {
    t = encode(v.at(i),c);
    tmp = Cudd_bddAnd(dd(),f,t);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(dd(),t);
    Cudd_RecursiveDeref(dd(),f);
    f = tmp;
    c--;
  }
  return f;
}

DdNode* Tuple::encode(void) const {
  Cudd_Ref(data_);
  return data_;
}

vector<int> Tuple::value(void) const {
  const int cube_size = cubeSize();

  std::vector<int> cube_(cube_size,0);
  int *cube = &cube_[0];
  CUDD_VALUE_TYPE value;

  const int tuple_size = arity_;
  std::vector<int> tuple(arity_,0);
  DdGen *gen = Cudd_FirstCube(dd(),data_,&cube,&value);
  assert(gen != NULL && "Invalid cube in generator");

  for(int i = cube_size - 1; i>=0; i--){
    int current = i & (tuple_size-1) ;
    if(current < arity_) {
      // we are only interested in the elements of the cube that correspond to
      // the current tuple. Therfore, we only read from the first element up to
      // the last element that falls in the tuple according with its cardinality.
      int to_and = (bitsPerInteger()-1-(i>>ba()));
      tuple[current] &= ~(1 << to_and);
      tuple[current] |= (cube[i]&1) << to_and;
    }
  }
  Cudd_GenFree(gen);
  return tuple;
}

}}
