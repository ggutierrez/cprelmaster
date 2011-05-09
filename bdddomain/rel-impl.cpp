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
  for (Tuple::iterator i = tuple.begin(); i != tuple.end(); ++i) {
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
  Cudd_RecursiveDeref(dd(),bdd_);
  bdd_ = right.bdd_;
  arity_ = right.arity_;
  Cudd_Ref(bdd_);
  return *this;
}

RelationImpl::~RelationImpl(void) {
  Cudd_RecursiveDeref(dd(),bdd_);
}

int RelationImpl::arity(void) const {
  return arity_;
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

void RelationImpl::complement(void) {
  bdd_ = Cudd_Not(bdd_);
}
}}


