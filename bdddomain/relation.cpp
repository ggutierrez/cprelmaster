//#include <algorithm>
//#include <iostream>
//#include <vector>
//#include <cuddCdd.h>
//#include <cudd/cuddInt.h>
//#include <bdddomain/relation.hh>

//using std::cout;
//using std::cerr;
//using std::endl;
//using std::vector;
//using std::pair;
//using std::make_pair;

//namespace BDDImpl {



//DdNode* encode(int p, int a) {
//  DdNode *f = BDDConfig::one;
//  DdNode *v, *tmp;
//  Cudd_Ref(f);
//  for (int i = (1 << BDDConfig::BBV); i--;) {
//    v = Cudd_bddIthVar(BDDConfig::dd,(i << BDDConfig::BA)+a);
//    tmp = Cudd_bddAnd(BDDConfig::dd, p&1 ? v : Cudd_Not(v),f);
//    Cudd_Ref(tmp);
//    Cudd_RecursiveDeref(BDDConfig::dd,f);
//    f = tmp;
//    p >>= 1;
//  }
//  return f;
//}
//DdNode* encode(const vector<int>& tuple) {
//  DdNode *f = BDDConfig::one;
//  DdNode *t, *tmp;
//  Cudd_Ref(f);
//  for (unsigned int i = 0; i < tuple.size(); i++) {
//    t = encode(tuple[i],i);
//    tmp = Cudd_bddAnd(BDDConfig::dd,f,t);
//    Cudd_Ref(tmp);
//    Cudd_RecursiveDeref(BDDConfig::dd,t);
//    Cudd_RecursiveDeref(BDDConfig::dd,f);
//    f = tmp;
//  }
//  return f;
//}

//void BddIter::remove(const vector<int>& tuple) {
//  DdNode *i = Cudd_Not(encode(tuple));
//  DdNode *tmp = Cudd_bddAnd(BDDConfig::dd,relation_,i);
//  Cudd_Ref(tmp);
//  Cudd_RecursiveDeref(BDDConfig::dd,i);
//  Cudd_RecursiveDeref(BDDConfig::dd,relation_);
//  relation_ = tmp;
//}

//BddIter::BddIter(DdNode *relation, int arity)
//  : relation_(relation), arity_(arity) {
//  assert(relation != NULL);
//  if (relation_ == BDDConfig::one ||
//      relation_ == BDDConfig::zero) {
//    cout << "Creating iterator on empty or full" << endl;
//  } else {
//    Cudd_Ref(relation_);
//  }
//}

//BddIter::BddIter(const BddIter& i)
//  : relation_(i.relation_), arity_(i.arity_) {}

//BddIter::~BddIter(void) {
//  assert(relation_ != NULL);
//  Cudd_RecursiveDeref(BDDConfig::dd, relation_);
//}
//bool BddIter::operator()(void) const {
//  return relation_ != BDDConfig::zero;
//}

//vector<int> BddIter::val(void) {

//  const int cube_size = 1<<(BDDConfig::BBV+BDDConfig::BA);
//  const int tuple_size = 1<<BDDConfig::BA;

//  int cube_[cube_size];
//  int *cube = cube_;
//  int tuple[tuple_size];
//  CUDD_VALUE_TYPE val;


//  for(int k = 0; k < tuple_size; k++) tuple[k] = 0;

//  DdGen* gen = Cudd_FirstCube(BDDConfig::dd,relation_,&cube,&val);
//  assert(gen != NULL);
//  for(int i = cube_size -1; i>=0; i--){
//    if( (i & (tuple_size-1))<arity_){
//      tuple[i&(tuple_size-1)] &=
//          ~(1<<((1<<BDDConfig::BBV)-1-(i>>BDDConfig::BA)));
//      tuple[i&(tuple_size-1)] |=
//          (cube[i]&1)<<((1<<BDDConfig::BBV)-1-(i>>BDDConfig::BA));
//    }
//  }
//  Cudd_GenFree(gen);

//  /// Prepare the output
//  vector<int> out;
//  out.resize(arity_);
//  copy(tuple,tuple+arity_,out.begin());

//  /// Affect the state of the iterator
//  remove(out);
//  /// return t
//  return out;
//}

//void BddIter::operator++(void) {
//  assert(false);
//}


//Bdd::Bdd(DdNode *n) : node_(n) {
//  if (node_ != NULL) Cudd_Ref(node_);
//}

//Bdd::Bdd(void) : node_(NULL) {}

//Bdd::Bdd(const Bdd& r) : node_(r.node_) {
//  if (node_ != NULL) Cudd_Ref(node_);
//}

//Bdd::Bdd(int i, bool b) {
//  if (b) {
//    node_ = Cudd_bddIthVar(BDDConfig::dd,i);
//  } else {
//    node_ = Cudd_Not(Cudd_bddIthVar(BDDConfig::dd,i));
//  }
//  assert(node_ != NULL);
//  Cudd_Ref(node_);
//}

//Bdd::~Bdd(void) {
//  if (node_ != NULL)
//    Cudd_RecursiveDeref(BDDConfig::dd, node_);
//}

//bool Bdd::eq(const Bdd& r) const { return node_ == r.node_; }

//bool Bdd::diff(const Bdd& r) const { return !eq(r); }

//Bdd Bdd::complement(void) const {
//  return Bdd(Cudd_Not(node_));
//}

//Bdd& Bdd::operator = (const Bdd& right) {
//  if (this == &right) return *this;
//  if (right.node_ != NULL) Cudd_Ref(right.node_);
//  if (node_ != NULL) Cudd_RecursiveDeref(BDDConfig::dd,node_);
//  node_ = right.node_;
//  return *this;
//}

//void Bdd::intersectAssign(const Bdd& r) {
//  Bdd tmp = intersect(r);
//  this->operator=(tmp);
//}

//Bdd Bdd::intersect(const Bdd& r) const {
//  DdNode *i = Cudd_bddAnd(BDDConfig::dd,node_,r.node_);
//  assert(i);
//  return Bdd(i);
//}
//Bdd Bdd::Union(const Bdd& r) const {
//  DdNode *i = Cudd_bddOr(BDDConfig::dd,node_,r.node_);
//  assert(i);
//  return Bdd(i);
//}

//void Bdd::unionAssign(const Bdd& r) {
//  Bdd tmp = Union(r);
//  this->operator=(tmp);
//}

//Bdd Bdd::difference(const Bdd& r) const {
//  return intersect(r.complement());
//}

//void Bdd::differenceAssign(const Bdd& r) {
//  Bdd tmp = difference(r);
//  this->operator=(tmp);
//}

//int Bdd::minterms(int a) const {
//  return
//      Cudd_CountMinterm(BDDConfig::dd,node_,a<<BDDConfig::BBV);
//}

//BddIter Bdd::tuples(int a) const {
//  return BddIter(node_,a);
//}

//void Bdd::print(ostream& os, int a) const {
//  if (node_ == CDD_ONE(BDDConfig::dd)) {
//    os << "Universe";
//    return;
//  }
//  if (node_ == CDD_ZERO(BDDConfig::dd)) {
//    os << "Empty";
//    return;
//  }

//  const int cube_size = (1<<(BDDConfig::BBV+BDDConfig::BA));


//  DdGen* gen;
//  int cube_[cube_size];
//  int *cube = cube_;

//  int tuple[1<<BDDConfig::BA];
//  CUDD_VALUE_TYPE val;

//  os << "#(" << minterms(a) << "){";

//  for(int k = 0; k < 1<<BDDConfig::BA; k++) tuple[k]=0;

//  Cudd_ForeachCube(BDDConfig::dd,node_,gen,cube,val) {
//    int done = 0;
//    while(!done){
//      done=1;
//      for(int i = cube_size - 1; i >= 0; i--){
//        if((i&((1<<BDDConfig::BA)-1))<a){
//          tuple[i&((1<<BDDConfig::BA)-1)] &=
//              ~(1<<((1<<BDDConfig::BBV)-1-(i>>BDDConfig::BA)));
//          tuple[i&((1<<BDDConfig::BA)-1)] |=
//              (cube[i]&1)<<((1<<BDDConfig::BBV)-1-(i>>BDDConfig::BA));
//          if((cube[i]&2)&&done){
//            done&=cube[i]&1;
//            cube[i]^=1;
//          }
//        }
//      }
//      os << "<";
//      for(int j = 0; j < a; j++) {
//        os  << tuple[j] << ",";
//      }
//      os << ">, ";
//    }
//  }
//  os << "}";
//}

//Bdd Bdd::one = Bdd(BDDConfig::one);

//Bdd Bdd::zero = Bdd(BDDConfig::zero);

///**
//   * \brief Path creation in BDD
//   *
//   * Creates a BDD to represent \a p (encoded as sequence of bits)
//   * for a column \a a.
//   */
//Bdd createPath(int p, int a) {
//  Bdd f(Bdd::one);
//  for (int i = (1 << BDDConfig::BBV); i--;) {
//    f.intersectAssign(Bdd((i << BDDConfig::BA)+a, p&1));
//    p >>= 1;
//  }
//  return f;
//}

//Bdd GRelation::addPair(const pair<int,int>& p) const {
//  Bdd r(Bdd::one);
//  r.intersectAssign(createPath(p.first,0));
//  r.intersectAssign(createPath(p.second,1));
//  return r;
//}

//GRelation::GRelation(Bdd& representation, int arity, int maximum)
//  : representation_(representation), arity_(arity)
//  , maximum_(maximum){}

//GRelation::GRelation(int arity,const vector<pair<int,int> >& data)
//  : representation_(Bdd::zero), arity_(arity)
//{
//  int max = 0;
//  for (unsigned int i = 0; i < data.size(); i++) {
//    assert(data[i].first >= 0 && data[i].second >= 0);
//    representation_.unionAssign(addPair(data[i]));
//    if (data[i].first > max) max = data[i].first;
//    if (data[i].second > max) max = data[i].second;
//  }
//  maximum_ = max;
//}

//GRelation::GRelation(int arity, int maximum)
//  : representation_(Bdd::zero), arity_(arity), maximum_(maximum)
//{
//  // TODO: for now only binary (arity = 2) is supported
//  assert(arity == 2);
//  pair<int,int> x;
//  for (int i = 0; i <= maximum_; i++)
//    for (int j = 0; j <= maximum_; j++) {
//      x = make_pair(i,j);
//      addPair(x);
//    }
//}

//GRelation::GRelation(const GRelation& r)
//  : representation_(r.representation_), arity_(r.arity_)
//  , maximum_(r.maximum_){}

//GRelation::~GRelation(void) {}

//BddIter GRelation::tuples(void) {
//  return representation_.tuples(arity_);
//}

//bool GRelation::empty(void) const {
//  return representation_.eq(Bdd::zero);
//}

//bool GRelation::full(void) const {
//  return representation_.eq(Bdd::one);
//}

//GRelation GRelation::complement(void) const {
//  Bdd c = representation_.complement();
//  return GRelation(c,arity_,maximum_);
//}

//GRelation GRelation::intersect(const GRelation& r) const {
//  assert(arity_ == r.arity_);
//  assert(maximum_ == r.maximum_);
//  Bdd t(representation_.intersect(r.representation_));
//  return
//      GRelation(t,arity_,maximum_);
//}

//bool GRelation::in(const pair<int,int>& t) const {
//  Bdd r(addPair(t));
//  Bdd inter(r.intersect(representation_));
//  return !inter.eq(Bdd::zero);
//}

//void GRelation::print(ostream& os) const {
//  representation_.print(os,arity_);
//}

//}

