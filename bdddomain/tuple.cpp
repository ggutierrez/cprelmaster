#include <bdddomain/tuple.hh>
#include <bdddomain/encoding.hh>
#include <vector>

using std::vector;

namespace MPG { namespace CPRel {
using namespace VarImpl;

Tuple::Tuple(const std::vector<int>& v)
  : data_(encode(v)), arity_(v.size()) {
  // The representation from "encode" is already referenced.
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
Tuple::Tuple(const std::initializer_list<int> l)
  : arity_(l.size()) {
  /** \todo This is not as efficient as it can be. The best thing is for encode
   * to get two iterators and this will avoid copying to a temporal vector
   */
  std::vector<int> v(l.begin(),l.end());
  // The representation from "encode" is already referenced.
  data_ = encode(v);
}
#endif

Tuple::Tuple(const Tuple& t)
  : data_(t.data_), arity_(t.arity_) {
  Cudd_Ref(data_);
}

Tuple::~Tuple(void) {
  Cudd_RecursiveDeref(dd(),data_);
}

Tuple& Tuple::operator = (const Tuple& t) {
  if (this == &t) return *this;
  /// \todo FIXME!
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
  for (int i = Limits::bitsPerInteger; i--;) {
    v = Cudd_bddIthVar(dd(),(i << Limits::ba)+a);
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

DdNode* Tuple::getBDD(void) const {
  return data_;
}

vector<int> Tuple::value(void) const {
  const int cubeSize = 1<<(Limits::bbv + Limits::ba);
  int cube_[cubeSize];
  int *cube = cube_;
  CUDD_VALUE_TYPE val;

  DdGen* gen = Cudd_FirstCube(dd(),data_,&cube,&val);
  assert(gen != NULL);
  vector<int> tuple = decodeCube(cube,arity_);
  Cudd_GenFree(gen);

  return tuple;
}
}}
