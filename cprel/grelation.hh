#ifndef __CPREL_CPREL_GRELATION_HH__
#define __CPREL_CPREL_GRELATION_HH__

#include <boost/shared_ptr.hpp>
#include <cprel/tuple.hh>

namespace CPRel {

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
  /// Constructor for an empty relation of arity \a a
  GRelation(int a);
  /// Copy constructor
  GRelation(const GRelation& r);
  /// Assignment
  GRelation& operator=(GRelation& right);
  /// Destructor
  ~GRelation(void);
  /// Adds a tuple \a t to the relation
  bool add(const Tuple& t);
  /// Returns the arity of the relation
  int arity(void) const;
  /// Returns the cardinality of the relation
  double cardinality(void) const;
  /// Modify this relation to be the union of this and \a r
  bool unionAssign(const GRelation& r);
};

/// Creates a relation with the elements contained in \a dom
GRelation create(const std::vector<Tuple>& dom);

}
#endif
