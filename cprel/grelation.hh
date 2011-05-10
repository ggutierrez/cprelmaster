#ifndef __CPREL_CPREL_GRELATION_HH__
#define __CPREL_CPREL_GRELATION_HH__

#include <boost/shared_ptr.hpp>
#include <cprel/tuple.hh>

namespace MPG { namespace CPRel {

namespace VarImpl {
  class RelationImpl;
}

/// Class representing a ground relation
class GRelation {
private:
  /// Relation storage
  boost::shared_ptr<VarImpl::RelationImpl> pimpl_;
  /// Avoid default construction
  GRelation(void);
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
  /// \name Test operations
  //@{
  /// Tests whether this relation is a subset of \a r
  bool subset(const GRelation& r) const;
  /// Tests whether this represents the same relation as \a r
  bool eq(const GRelation& r) const;
  //@}
  /// \name Information
  //@{
  /// Returns the arity of the relation
  int arity(void) const;
  /// Returns the cardinality of the relation
  double cardinality(void) const;
  //@}
};

/// Creates a relation with the elements contained in \a dom
GRelation create(const std::vector<Tuple>& dom);

/// Outputs relation \a r to \a os
std::ostream& operator<< (std::ostream& os, const GRelation& r);
}}
#endif
