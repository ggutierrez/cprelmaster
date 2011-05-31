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
/// Exception indicating invalid permutation description
//class InvalidPermDescriptor : public Exception {
//public:
//  InvalidPermDescriptor(const char* l)
//    : Exception(l,"Invalid permutation description") {}
//};

class DescIterator;
/**
 * \brief Describes a permutation on a relation
 * \ingroup GRelation
 *
 * A Permutation descriptor describes the way to perform a given permutation on a relation. It
 * \todo example!
 */
class PermDescriptor {
private:
  friend class DescIterator;
  /// Columns in the original relation
  std::vector<int> origin_;
  /// Corresponding columns in the permuted relation
  std::vector<int> permute_;
public:
  /// Default constructor for an empty description
  PermDescriptor(void) {}
  /// Copy constructor
  PermDescriptor(const PermDescriptor& d)
    : origin_(d.origin_), permute_(d.permute_) {}
  /// returns the size of the description
  unsigned int size(void) const { return origin_.size(); }
  /**
   * \brief Augments the description to contain the permutation of column \a x
   * by column \a y.
   */
  void permute(int x, int y) {
    // \todo Check if x and y already exist in the description and rise an exception
    origin_.push_back(x);
    permute_.push_back(y);
    assert(origin_.size() == permute_.size()
           && "Invalid state of permutation descriptor");
  }
};
/**
 * \brief Iterator on a permutation descriptor.
 * \ingroup GRelation
 */
class DescIterator {
private:
  /// Reference to the iterated description
  const PermDescriptor& desc_;
  /// Next permutation to be iterated
  unsigned int next_;
public:
  /// Constructor
  DescIterator(const PermDescriptor& desc)
    : desc_(desc), next_(0) {}
  /// Return a pair representing the current permutation
  std::pair<int,int> val(void) const {
    return std::make_pair(desc_.origin_.at(next_),
                          desc_.permute_.at(next_));
  }
  /// Increments the iterator
  void operator++(void) {
    next_++;
  }
  /// Test if the iterator is still valid
  bool operator()(void) const {
    return next_ < desc_.origin_.size();
  }
};

}}
#endif
