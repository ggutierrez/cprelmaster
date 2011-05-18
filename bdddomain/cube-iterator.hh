#ifndef __CPREL_BDDDOMAIN_CUBE_ITERATOR_HH__
#define __CPREL_BDDDOMAIN_CUBE_ITERATOR_HH__

#include <bdddomain/rel-impl.hh>
#include <cudd/cuddInt.h>

namespace MPG { namespace CPRel { namespace VarImpl {
/**
 * \brief Class to iterate on the cubes of a relation implementation
 * \ingroup DomRepr
 *
 * A cube of a relation represents possibly severla tuples that are part of the
 * relation. Iterating on the cubes of a relation representation is in the worst
 * case as iterating on the tuples but in other cases, cubes offer a more compact
 * representation of what is in the relation.
 */
class CubeIterator {
private:
  /// The BDD with the representation of the relation being iterated
  DdNode *relation_;
  /// The arity of the relation
  int arity_;
  /// Avoid default construction
  CubeIterator(void);
  /// Removes \a t from the iterated relation
  void remove(const Tuple& t);
public:
  /// Constructor
  CubeIterator(DdNode *rel, int a);
  /// Copy constructor
  CubeIterator(const CubeIterator&);
  /// Destructor
  ~CubeIterator(void);
  /// Return a current tuple
  std::vector<int> val(void);
  /// Tests whether the iterator is still valid
  bool operator()(void) const;
};
}}}
#endif
