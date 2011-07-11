#include <iostream>
#include <bdddomain/encoding.hh>
#include <bdddomain/rel-impl.hh>

using std::pair;
using std::vector;

namespace MPG { namespace CPRel { namespace VarImpl {

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

RelationImpl RelationImpl::create_fromBdd(DdNode* b, int a) {
  return RelationImpl(b,a);
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
  DdNode *et = t.getBDD();
  Cudd_Ref(et);
  DdNode *tmp = Cudd_bddOr(dd(),bdd_,et);
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd(),et);
  Cudd_RecursiveDeref(dd(),bdd_);
  bdd_ = tmp;
}

void RelationImpl::remove(const Tuple& t) {
  assert(arity_ == t.arity());
  DdNode *et = t.getBDD();
  Cudd_Ref(et);
  DdNode *tmp = Cudd_bddAnd(dd(),bdd_,Cudd_Not(et));
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd(),et);
  Cudd_RecursiveDeref(dd(),bdd_);
  bdd_ = tmp;
}

double RelationImpl::cardinality(void) const {
  return Cudd_CountMinterm(dd(),bdd_,arity_ << Limits::bbv);
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
  return bdd_ == r.bdd_;
}

void RelationImpl::complement(void) {
  bdd_ = Cudd_Not(bdd_);
}

RelationImpl RelationImpl::permute(const PermDescriptor& permDesc) const {
  // No error detection on permDesc is done here. The reason is because under
  // some circumstances this method is used to move columns in the representation
  // to places that are outside of the arity of the relation. For instance,
  // this \times U^n is implemented by doing the shift of n columns to the right.

  RelationImpl ret(VarImpl::swap_columns(bdd_,permDesc),arity_);
  assert(ret.cardinality() == cardinality());
  assert(ret.arity() == arity());
  return ret;
}

RelationImpl RelationImpl::shiftLeft(int n) const {
  if (n == 0) return RelationImpl(*this);
  return RelationImpl(VarImpl::shiftLeft(bdd_,arity_,n),arity_+n);
}

RelationImpl RelationImpl::shiftRight(int n) const {
  if (n == 0) return RelationImpl(*this);
  if (n == arity_) return RelationImpl(0);
  return RelationImpl(VarImpl::shiftRight(bdd_,arity_,n),arity_-n);
}

RelationImpl RelationImpl::timesU(int n, bool left) const {
  if (left)
    return RelationImpl(bdd_,arity_+n);

  PermDescriptor d;
  for (int i = 0; i < arity_; i++) d.permute(i,i+n);
  RelationImpl r = permute(d);
  return RelationImpl(r.bdd_,arity_+n);
}

RelationImpl RelationImpl::join(int j, const RelationImpl& r) const {
  const RelationImpl& l = *this;

//  std::cout << "Join input: this " << arity() << " -- " << cardinality() << std::endl;
//  std::cout << "****Join input: r " << r.bdd_ << "  " << r.arity() << " -- " << r.cardinality() << std::endl;

  assert(l.arity() >= j && r.arity() >= j
         && "There are not enough columns for the join");

  RelationImpl lxu = l.timesU(r.arity() - j,false);
  RelationImpl rxu = r.timesU(l.arity() - j,true);

  lxu.intersect(rxu);

//  std::cout << "Join result: lxu " << lxu.arity() << " -- " << lxu.cardinality() << std::endl;

  return lxu;
}

RelationImpl RelationImpl::exists(int c) const {
 return RelationImpl(VarImpl::exists(c, bdd_), arity_);
}

RelationImpl RelationImpl::unique(int c) const {
 return RelationImpl(VarImpl::unique(c, bdd_), arity_);
}

RelationImpl RelationImpl::unique(const std::vector<int>& c) const {
  return RelationImpl(VarImpl::unique(c, bdd_), arity_);
}

RelationImpl RelationImpl::projectBut(int c) const {
  RelationImpl q(exists(c));

  PermDescriptor d;
  for (int i = arity_ -1; i > c; i--) {
    d.permute(i,i-1);
  }
  RelationImpl perm(q.permute(d));

  RelationImpl ret(perm.bdd_,arity_-1);

  assert(ret.cardinality() == cardinality());
  assert(ret.arity() == arity()-1);

  return ret;
}

RelationImpl RelationImpl::project(int p) const {
  RelationImpl r(*this);
  assert(r.cardinality() == cardinality() && "Something bad happend at CC");
  assert(r.arity() == arity() && "Something bad happend at CC");

  if (p == arity_) return r;

  RelationImpl result(VarImpl::exists(arity()-1,p+1,r.bdd_),p);

//  for (int i = p; i < arity_; i++) {
//    RelationImpl tmp = r.projectBut(i);
//    r = tmp;
//  }
//  assert(r.arity() == arity() && "Error performing projection");
  return result;
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
/*
  const int cube_size = 1<<(Limits::bbv + Limits::ba);
  const int tuple_size = 1 << Limits::ba;

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
          ~(1<<((1<<Limits::bbv)-1-(i>>Limits::ba)));
      tuple[i&(tuple_size-1)] |=
          (cube[i]&1)<<((1<<Limits::bbv)-1-(i>>Limits::ba));
    }
  }
  Cudd_GenFree(gen);

  // Prepare the output
  vector<int> v;
  for (int i = 0; i < arity_; i++) v.push_back(tuple[arity_-1-i]);
  Tuple out(v);
  // Affect the state of the iterator
  remove(out);

  return out;
*/
  const int cube_size = 1<<(Limits::bbv + Limits::ba);

  int cube_[cube_size];
  int *cube = cube_;
  CUDD_VALUE_TYPE val;

  DdGen* gen = Cudd_FirstCube(dd(),relation_,&cube,&val);
  assert(gen != NULL);
  vector<int> v = decodeCube(cube,arity_);
  Cudd_GenFree(gen);

  // Prepare the output
  Tuple out(v);
  // Affect the state of the iterator
  remove(out);

  return out;

}

void RelationImplIter::remove(const Tuple& t) {
  DdNode *i = t.getBDD();
  Cudd_Ref(i);
  DdNode *tmp = Cudd_bddAnd(dd(),relation_,Cudd_Not(i));
  Cudd_Ref(tmp);
  Cudd_RecursiveDeref(dd(),i);
  Cudd_RecursiveDeref(dd(),relation_);
  relation_ = tmp;
}

}}}


