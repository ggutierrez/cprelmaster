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

void Tuple::decodeCube(int* cube, std::vector<int>& tuple) const {
  const int cubeSize = 1<<(Limits::bbv + Limits::ba);
  const int maxTupleSize = 1 << Limits::ba;

  std::cout << "The size of a cube: " << cubeSize << std::endl;

  for (int i = 0; i < cubeSize; i++) {
    // maxTupleSize -1 in binary is equivalent to ones at every place
    // as i is ranging from 0 to cubeSize there are values that are not
    // interesting for a tuple of arity_ elements. This conditional ensures that
    // only meaningful possitions in the cube are taken into account
    if ((i & (maxTupleSize - 1)) < arity_) {
      std::cout << cube[i] << " ";

    }
  }
  std::cout << std::endl;
}

vector<int> Tuple::value(void) const {
  const int cubeSize = 1<<(Limits::bbv + Limits::ba);
  int cube_[cubeSize];
  int *cube = cube_;
  CUDD_VALUE_TYPE val;

  DdGen* gen = Cudd_FirstCube(dd(),data_,&cube,&val);
  assert(gen != NULL);

  vector<int> tuple;
  decodeCube(cube,tuple);

  return tuple;

}

vector<int> Tuple::value2(void) const {

  const int cube_size = 1<<(Limits::bbv + Limits::ba);
  const int tuple_size = 1 << Limits::ba;

  int cube_[cube_size];
  int *cube = cube_;
  int tuple[tuple_size];
  CUDD_VALUE_TYPE val;

  for(int k = 0; k < tuple_size; k++) tuple[k] = 0;

  DdGen* gen = Cudd_FirstCube(dd(),data_,&cube,&val);
  assert(gen != NULL);
  for(int i = cube_size -1; i>=0; i--){
    if( (i & (tuple_size-1)) < arity_){
      tuple[i&(tuple_size-1)] &=
          ~(1<<((1<<Limits::bbv)-1-(i>>Limits::ba)));
      tuple[i&(tuple_size-1)] |=
          (cube[i]&1)<<((1<<Limits::bbv)-1-(i>>Limits::ba));
    }
  }
  Cudd_GenFree(gen);

  // Prepare the output
  vector<int> v;
  for (int i = 0; i < arity_; i++) v.push_back(tuple[arity_-1-i]);
  return v;
}

}}
