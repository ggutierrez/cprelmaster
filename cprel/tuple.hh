#ifndef __CPREL_CPREL_TUPLE_HH__
#define __CPREL_CPREL_TUPLE_HH__

#include <cassert>
#include <vector>
#include <iostream>

namespace MPG { namespace CPRel {
/**
 * \defgroup TupleGroup Tuples
 *
 * Tuples are the abstraction for the elements of relations.
 */

using std::vector;

/**
 * \brief Class to abstract a tuple in a relation
 * \ingroup TupleGroup
 */
class Tuple {
private:
  /// Actual data container
  vector<int> data_;
  /// Arity of the tuple
  int arity_;
public:
  typedef vector<int>::const_iterator iterator;
  /// Default constructor
  Tuple(void);
  /// Constructor for an empty tuple of \a k elements
  Tuple(int k) {
    data_.reserve(k);
    arity_ = k;
    // initialize all the elements to -1
    for(int i = 0; i < arity_; i++)
      data_.push_back(-1);
  }
  /// Constructor for a binary tuple
  Tuple(int a, int b) : arity_(2) {
    data_.reserve(2);
    data_.push_back(a);
    data_.push_back(b);
  }
  /// Constructor for a ternary tuple
  Tuple(int a, int b, int c) : arity_(3) {
    data_.reserve(3);
    data_.push_back(a);
    data_.push_back(b);
    data_.push_back(c);
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
  /// Iterator to the start of the tuple
  vector<int>::const_iterator cbegin(void) const {
    return data_.begin();
  }
  /// Iterator to the end of the tuple
  vector<int>::const_iterator cend(void) const {
    return data_.end();
  }
};

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
