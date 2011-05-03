#include <iostream>
#include <vector>
#include <cuddCdd.h>
#include <cudd/cuddInt.h>


using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::make_pair;

namespace BDDImpl {

class BDDConfig {
public:
  /// Pointer to the Bdd manager
  static DdManager *dd;
  /// Constant true
  static DdNode *one;
  /// Constant false
  static DdNode *zero;
  static int BBV;
  static int  BA;
  static int references(void) {
    return Cudd_CheckZeroRef(dd);
  }
};

// Initializate the manager
DdManager* BDDConfig::dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
DdNode* BDDConfig::one = CDD_ONE(BDDConfig::dd);
DdNode* BDDConfig::zero = CDD_ZERO(BDDConfig::dd);
int BDDConfig::BBV = 5;
int BDDConfig::BA = 3;


/// Class for Binary Decision Diagrams
class Bdd {
private:
  enum BddTerm { Bdd_Zero, Bdd_One, Bdd_Unk};
  /// Root node of the representation
  DdNode *node_;
  /// Constructor from an existing node
  Bdd(DdNode *n) : node_(n) {
    if (node_ != NULL) Cudd_Ref(node_);
  }
  /// Construcotr for terminal \a t
  Bdd(BddTerm t) {
    // This initialization cannot be done with the previous constructor because
    // when terminal are created they cannot be referenced, otherwise it will
    // report referenced nodes before finishing the program.
    switch(t) {
    case Bdd_Zero: node_ = BDDConfig::zero; break;
    case Bdd_One:  node_ = BDDConfig::one; break;
    default:
      assert(false); break;
    }
  }

public:
  /// Default constructor that contains no representation
  Bdd(void) : node_(NULL) {}
  /// Constructor from an existing representation
  Bdd(const Bdd& r) : node_(r.node_) {
    if (node_ != NULL) Cudd_Ref(node_);
  }
  /// Creates a representation with the ith node with value \a b
  Bdd(int i, bool b) {
    if (b) {
      node_ = Cudd_bddIthVar(BDDConfig::dd,i);
    } else {
      node_ = Cudd_Not(Cudd_bddIthVar(BDDConfig::dd,i));
    }
    assert(node_ != NULL);
    Cudd_Ref(node_);
  }
  /// Destructor
  ~Bdd(void) {
    if (node_)
      Cudd_RecursiveDeref(BDDConfig::dd, node_);
  }
  /// Constant true
  static Bdd one;
  /// Constant false
  static Bdd zero;
  /// Tests if this representation is equal to \a r
  bool eq(const Bdd& r) { return node_ == r.node_; }
  /// Tests if this representation is different from \a r
  bool diff(const Bdd& r) { return !eq(r); }
  /// Compute the complement of this representation
  Bdd complement(void) const {
    // TODO: this has to change for CDDs
    return Bdd(Cudd_Not(node_));
  }
  /// Assignement operator
  Bdd& operator = (const Bdd& right) {
    if (this == &right) return *this;
    if (right.node_ != NULL) Cudd_Ref(right.node_);
    if (node_ != NULL) Cudd_RecursiveDeref(BDDConfig::dd,node_);
    node_ = right.node_;
    return *this;
  }
  ///Computes the logic and between this representation and \a r.
  void intersectAssign(const Bdd& r) {
    Bdd tmp = intersect(r);
    this->operator=(tmp);
  }
  Bdd intersect(const Bdd& r) const {
    DdNode *i = Cudd_bddAnd(BDDConfig::dd,node_,r.node_);
    assert(i);
    return Bdd(i);
  }
  Bdd Union(const Bdd& r) const {
    DdNode *i = Cudd_bddOr(BDDConfig::dd,node_,r.node_);
    assert(i);
    return Bdd(i);
  }
  ///Computes the logic or between this representation and \a r.
  void unionAssign(const Bdd& r) {
    Bdd tmp = Union(r);
    this->operator=(tmp);
  }
};
  Bdd Bdd::one = Bdd(Bdd::Bdd_One);
  Bdd Bdd::zero = Bdd(Bdd::Bdd_Zero);

  /**
   * \brief Path creation in BDD
   *
   * Creates a BDD to represent \a p (encoded as sequence of bits)
   * for a column \a a.
   */
  Bdd createPath(int p, int a) {
    Bdd f(Bdd::one);
    for (int i = (1 << BDDConfig::BBV); i--;) {
      f.intersectAssign(Bdd((i << BDDConfig::BA)+a, p&1));
      p >>= 1;
    }
    return f;
  }
  
  /// Definition for ground relations
  class GRelation {
  private:
    /// Relation storage
    Bdd representation_;
    /// Arity
    int arity_;
  public:
    GRelation(int arity,const vector<pair<int,int> >& data) 
      : representation_(Bdd::one), arity_(arity) {
      for (unsigned int i = 0; i < data.size(); i++) {
	std::cout << "Should add " << data[i].first << ", " << data[i].second << endl;
	representation_.intersectAssign(createPath(data[i].first,0));
	representation_.intersectAssign(createPath(data[i].second,1));
      }
    }
    ~GRelation(void) {}
  };
}

int main(void) {
  using namespace BDDImpl;
  cout << "Bdd package configuration "
       << BDDConfig::BBV
       << endl;
  {
    Bdd r = Bdd::one;
    Bdd s = Bdd::zero;
    Bdd t(r.intersect(s)); 
    r.intersectAssign(s);
    cout << "Equal? " << r.eq(t) << endl;

    vector<pair<int,int> > data;
    data.push_back(make_pair(0,0));
    data.push_back(make_pair(0,1));
    data.push_back(make_pair(1,0));
    data.push_back(make_pair(1,1));
    
    GRelation rr(2,data);

    cout << "References in the Bdd manager (before): " << BDDConfig::references() << endl;
  }
  cout << "References in the Bdd manager: " << BDDConfig::references() << endl;
          return 0;
}
