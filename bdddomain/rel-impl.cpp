#include <iostream>
#include <memory>
#include <bdddomain/rel-impl.hh>


namespace CPRel { namespace BDDImpl {
PManager BddManager::_instance;

/// Function to return the manager
inline
DdManager* dd(void) {
  return BddManager::instance()->manager();
}

/// Function to return logical true
inline
DdNode* one(void) {
  return BddManager::instance()->one();
}

/// Function to return logical false
inline
DdNode* zero(void) {
  return BddManager::instance()->zero();
}

inline
int bbv(void) {
  return BddManager::instance()->bbv();
}

inline
int ba(void) {
  return BddManager::instance()->ba();
}

DdNode* encode(int p, int a) {
  DdNode *f = one();
  DdNode *v, *tmp;
  Cudd_Ref(f);
  for (int i = (1 << bbv()); i--;) {
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
  int c = 0;
  for (Tuple::iterator i = tuple.cbegin(); i != tuple.cend(); ++i) {
    t = encode(*i,c);
    tmp = Cudd_bddAnd(dd(),f,t);
    Cudd_Ref(tmp);
    Cudd_RecursiveDeref(dd(),t);
    Cudd_RecursiveDeref(dd(),f);
    f = tmp;
    c++;
  }
  return f;
}

RelationImpl::RelationImpl(DdNode *n, int a)
  : bdd_(n), arity_(a) {
  Cudd_Ref(bdd_);
}

RelationImpl::RelationImpl(int a)
  : bdd_(zero()), arity_(a) {
  Cudd_Ref(bdd_);
}

RelationImpl::RelationImpl(const RelationImpl &r)
  : bdd_(r.bdd_), arity_(r.arity_){
  Cudd_Ref(bdd_);
}

RelationImpl& RelationImpl::operator=(const RelationImpl& right) {
  if (this == &right) return *this;
  swap(right);
  return *this;
}

RelationImpl RelationImpl::create_full(int a) {
  return RelationImpl(one(),a);
}

void RelationImpl::swap(const RelationImpl& r) {
  Cudd_RecursiveDeref(dd(),bdd_);
  bdd_ = r.bdd_;
  arity_ = r.arity_;
  Cudd_Ref(bdd_);
}

RelationImpl::~RelationImpl(void) {
  Cudd_RecursiveDeref(dd(),bdd_);
}

int RelationImpl::arity(void) const {
  return arity_;
}

bool RelationImpl::empty(void) const {
  return bdd_ == zero();
}

bool RelationImpl::universe(void) const {
  return bdd_ == one();
}

void RelationImpl::add(const Tuple& t) {
  assert(arity_ == t.arity());
  DdNode *et = encode(t);
  DdNode *tmp = Cudd_bddOr(dd(),bdd_,et);
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd(),et);
  Cudd_RecursiveDeref(dd(),bdd_);
  bdd_ = tmp;
}

void RelationImpl::remove(const Tuple& t) {
  assert(arity_ == t.arity());
  DdNode *et = encode(t);
  DdNode *tmp = Cudd_bddAnd(dd(),bdd_,Cudd_Not(et));
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd(),et);
  Cudd_RecursiveDeref(dd(),bdd_);
  bdd_ = tmp;
}

double RelationImpl::cardinality(void) const {
  return Cudd_CountMinterm(dd(),bdd_,arity_ << bbv());
}

void RelationImpl::add(const RelationImpl& r) {
  assert(arity_ == r.arity_);
  DdNode *bddr = r.bdd_;
  DdNode *tmp = Cudd_bddOr(dd(),bdd_,bddr);
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd(),bdd_);
  bdd_ = tmp;
}

void RelationImpl::remove(const RelationImpl& r) {
  assert(arity_ == r.arity_);
  DdNode *bddr = r.bdd_;
  DdNode *tmp = Cudd_bddAnd(dd(),bdd_,Cudd_Not(bddr));
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd(),bdd_);
  bdd_ = tmp;
}

void RelationImpl::intersect(const RelationImpl& r) {
  assert(arity_ == r.arity_);
  DdNode *tmp = Cudd_bddAnd(dd(),bdd_,r.bdd_);
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd(),bdd_);
  bdd_=tmp;
}

bool RelationImpl::equal(const RelationImpl& r) const {
  if (arity_ != r.arity_) return false;
  return bdd_ != r.bdd_;
}

void RelationImpl::complement(void) {
  bdd_ = Cudd_Not(bdd_);
}

RelationImplIter RelationImpl::tuples(void) const {
  return RelationImplIter(bdd_,arity_);
}

RelationImplIter::RelationImplIter(DdNode *rel, int a)
  : relation_(rel), arity_(a){
  Cudd_Ref(relation_);
}

RelationImplIter::RelationImplIter(const RelationImplIter& it)
  : relation_(it.relation_), arity_(it.arity_){
  Cudd_Ref(relation_);
}

RelationImplIter::~RelationImplIter(void) {
  Cudd_RecursiveDeref(dd(),relation_);
}

bool RelationImplIter::operator ()(void) const {
  return relation_ != zero();
}

Tuple RelationImplIter::val(void) {
  const int cube_size = 1<<(bbv() + ba());
  const int tuple_size = 1 << ba();

  int cube_[cube_size];
  int *cube = cube_;
  int tuple[tuple_size];
  CUDD_VALUE_TYPE val;

  for(int k = 0; k < tuple_size; k++) tuple[k] = 0;

  DdGen* gen = Cudd_FirstCube(dd(),relation_,&cube,&val);
  assert(gen != NULL);
  for(int i = cube_size -1; i>=0; i--){
    if( (i & (tuple_size-1))<arity_){
      tuple[i&(tuple_size-1)] &=
          ~(1<<((1<<bbv())-1-(i>>ba())));
      tuple[i&(tuple_size-1)] |=
          (cube[i]&1)<<((1<<bbv())-1-(i>>ba()));
    }
  }
  Cudd_GenFree(gen);

  // Prepare the output
  Tuple out(arity_);
  for (int i = 0; i < arity_; i++) out[i] = tuple[i];

  // Affect the state of the iterator
  remove(out);

  return out;
}

void RelationImplIter::remove(const Tuple& t) {
  DdNode *i = Cudd_Not(encode(t));
  DdNode *tmp = Cudd_bddAnd(dd(),relation_,i);
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd(),i);
  Cudd_RecursiveDeref(dd(),relation_);
  relation_ = tmp;
}

}}


