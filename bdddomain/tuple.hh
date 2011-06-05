#ifndef __CPREL_CPREL_TUPLE_HH__
#define __CPREL_CPREL_TUPLE_HH__

#include <cassert>
#include <vector>
#include <set>
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
  /// Constructor for an empty tuple of \a k elements
  Tuple(int k) {
    data_.reserve(k);
    arity_ = k;
    // initialize all the elements to -1
    for(int i = 0; i < arity_; i++)
      data_.push_back(-1);
  }
  /// Copy constructor
  Tuple(const Tuple& t)
    : data_(t.data_), arity_(t.arity_) {}
  /// Destructor
  ~Tuple(void) {}
  /// Access to the \a i element of the tuple
  int& operator[](int i) {
    assert(i>=0 && i<arity_);
    return data_[i];
  }
  /// Access to the \a i element of the tuple
  int at(int i) const {
    assert(i>=0 && i<arity_);
    return data_.at(i);
  }
  /// Arity of the tuple
  int arity(void) const { return arity_; }
};

/// Creates a binary tuple with \a a and \a b
inline
Tuple make_Tuple(int a, int b) {
  Tuple t(2);
  t[0] = a; t[1] = b;
  return t;
}

/// Creates a ternary tuple with \a a, \a b and \a c
inline
Tuple make_Tuple(int a, int b, int c) {
  Tuple t(3);
  t[0] = a; t[1] = b; t[2] = c;
  return t;
}

/**
 * \brief Outputs tuple \a t to \a os
 * \ingroup TupleGroup
 */
inline
std::ostream& operator << (std::ostream& os, const Tuple& t) {
  os << "[";
  for (int i = 0; i < t.arity(); i++) {
    os << t.at(i);
    if (i < t.arity()-1)
      os << ",";
  }
  os << "]";
  return os;

}
}}
#endif
