#ifndef __CPREL_CPREL_GRELATION_HH__
#define __CPREL_CPREL_GRELATION_HH__

#include <boost/shared_ptr.hpp>
#include <bdddomain/tuple.hh>
#include <cprel/descriptor.hh>
#include <boost/shared_ptr.hpp>

namespace MPG { namespace CPRel {
/**
 * \defgroup GRelation Ground relations
 *
 * Ground relations are the basic construct of the relation domain. The term
 * ground is used to differenciate this relation from a relation represented by
 * a decission variable. This module offers the capability to define and operate
 * on this kind of relations.
 */

namespace VarImpl {
  class RelationImpl;
}

class GRelationIter;

/**
 * \brief Class representing a ground relation
 * \ingroup GRelation
 */
class GRelation {
private:
  friend class GRelationIter;
  typedef boost::shared_ptr<VarImpl::RelationImpl> Impl;
  /// Relation storage
  Impl pimpl_;
  /// Avoid default construction
  GRelation(void);
  /// Constructor taking an implementation
  explicit GRelation(Impl impl);
public:
  /// \name Constructors, destructors and assignement
  //@{
  /// Constructor for an empty relation of arity \a a
  explicit GRelation(int a);
  /// Copy constructor
  GRelation(const GRelation& r);
  /// Assignment
  GRelation& operator=(GRelation& right);
  /// Destructor
  ~GRelation(void);
  /// Constructs a full relation of arity \a a
  static GRelation create_full(int a);
  //@}
  /// \name Modification operations
  //@{
  /**
   * \brief Adds tuple \a t to the relation
   *
   * The returned value indicates if the relation changed due to the operation.
   */
  bool add(const Tuple& t);
  /**
   * \brief Union of relations: \f$ this = this \cup r \f$.
   *
   * The returned boolean indicates of \c this relation changed
   * because the operation.
   */
  bool unionAssign(const GRelation& r);
  /**
   * \brief Difference of relations: \f$ this = this \setminus r \f$.
   *
   * The returned boolean indicates of \c this relation changed
   * because the operation.
   */
  bool differenceAssign(const GRelation& r);
  //@}
  /// \name Relation operations
  //@{
  /// Computes \f$ this \setminus r \f$
  GRelation difference(const GRelation& r) const;
  /// Computes \f$ this \cap r \f$
  GRelation intersect(const GRelation& r) const;
  /// Computes \f$ \overline{this}\f$
  GRelation complement(void) const;
  /// Computes the permutation of this according to \a desc
  GRelation permute(const PermDescriptor& desc) const;
  //@}
  /// \name Test operations
  //@{
  /// Tests \f$ this \subseteq r \f$
  bool subsetEq(const GRelation& r) const;
  /// Tests \f$ this \supset r \f$
  bool superset(const GRelation& r) const;
  /// Tests \f$ this \cap r = \emptyset \f$
  bool disjoint(const GRelation& r) const;
  /// Tests whether this represents the same relation as \a r
  bool eq(const GRelation& r) const;
  /// Tests whther the relation is empty
  bool empty(void) const;
  /// Tests whther the relation represents the univers
  bool universe(void) const;
  //@}
  /// \name Information
  //@{
  /// Returns the arity of the relation
  int arity(void) const;
  /// Returns the cardinality of the relation
  double cardinality(void) const;
  //@}
  /// \name Constant relations
  //@{
  /// Creates the binary relation \f$ R = \{(x,y) : x = y \} \f$
  static GRelation equalXY(void);
  //@}
};

/**
 * \brief Creates a relation with the elements contained in \a dom.
 * \ingroup GRelation
 *
 * The arity of the created relation corresponds to the number of elements of the
 * provided tuples. All the tuples are assumed to be of the same arity.
 */
GRelation create(const std::vector<Tuple>& dom);

/**
 * \brief Creates a full relation of arity \a a.
 * \ingroup GRelation
 */
GRelation create_full(int a);

/**
 * \brief Outputs relation \a r to \a os
 * \ingroup GRelation
 *
 * \todo The implementation of this function relies on an iterator on the tuples
 * of \a r. I should offer a better way to print the domain in a compressed way
 */
std::ostream& operator<< (std::ostream& os, const GRelation& r);

namespace VarImpl {
  class RelationImplIter;
}

/**
 * \brief Iterator on the tuples of a ground relation
 * \ingroup GRelation
 */
class GRelationIter {
private:
  typedef boost::shared_ptr<VarImpl::RelationImplIter> Impl;
  /// Relation storage
  Impl pimpl_;
  typedef boost::shared_ptr<Tuple> TuplePtr;
  /**
   * \brief Stores the current tuple
   *
   * This temporal storage is needed because the iterator provided by the
   * implementation is not at least forward iterator.
   */
  TuplePtr current_;
  /// Indicates if there is a current element to be read
  bool valid_;
  /// Avoid default construction
  GRelationIter(void);
public:
  /// Constructs an iterator on relation \a r
  GRelationIter(const GRelation& r);
  /// Copy constructor
  GRelationIter(const GRelationIter& it);
  /// Destructor
  ~GRelationIter(void);
  /// Tests whether the iterator is still valid
  bool operator()(void) const;
  /// Returns the current value under iteration
  Tuple val(void) const;
  /// Advances the iterator
  void operator++(void);
};
}}
#endif
