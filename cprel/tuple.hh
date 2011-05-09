#ifndef __CPREL_CPREL_TUPLE_HH__
#define __CPREL_CPREL_TUPLE_HH__

#include <vector>
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
  /// Arity of the tuple
  int arity(void) const { return arity_; }
  /// Iterator to the start of the tuple
  vector<int>::const_iterator begin(void) const {
    return data_.begin();
  }
  /// Iterator to the end of the tuple
  vector<int>::const_iterator end(void) const {
    return data_.end();
  }
};
}
#endif
