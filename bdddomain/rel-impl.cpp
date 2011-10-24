#include <iostream>
#include <bdddomain/encoding.hh>
#include <bdddomain/rel-impl.hh>
#include <bdddomain/bdd.hh>

using std::pair;
using std::vector;

namespace MPG { namespace VarImpl {

    RelationImpl::RelationImpl(BDD n, int a)
      : bdd_(n), arity_(a) {}

    RelationImpl::RelationImpl(int a)
      : bdd_(zero()), arity_(a) {}

    RelationImpl::RelationImpl(const RelationImpl &r)
      : bdd_(r.bdd_), arity_(r.arity_) {}

    RelationImpl& RelationImpl::operator=(const RelationImpl& right) {
      if (this == &right) return *this;
      swap(right);
      return *this;
    }

    RelationImpl RelationImpl::create_full(int a) {
      return RelationImpl(one(),a);
    }

    RelationImpl RelationImpl::create_fromBdd(BDD b, int a) {
      return RelationImpl(b,a);
    }

    void RelationImpl::swap(const RelationImpl& r) {
      bdd_ = r.bdd_;
      arity_ = r.arity_;
    }

    RelationImpl::~RelationImpl(void) {}

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
      BDD et = t.getBDD();
      bdd_ |= et;

    }

    void RelationImpl::remove(const Tuple& t) {
      assert(arity_ == t.arity());
      BDD et = t.getBDD();
      bdd_ &= !et;
    }

    double RelationImpl::cardinality(void) const {
      return bdd_.CountMinterm(arity_ << Limits::bbv);
    }

    void RelationImpl::add(const RelationImpl& r) {
      assert(arity_ == r.arity_);
      bdd_ |= r.bdd_;
    }

    void RelationImpl::remove(const RelationImpl& r) {
      assert(arity_ == r.arity_);
      bdd_ &= !r.bdd_;
    }

    void RelationImpl::intersect(const RelationImpl& r) {
      assert(arity_ == r.arity_);
      bdd_ &= r.bdd_;
    }

    bool RelationImpl::equal(const RelationImpl& r) const {
      if (arity_ != r.arity_) return false;
      return bdd_ == r.bdd_;
    }

    void RelationImpl::complement(void) {
      bdd_ = !bdd_;
    }

    /*
     * Relational algebra operations on relations
     */

    RelationImpl RelationImpl::unique(int c) const {
      return RelationImpl(VarImpl::unique(c, bdd_), arity_);
    }

    RelationImpl RelationImpl::unique(const std::vector<int>& c) const {
      return RelationImpl(VarImpl::unique(c, bdd_), arity_);
    }

    RelationImpl RelationImpl::exists(int c) const {
      return RelationImpl(VarImpl::exists(c, bdd_), arity_);
    }

    RelationImpl RelationImpl::exists(const std::vector<int>& c) const {
      return RelationImpl(VarImpl::exists(c, bdd_), arity_);
    }

    /*
     * Permutation
     */
    RelationImpl RelationImpl::replace(const std::vector<std::pair<int,int>>& pairing) {
      return RelationImpl(VarImpl::replace(pairing,bdd_), arity_);
    }

    RelationImpl RelationImpl::swap(const std::vector<std::pair<int,int>>& pairing) {
      return RelationImpl(VarImpl::swap(pairing,bdd_), arity_);
    }

    /*
     * Cross product
     */
    RelationImpl RelationImpl::timesULeft(int n) const {
      assert(arity_ + n <= Limits::arity &&
	     "Manager does not configure with the appropriate capacity to handle this operation.");
      return RelationImpl(bdd_, arity_ + n);
    }

    RelationImpl RelationImpl::timesURight(int n) const {
      assert(arity_ + n <= Limits::arity &&
	     "Manager does not configure with the appropriate capacity to handle this operation.");
      
      std::vector<std::pair<int,int>> replacement(Limits::arity);
      for (int i = 0; i < arity_; i++) {
	replacement[i] = {i,i+n};
      }
      for (int i = arity_; i < Limits::arity - n; i++) {
	replacement[i] = {i,i};
      }
      int j = 0;
      for (int i = Limits::arity - n; i < Limits::arity; i++, j++) {
	replacement[i] = {i, j};
      }
      
      return RelationImpl(VarImpl::replace(replacement,bdd_), arity_ + n);
    }

    // still for review
    RelationImpl RelationImpl::timesU(int n, bool left) const {
      if (left)
	return RelationImpl(bdd_,arity_+n);

      PermDescriptor d;
      for (int i = 0; i < arity_; i++) d.permute(i,i+n);
      RelationImpl r = permute(d);
      return RelationImpl(r.bdd_,arity_+n);
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


    RelationImpl RelationImpl::join(int j, const RelationImpl& right) const {
      const RelationImpl& left = *this;

      assert(left.arity() >= j && right.arity() >= j
	     && "There are not enough columns for the join");

      RelationImpl lxu = left.timesU(right.arity() - j,false);
      RelationImpl rxu = right.timesU(left.arity() - j,true);

      lxu.intersect(rxu);
      return lxu;
    }

    RelationImpl RelationImpl::follow(int f, const RelationImpl& right) const {
      const RelationImpl& left = *this;

      /// \todo handle errors on arity of the relations with exceptions

      //  assert(left.arity() >= f && right.arity() >= f
      //         && "There are not enough columns for the join");

      // 1) compute the join of the relations on f columns
      RelationImpl join = left.join(f,right);

      // 2) remove the "join" columns. These columns are in the range right.arity-1
      //    to right.arity-f
      int leftMost = right.arity() - 1;
      int rightMost = right.arity() - f;

      assert(leftMost >= rightMost && "Unexpected result for range of join columns");

      return RelationImpl(
			  VarImpl::discard(join.bdd_,join.arity(), leftMost, rightMost),
			  join.arity()-f
			  );
    }


    RelationImpl RelationImpl::forall(int c) const {
      return RelationImpl(VarImpl::forall(c, bdd_), arity_);
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
      // p indicates the columns on the right that will remain at the end.

      // it is a mistake to project in more columns that the ones in the relation
      assert(p <= arity_ && "Projecting in more columns that the onse in the relation");

      RelationImpl r(*this);
      // Project on 0 columns is the empty relation
      if (p == 0) return RelationImpl(0);
      // Project on all the columns of a relation is the relation itself
      if (p == arity_) return r;

      // 1) existentially quantify on all the columns but the ones from 0 (right most)
      //    to p (not including).
      int first = p;
      int last = arity_ - 1;
      return RelationImpl(VarImpl::exists(first,last,r.bdd_),p);
    }

    void RelationImpl::print(std::ostream& os) const {
      // this prints the bdd with all the columns in the manager
      printSet(bdd_,Limits::arity,os);
      // this is to print only the columns that correspond to the
      // represented relation
      //printSet(bdd_,arity_,os);
    }

    std::ostream& operator << (std::ostream& os, const RelationImpl& r) {
      r.print(os);
      return os;
    }


    RelationImplIter RelationImpl::tuples(void) const {
      return RelationImplIter(bdd_,arity_);
    }

    RelationImplIter::RelationImplIter(BDD rel, int a)
      : relation_(rel), arity_(a){}

    RelationImplIter::RelationImplIter(const RelationImplIter& it)
      : relation_(it.relation_), arity_(it.arity_){}

    RelationImplIter::~RelationImplIter(void) {}

    bool RelationImplIter::operator ()(void) const {
      return relation_ != zero();
    }

    Tuple RelationImplIter::val(void) {
      const int cube_size = 1<<(Limits::bbv + Limits::ba);

      int cube_[cube_size];
      int *cube = cube_;
      CUDD_VALUE_TYPE val;

      DdGen* gen = Cudd_FirstCube(dd(),relation_.getNode(),&cube,&val);
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
      BDD i = t.getBDD();
      relation_ &= !i;
    }

  }}
