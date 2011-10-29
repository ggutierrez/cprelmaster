#include <iostream>
#include <bdddomain/encoding.hh>
#include <bdddomain/rel-impl.hh>
#include <bdddomain/bdd.hh>

using std::pair;
using std::vector;

namespace MPG { namespace VarImpl {

    /*
     * Constructors, destructors and assignment
     */
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

    /*
     * Information
     */
    bool RelationImpl::equal(const RelationImpl& r) const {
      if (arity_ != r.arity_) return false;
      return bdd_ == r.bdd_;
    }

    double RelationImpl::cardinality(void) const {
      return bdd_.CountMinterm(arity_ << Limits::bbv);
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

    /*
     * Modification
     */
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

    void RelationImpl::complement(void) {
      bdd_ = !bdd_;
    }

    /*
     * Existential quantification
     */
    RelationImpl RelationImpl::exists(int c) const {
      return RelationImpl(VarImpl::exists(c, bdd_), arity_);
    }

    RelationImpl RelationImpl::exists(const std::vector<int>& c) const {
      return RelationImpl(VarImpl::exists(c, bdd_), arity_);
    }

    /*
     * Unique quantification
     */
    RelationImpl RelationImpl::unique(int c) const {
      return RelationImpl(VarImpl::unique(c, bdd_), arity_);
    }

    RelationImpl RelationImpl::unique(const std::vector<int>& c) const {
      return RelationImpl(VarImpl::unique(c, bdd_), arity_);
    }

    /*
     * Universal quantification
     */
    RelationImpl RelationImpl::forall(int c) const {
      return RelationImpl(VarImpl::forall(c, bdd_), arity_);
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
   
    RelationImpl RelationImpl::permute(const std::vector<std::pair<int,int>>& perm) const {
      BDD result = bdd_;
      for (const auto& p : perm) {
   	std::vector<std::pair<int,int>> x;
   	x.push_back(p);
   	result = VarImpl::swap(x,result);
      } 
      return RelationImpl(result,arity_);
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

    /*
     * Column manipulation
     */
    RelationImpl RelationImpl::discard(int start, int end) const {
      assert(start >= 0 && start < end && start <= Limits::arity && "Invalid value for start");
      assert(end > 0 && end > start && end <= Limits::arity && "Invalid value for end");

      if (start == 0 && end == arity_ + 1) {
	// the whole relation will be discarded
	return RelationImpl(zero(),0);
      }
      
      // Note that this loop is inneficient, the reason is that it
      // does one swap at the time. I have to investigate further if
      // it is possible to get the same effect by computing all the
      // swapping first and then doing the operation once at the bdd.
      int len = end - start;
      BDD replaced = bdd_;
      for (int i = end; i < arity_; i++) {
	std::vector<std::pair<int,int>> p;
	p.push_back({i-len,i});
	replaced = VarImpl::swap(p,replaced);
      }

      std::vector<int> quantify;
      for (int i = arity_ - len; i < arity_; i++) {
	quantify.push_back(i);
      }
     
      BDD quantified = VarImpl::exists(quantify,replaced);
      return RelationImpl(quantified,arity_ - len);
    }

    RelationImpl RelationImpl::shiftRight(int n) const {
      assert(n >= 0 && "Only positive values are accepted");
      // moving zero columns yields the same relation
      if (n == 0) return *this;
      // moving more columns than the arity will result in an empty
      // relation
      if (n >= arity_)
	return RelationImpl(0);
      BDD res = bdd_;
      for (int i = n; i < arity_; i++) {
   	std::vector<std::pair<int,int>> p;
	p.push_back({i-n,i});
	res = VarImpl::swap(p,res);
      }
      // quantify on the columns that need to disapear
      std::vector<int> quantify;
      for (int i = arity_-1; i >= arity_ - n; i--) {
	quantify.push_back(i);
      }
      res = VarImpl::exists(quantify,res);
      return RelationImpl(res,arity_-n);
    }
    
    /*
     * Relational algebra operations on relations
     */
    
    /*
     * Join, follow  
     */
    
    RelationImpl RelationImpl::join(int j, const RelationImpl& right) const {
      const RelationImpl& left = *this;
      assert(left.arity() >= j && right.arity() >= j
    	     && "There are not enough columns for the join");
    
      RelationImpl lxu = left.timesURight(right.arity() - j);
      RelationImpl rxu = right.timesULeft(left.arity() - j);
    
      lxu.intersect(rxu);
      return lxu;
    }
    
    RelationImpl RelationImpl::follow(int f, const RelationImpl& right) const {
      const RelationImpl& left = *this;
      
      /// \todo handle errors on arity of the relations with exceptions
    
      // 1) compute the join of the relations on f columns
      RelationImpl join = left.join(f,right);
      
      //std::cout << "The join " << join << std::endl; 
      //std::cout << "Is empty the result ? " << join.empty() << std::endl; 
      // 2) remove the "join" columns.      
      //std::cout << "First " << right.arity() - f << std::endl; 
      //std::cout << "Second " << right.arity()  << std::endl;  
      return RelationImpl(
    			  join.discard(right.arity() - f, right.arity()).bdd_,
    			  join.arity()-f
    			  );
    }
    
    /*
     * Projection
     */

    RelationImpl RelationImpl::project(int p) const {
      // p indicates the _number_ of columns on the right that will
      // remain at the end.

      // it is a mistake to project in more columns that the ones in
      // the relation
      assert(p <= arity_ &&
	     "Projecting in more columns that the onse in the relation");

      RelationImpl r(*this);
      // Project on 0 columns results in the empty relation
      if (p == 0) return RelationImpl(0);
      // Project on all the columns of a relation is the relation itself
      if (p == arity_) return r;
   
      // Projecting is the same as removing all the columns that are
      // at indices greater than p-1.  remember that p is a number of
      // columns on the right and not a column index.
      return discard(p, arity_);
    }
   
    /*
     * Relation output
     */
    void RelationImpl::print(std::ostream& os) const {
      os << "[a: " << arity_ << " #: " << cardinality() << "]" << std::endl;
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
   
    /*
     * Element access
     */
    Tuple RelationImpl::pickOneTuple(void) const {
      BDD tupleRepr = VarImpl::oneTuple(arity_,bdd_);
      return Tuple(tupleRepr,arity_);
    }
   
  }
}
