#ifndef __CPREL_BDDDOMAIN_REL_IMPL_HH__
#define __CPREL_BDDDOMAIN_REL_IMPL_HH__

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <cuddCdd.h>
#include <cprel/tuple.hh>

namespace MPG { namespace CPRel { namespace VarImpl {

class BddManager;
typedef boost::shared_ptr<BddManager> PManager;
/**
 * \brief Class to handle the creation and destruction of Cudd related objects.
 *
 * The idea of having this class is to create a global object that
 * is in charge of the resource deallocation of Cudd entities. For
 * this reason only one manager is used and this class implements the
 * singleton pattern.
 */
class BddManager : private boost::noncopyable {
private:
  /// Only one single instance of this class is allowed at run time
  static PManager _instance;
  /// Pointer to the Bdd manager
  DdManager *dd;
  /// Constant true
  DdNode *one_;
  /// Constant false
  DdNode *zero_;
  int BBV_;
  int  BA_;
  /// \name Constructors and destructor
  //@{
  /// Constructor that initializes the BDD manager of CUDD
  BddManager (void)
    : dd(Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0))
    , one_(CDD_ONE(dd))
    , zero_(CDD_ZERO(dd))
    , BBV_(5), BA_(3)
  {
    std::cout << "Created bdd manager" << std::endl;
  }
  /// Creates an instace of this object
  static void create() {
    _instance.reset( new BddManager, &deleter );
  }
  /// Destructor that releases the BDD manager of CUDD
  ~BddManager (void) {
    std::cout << "Called destructor: " << references() << std::endl;
    Cudd_Quit(dd);
  }
  //@}
  /// Method called by the managed pointer when destructed
  static void deleter(BddManager *ptr) {
    delete ptr;
  }
  int references(void) {
    return Cudd_CheckZeroRef(dd);
  }
public:
  /// Returns an instance of the manager
  static PManager instance(void) {
    if (_instance.get() != NULL)
      return _instance;
    create();
    return _instance;
  }
  /// \name Access
  //@{
  /// Returns the bdd manager
  DdManager* manager(void) {
    return dd;
  }
  /// Returns the constant \c true
  DdNode* one(void) {
    return one_;
  }
  /// Returns the constant \c false
  DdNode* zero(void) {
    return zero_;
  }
  int bbv(void) {
    return BBV_;
  }
  int ba(void) {
    return BA_;
  }
  //@}
};

class RelationImplIter;
/// Stores the representation of a relation using BDDs
class RelationImpl {
private:
  /// The BDD with the representation
  DdNode *bdd_;
  /// The arity of the represented relation
  int arity_;
  /// Avoid default construction
  RelationImpl(void);
  /// Constructor from anexisting bdd
  RelationImpl(DdNode *n, int a);
public:
  /// \name Constructors, destructor and assignement
  //@{
  /// Default constructor for an empty relation of arity \a a
  explicit RelationImpl(int a);
  /// Copy constructor
  RelationImpl(const RelationImpl& r);
  /// Asignement operator
  RelationImpl& operator=(const RelationImpl& right);
  /// Swap
  void swap(const RelationImpl& r);
  /// Create a full relation of arity \a a
  static RelationImpl create_full(int a);
  /// Destructor
  ~RelationImpl(void);
  //@}
  /// \name Modification
  //@{
  /// Adds tuple \a t to the relation.
  void add(const Tuple& t);
  /// Removes tuple \a t from the relation
  void remove(const Tuple& t);
  /// Adds relation \a r to this relation
  void add(const RelationImpl& r);
  /// Removes relation \a r to this relation
  void remove(const RelationImpl& r);
  /// Intersects the relation with relation \a r
  void intersect(const RelationImpl& r);
  /// Complement the represented relation
  void complement(void);
  //@}
  /// \name Information
  //@{
  /// Tests if relation \a r represents the same elements
  bool equal(const RelationImpl& r) const;
  /// Returns the current cardinality of the relation
  double cardinality(void) const;
  /// Returns the arity of the relation
  int arity(void) const;
  /// Tests if the represented relation is empty
  bool empty(void) const;
  /// Tests if the represented relation is the universe
  bool universe(void) const;
  //@}
  /// \name Iteration
  //@{
  /// Returns an iterator on the tuples of the relation
  RelationImplIter tuples(void) const;
  //@}
};

/// Tests whether two relations are the same
inline
bool operator==(const RelationImpl& r, const RelationImpl& s) {
  return r.equal(s);
}

/// Tests whether two relations are different
inline
bool operator!=(const RelationImpl& r, const RelationImpl& s) {
  return !r.equal(s);
}

/// Returns the union of relations \a r and \a s
inline
RelationImpl Union(const RelationImpl& r, const RelationImpl& s) {
  assert(r.arity() == s.arity());
  RelationImpl u(r);
  u.add(s);
  return u;
}

/// Returns the difference between relations \a r and \a s
inline
RelationImpl difference(const RelationImpl& r, const RelationImpl& s) {
  assert(r.arity() == s.arity());
  RelationImpl d(r);
  d.remove(s);
  return d;
}

/// Returns the intersection between relations \a r and \a s
inline
RelationImpl intersect(const RelationImpl& r, const RelationImpl& s) {
  assert(r.arity() == s.arity());
  RelationImpl i(r);
  i.intersect(s);
  return i;
}

/// Tests if relation \a r is a subset of relation \a s
inline
bool subset(const RelationImpl& r, const RelationImpl& s) {
  assert(r.arity() == s.arity());
  return intersect(r,s) == r;
}

/// Returns the complement of relation \a r
inline
RelationImpl complement(const RelationImpl& r) {
  RelationImpl c(r);
  c.complement();
  return c;
}

/// Class to iterate in tuples of a relation implementation
class RelationImplIter {
private:
  /// The BDD with the representation of the relation being iterated
  DdNode *relation_;
  /// The arity of the relation
  int arity_;
  /// Avoid default construction
  RelationImplIter(void);
  /// Removes \a t from the iterated relation
  void remove(const Tuple& t);
public:
  /// Constructor
  RelationImplIter(DdNode *rel, int a);
  /// Copy constructor
  RelationImplIter(const RelationImplIter&);
  /// Destructor
  ~RelationImplIter(void);
  /// Return a current tuple
  Tuple val(void);
  /// Tests whether the iterator is still valid
  bool operator()(void) const;
};

inline
std::ostream& operator << (std::ostream& os, const RelationImpl& r) {
  os << "{";
  for (RelationImplIter it = r.tuples(); it();)
    os << it.val() << " ";

  os << "}";
  return os;
}
}}}

#endif
