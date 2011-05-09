#ifndef __CPREL_CPREL_TUPLE_HH__
#define __CPREL_CPREL_TUPLE_HH__

#include <cassert>
#include <vector>
#include <iostream>

namespace CPRel {

using std::vector;

/// Class to abstract a tuple in a relation
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
    data_.resize(k);
    arity_ = k;
    // initialize all the elements to -1
    for(int i = 0; i < arity_; i++) data_[i] = -1;
  }
  /// Constructor for a binary tuple
  Tuple(int a, int b) {
    data_.resize(2);
    data_[0] = a;
    data_[1] = b;
    arity_ = 2;
  }
  /// Copy constructor
  Tuple(const Tuple& t)
    : data_(t.data_) {}
  /// Destructor
  ~Tuple(void) {}
  /// Access to the \a i element of the tuple
  int& operator[](int i) {
    assert(i>=0 && i<arity_);
    return data_[i];
  }

  int operator[](int i) const {
    assert(i>=0 && i<arity_);
    return data_[i];
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

inline
std::ostream& operator << (std::ostream& os, const Tuple& t) {
  os << "[";
  for (int i = 0; i < t.arity(); i++) {
    os << t[i];
    if (i < t.arity()-1)
      os << ",";
  }
  os << "]";
  return os;
}

}
#endif
