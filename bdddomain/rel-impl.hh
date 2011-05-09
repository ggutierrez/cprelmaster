#ifndef __CPREL_BDDDOMAIN_REL_IMPL_HH__
#define __CPREL_BDDDOMAIN_REL_IMPL_HH__

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <cuddCdd.h>
#include <cprel/tuple.hh>

namespace CPRel { namespace BDDImpl {
/**
 * \brief Class to handle the creation and destruction of Cudd related objects.
 *
 * The idea of having this class is to create a global object that
 * is in charge of the resource deallocation of Cudd entities.
 */

class BddManager;
typedef boost::shared_ptr<BddManager> PManager;

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
  /// Constructor that initializes the BDD manager of CUDD
  BddManager (void)
    : dd(Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0))
    , one_(CDD_ONE(dd))
    , zero_(CDD_ZERO(dd))
    , BBV_(5), BA_(3)
  {
    std::cout << "Created bdd manager" << std::endl;
  }
  /// Destructor that releases the BDD manager of CUDD
  ~BddManager (void) {
    std::cout << "Called destructor: " << references() << std::endl;
    Cudd_Quit(dd);
  }
  static void deleter(BddManager *ptr) {
    delete ptr;
  }
  static void create() {
    _instance.reset( new BddManager, &deleter );
  }
  int references(void) {
    return Cudd_CheckZeroRef(dd);
  }
public:
  static PManager instance(void) {
    if (_instance.get() != NULL)
      return _instance;
    create();
    return _instance;
  }
  DdManager* manager(void) {
    return dd;
  }
  DdNode* one(void) {
    return one_;
  }
  DdNode* zero(void) {
    return zero_;
  }
  int bbv(void) {
    return BBV_;
  }
  int ba(void) {
    return BA_;
  }
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
public:
  /// Default constructor for an empty relation of arity \a a
  RelationImpl(int a);
  /// Copy constructor
  RelationImpl(const RelationImpl& r);
  /// Asignement operator
  RelationImpl& operator=(const RelationImpl& right);
  /// Swap
  void swap(const RelationImpl& r);
  /// Destructor
  ~RelationImpl(void);
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
  /// Returns the current cardinality of the relation
  double cardinality(void) const;
  /// Returns the arity of the relation
  int arity(void) const;
  /// Returns an iterator on the tuples of the relation
  RelationImplIter tuples(void) const;
};

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

}}

#endif
