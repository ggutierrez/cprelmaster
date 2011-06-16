#ifndef __CDDOBJ_CDD_HH__
#define __CDDOBJ_CDD_HH__

#include <iostream>
#include <list>
#include <tr1/tuple>
#include <exception>
#include <cuddCdd.h>

// 32 bits integers max 8ary
#define BBV 5
#define BA 3

//#define BBV 2
//#define BA 1

namespace Cdd {
  class failedMerge : public std::exception {
    virtual const char* what(void) const throw() {
      return "Failed operation on CDD";
    }
  };
  class malformedDomain : public std::exception {
    virtual const char* what(void) const throw() {
      return "Malformed bounds in definition";
    }
  };
  /**
   * \brief BDD abstraction
   */
  class BDD {
  private:
    /// Function pointer
    DdNode *node;
    /// Create a BDD to representing \a p for a column \a a
    static BDD path(int p, int a);
  public:
    /// Constructor
    BDD(void);
    /// Constructor from an existing node
    BDD(DdNode *n);
    /// Copy constructor
    BDD(const BDD& from);
    /// Returns the variable at possition \a v with a value of \a b
    BDD(int v, bool b);
    /// Constructs a BDD with the binary encoding of the elementes in [\a b, \a e]
    template <class I>
    BDD(I& b, I& e);
    /// Destructor
    ~BDD(void);
    static BDD create(std::tr1::tuple<int,int> t);
    /// \name Test operations
    //@{
    /// BDD equality
    bool operator == (const BDD& b) const;
    /// BDD diesquality
    bool operator != (const BDD& b) const;
    //@}
    /// \name Operations
    //@{
    /// BDD negation
    BDD operator ! (void) const;
    /// BDD negation
    BDD operator ~ (void) const;
    /// Assignment operation
    BDD operator = (const BDD& right);
    /// BDD intersection
    BDD operator &= (const BDD& other);
    /// BDD union
    BDD operator |= (const BDD& other);
    /// CDD merge
    BDD operator << (const BDD& other) const;
    /// CDD intersection
    BDD operator && (const BDD& other) const;
    /// CDD union
    BDD operator || (const BDD& other) const;
    /// CDD xor
    BDD operator ^ (const BDD& other) const;
    /// Status operation
    BDD status(const BDD& r) const;
    /// Delta operation
    BDD delta(void) const;
    //@}
    /// \name BDD information and statistics
    //@{
    /// Return the number of minterms of this BDD with a maximum number of variables of \a nvars
    double minterms(int nvars) const;
    /// Return the number of nodes in the BDD
    int numNodes(void) const;
    /// Print the tuples represented in the BDD for a relation with arity a
    void printTuples(int a, std::ostream& os) const;
    //@}
    /// \name BDD output
    //{@
    /// Output the BDD in dot format to \a fname
    void dot(const char* fname) const;
    //@}
  };

  class Cudd {
    friend class BDD;
  private:
    static DdManager *dd;
  public:
    /// Constructor
    Cudd(void);
    /// Terminal one
    static BDD one(void);
    /// Terminal unknown
    static BDD unk(void);
    /// Terminal zero
    static BDD zero(void);
#ifndef NDEBUG
    /// Return the number of referenced nodes in the manager
    static int references(void);
    /// Print statistics about CUDD
    static void stats(void);
#endif
  };

  class Relation {
  private:
    /// Domain representation
    BDD r;
    /// Arity
    int arity;
    /**
     * \brief Initialization of a constraint domain with \a lb and \a ub as the
     * lower and upper bound resp.
     *
     * \warning \a lb and \a ub are assumed to be ground relations
     */
    void init(const BDD& lb, const BDD& ub);
  public:
    /// Constructor for an empty relation of arity \a a
    Relation(int a);
    /**
     * Constructor for the relation domain [\a lb, \a ub]
     *
     * \warning raises an exception if \a lb is not a subset of \a ub
     */
    Relation(const BDD& lb, const BDD& ub, int a);
    /**
     * Constructor for a relation domain described by \a dom
     */
    Relation(const BDD& dom, int a);
    /**
     * \brief Initialization of a constraint domain with the relation represented
     * by \a d.
     *
     * \warning \a d is assumed to be a domain representation: CDD or BDD
     */
    void init(const BDD& d);
    /// Destructor
    ~Relation(void);
    /**
     * \brief Exclude the relation represented by \a g from the domain of this
     * variable.
     *
     * \warning It is assumed that \a g has the same cardinality of this.
     * \warning An exception is triggered if the operation fails (e.g. tries to
     * remove elements already in the lower bound)
     */
    void exclude(const BDD& g);
    Relation& operator >>= (const BDD& g);

    void include(const BDD& g);
    Relation& operator <<= (const BDD& g);
    /// Cardinality (number of tuples represented by the BDD \a b for a relation of this cardinality)
    double cardinality(const BDD& b) const;
    /// Return the lower bound of the relation (do not us this in propagation)
    const BDD glb(void) const;
    /// Return the upper bound of the relation (do not us this in propagation)
    const BDD oob(void) const;
    /// Return the unknow of the relation (do not us this in propagation)
    const BDD unk(void) const;
    /// Return the CDD representing the domain
    const BDD dom(void) const;
  };

  /// Output relation \a r to \a os
  std::ostream& operator << (std::ostream& os, const Relation& r);

  template <class I>
  BDD::BDD(I& b, I& e) {
    BDD node = Cudd::one();
    int i = 0;
    for (; b != e; ++b, i++) {
      node &= BDD::path(*b, i);
    }
  }
}

#endif
