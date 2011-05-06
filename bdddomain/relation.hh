#ifndef __BDDDOMAIN_RELATION_HH__
#define __BDDDOMAIN_RELATION_HH__

#include <vector>
#include <cuddCdd.h>

namespace BDDImpl {

  using std::vector;
  using std::pair;
  
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

  /// Iterator on a Bdd representation
  class BddIter {
  private:
    /// The represented relation
    DdNode *relation_;
    /// Arity of the encoded relation
    int arity_;
    /// Undefined default constructor
    BddIter(void);
    /// Removes a tuple from the iterated relation
    void remove(const vector<int>& tuple);
  public:
    BddIter(DdNode *relation, int arity);
    /// Copy constructor
    BddIter(const BddIter& i);
    ~BddIter(void);
    bool operator()(void) const;
    /// This is asingle pass iterator
    vector<int> val(void);
    void operator++(void);
  };

  /// Class for Binary Decision Diagrams
  class Bdd {
  private:
    friend class BddIter;
    enum BddTerm { Bdd_Zero, Bdd_One, Bdd_Unk};
    /// Root node of the representation
    DdNode *node_;
    /// Constructor from an existing node
    explicit Bdd(DdNode *n);
  private:
    /// Construcotr for terminal \a t
    explicit Bdd(BddTerm t);
  public:
    /// Default constructor that contains no representation
    Bdd(void);
    /// Constructor from an existing representation
    Bdd(const Bdd& r);
    /// Creates a representation with the ith node with value \a b
    Bdd(int i, bool b);
    /// Destructor
    ~Bdd(void);
    /// Constant true
    static Bdd one;
    /// Constant false
    static Bdd zero;
    /// Tests if this representation is equal to \a r
    bool eq(const Bdd& r) const;
    /// Tests if this representation is different from \a r
    bool diff(const Bdd& r) const;
    /// Compute the complement of this representation
    Bdd complement(void) const;
    /// Assignement operator
    Bdd& operator = (const Bdd& right);
    ///Computes the logic and between this representation and \a r.
    void intersectAssign(const Bdd& r);
    Bdd intersect(const Bdd& r) const;
    Bdd Union(const Bdd& r) const;
    ///Computes the logic or between this representation and \a r.
    void unionAssign(const Bdd& r);
    /// Computes the difference between this and \a r
    Bdd difference(const Bdd& r) const;
    /// Computes the difference between this representation and \a r
    void differenceAssign(const Bdd& r);
    /// Return the number of minterms represented
    int minterms(int a) const;
    BddIter tuples(int a);
    // Print: temporal
    void print(int a) const;
  };

  /// Definition for ground relations
  class GRelation {
  private:
    /// Relation storage
    Bdd representation_;
    /// Arity
    int arity_;
    /// Maximum element of the relation
    int maximum_;
    /// Add the pair \a p to the relation
    Bdd addPair(const pair<int,int>& p) const;
  public:
    /// Constructor from a representation
    GRelation(Bdd& representation, int arity, int maximum);
    /// Constructor of a ground relation from a set of tuples
    GRelation(int arity,const vector<pair<int,int> >& data);
    /// Constructor for a full ground relation of arity \a arity and
    /// maximum element \a maximum
    GRelation(int arity, int maximum);
    /// Copy constructor
    GRelation(const GRelation& r);
    /// Destructor
    ~GRelation(void);
    /// Iterator
    BddIter tuples(void);
    /// Tests if this represents the empty relation
    bool empty(void) const;
    bool full(void) const;
    /// Complement
    GRelation complement(void) const;
    /// Relation intersection
    GRelation intersect(const GRelation& r) const;
    /// Tests if tuple \a t is present in the relation
    bool in(const pair<int,int>& t) const;
    /// Print a relation as a set of tuples
    void print(void) const;
  };
}

#endif
