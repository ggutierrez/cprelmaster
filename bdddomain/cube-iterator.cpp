#include <bdddomain/cube-iterator.hh>

namespace MPG { namespace CPRel { namespace VarImpl {
CubeIterator::CubeIterator(DdNode *rel, int a)
  : relation_(rel), arity_(a){
  Cudd_Ref(relation_);
}

CubeIterator::CubeIterator(const CubeIterator& it)
  : relation_(it.relation_), arity_(it.arity_){
  Cudd_Ref(relation_);
}

CubeIterator::~CubeIterator(void) {
  Cudd_RecursiveDeref(dd(),relation_);
}

bool CubeIterator::operator ()(void) const {
  return relation_ != zero();
}

std::vector<int> CubeIterator::val(void) {
//  const int cube_size = 1<<(bbv() + ba());
//  const int tuple_size = 1 << ba();

//  int cube_[cube_size];
//  int *cube = cube_;
//  int tuple[tuple_size];
//  CUDD_VALUE_TYPE val;

//  for(int k = 0; k < tuple_size; k++) tuple[k] = 0;

//  DdGen* gen = Cudd_FirstCube(dd(),relation_,&cube,&val);
//  assert(gen != NULL);
//  for(int i = cube_size -1; i>=0; i--){
//    if( (i & (tuple_size-1))<arity_){
//      tuple[i&(tuple_size-1)] &=
//          ~(1<<((1<<bbv())-1-(i>>ba())));
//      tuple[i&(tuple_size-1)] |=
//          (cube[i]&1)<<((1<<bbv())-1-(i>>ba()));
//    }
//  }
//  Cudd_GenFree(gen);

//  // Prepare the output
//  Tuple out(arity_);
//  for (int i = 0; i < arity_; i++) out[i] = tuple[i];

//  // Affect the state of the iterator
//  remove(out);

//  return out;
}

void CubeIterator::remove(const Tuple& t) {
//  DdNode *i = Cudd_Not(encode(t));
//  DdNode *tmp = Cudd_bddAnd(dd(),relation_,i);
//  Cudd_Ref(tmp);
//  Cudd_RecursiveDeref(dd(),i);
//  Cudd_RecursiveDeref(dd(),relation_);
//  relation_ = tmp;
}
}}}
