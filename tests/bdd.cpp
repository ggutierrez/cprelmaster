#include <iostream>
#include <vector>
#include <cuddCdd.h>
#include <cudd/cuddInt.h>


using std::cout;
using std::cerr;
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
  // Print: temporal
  void print(int a) const {
    if (node_ == CDD_ONE(BDDConfig::dd)) {
      cerr << "Universe";
      return;
    }
    if (node_ == CDD_ZERO(BDDConfig::dd)) {
      cerr << "Empty";
      return;
    }

    DdGen* gen;
    int *cube = (int*)malloc(sizeof(int)*(1<<(BDDConfig::BBV+BDDConfig::BA)));
    int tuple[1<<BDDConfig::BA];
    CUDD_VALUE_TYPE val;
    int done;
    int i,k,j;
    cerr << "#(" << Cudd_CountMinterm(BDDConfig::dd,node_,a<<BDDConfig::BBV) << "){";
    //printf("Cardinality: %f\n", Cudd_CountMinterm(Cudd::dd,node,a<<BBV));
    for(k=0;k<1<<BDDConfig::BA;k++)tuple[k]=0;
    Cudd_ForeachCube(BDDConfig::dd,node_,gen,cube,val){
      done=0;
      while(!done){
        done=1;
        for(i=(1<<(BDDConfig::BBV+BDDConfig::BA))-1;i>=0;i--){
          if((i&((1<<BDDConfig::BA)-1))<a){
            tuple[i&((1<<BDDConfig::BA)-1)]&=~(1<<((1<<BDDConfig::BBV)-1-(i>>BDDConfig::BA)));
            tuple[i&((1<<BDDConfig::BA)-1)]|=(cube[i]&1)<<((1<<BDDConfig::BBV)-1-(i>>BDDConfig::BA));
            if((cube[i]&2)&&done){
              done&=cube[i]&1;
              cube[i]^=1;
            }
          }
        }
        cerr << "<";
        for(j = 0; j < a; j++) {
          cerr << tuple[j] << ",";//printf("%d,",tuple[j]);
        }
        //printf(">\n");
        cerr << ">, ";
      }
    }
    cerr << "}";
    
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
      : representation_(Bdd::zero), arity_(arity) {
      cout << "References in the Bdd manager (before): " << BDDConfig::references() << endl;
      for (unsigned int i = 0; i < data.size(); i++) {
	Bdd r(Bdd::one);
	r.intersectAssign(createPath(data[i].first,0));
	r.intersectAssign(createPath(data[i].second,1));
	representation_.unionAssign(r);
      }
      cout << "References in the Bdd manager: " << BDDConfig::references() << endl;
    }
    ~GRelation(void) {}
    void print(void) {
      representation_.print(arity_);
    }
  };
}

int main(void) {
  using namespace BDDImpl;
  cout << "Bdd package configuration "
       << BDDConfig::BBV
       << endl;
  {
    /*
    Bdd r(Bdd::one);
    // add 0,1
    Bdd f(createPath(0,0));
    r.intersectAssign(f);
    
    f = createPath(1,1);
    r.intersectAssign(f);
    r.print(2);
    
    Bdd s(Bdd::one);
    // add 1, 2
    Bdd t(createPath(1,0));
    s.intersectAssign(t);
    t = createPath(2,1);
    s.intersectAssign(t);
    
    cerr << endl;
    s.print(2);

    cerr << endl;
    r.unionAssign(s);
    r.print(2);
    */

    vector<pair<int,int> > data(4);
    data.push_back(make_pair(0,0));
    data.push_back(make_pair(0,1));
    data.push_back(make_pair(1,0));
    data.push_back(make_pair(1,1));
      
    GRelation rr(2,data);
    rr.print();
   
    cout << "References in the Bdd manager (before): " << BDDConfig::references() << endl;
  }
  cout << "References before exit: " << BDDConfig::references() << endl;
  return 0;
}
