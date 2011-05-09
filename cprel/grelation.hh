#ifndef __CPREL_CPREL_GRELATION_HH__
#define __CPREL_CPREL_GRELATION_HH__

#include <boost/shared_ptr.hpp>
namespace CPRel {
/// Class representing a ground relation
class GRelation {
private:
  class impl;
  /// Relation storage
  boost::shared_ptr<impl> pimpl_;
  /// Avoid default construction
  GRelation(void);
public:
  /// Constructor for a relation of arity \a a
  GRelation(int a);
  /// Destructor
  ~GRelation(void);
};

#endif
