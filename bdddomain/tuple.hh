#ifndef __CPREL_CPREL_TUPLE_HH__
#define __CPREL_CPREL_TUPLE_HH__

#include <cassert>
#include <vector>
#include <iostream>
#include <bdddomain/manager.hh>

namespace MPG { namespace CPRel {
/**
 * \defgroup TupleGroup Tuples
 *
 * Tuples are the abstraction for the elements of relations.
 */

/**
 * \brief Class to abstract a tuple in a relation
 * \ingroup TupleGroup
 *
 * \todo For now a tuple is represented as a vector of elements and this can be
 * better if we use pimp and usa abdd for instance to store it.
 */
class Tuple {
private:
  /// Actual data container
  std::vector<int> data_;
  /// Arity of the tuple
  int arity_;
  /// Avoiding Default constructor
  Tuple(void);
  /// Returns a BDD representation for the encoding of \a p in column \a a
  static DdNode* encode(int p, int a);
public:
  /// Returns a BDD representing \a this
  DdNode* encode(void) const;
public:
  typedef std::vector<int>::const_iterator iterator;
  /**
   * \brief Construct a tuple with all the elements present in \a v. The arity
   * of the tuple is the size of the vector.
   */
  explicit Tuple(const std::vector<int>& v);
  /// Constructor for an empty tuple of \a k elements
  Tuple(int k);
  /// Copy constructor
  Tuple(const Tuple& t);
  /// Destructor
  ~Tuple(void);
  /// Returns a vector with all the elements in the tuple
  std::vector<int> value(void) const;
  /// Arity of the tuple
  int arity(void) const { return arity_; }
};

/// Creates a binary tuple with \a a and \a b
inline
Tuple make_Tuple(int a, int b) {
  std::vector<int> v;
  v.reserve(2);
  v.push_back(a); v.push_back(b);
  return Tuple(v);
}

/// Creates a ternary tuple with \a a, \a b and \a c
inline
Tuple make_Tuple(int a, int b, int c) {
  std::vector<int> v;
  v.reserve(3);
  v.push_back(a); v.push_back(b); v.push_back(c);
  return Tuple(v);
}

/**
 * \brief Outputs tuple \a t to \a os
 * \ingroup TupleGroup
 */
inline
std::ostream& operator << (std::ostream& os, const Tuple& t) {
  os << "[";
  const std::vector<int>& v = t.value();
  for (unsigned int i = 0; i < v.size(); i++) {
    os << v.at(i);
    if (i < v.size()-1)
      os << ",";
  }
  os << "]";
  return os;

}
}}
#endif
