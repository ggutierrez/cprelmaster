#ifndef __CPREL_CPREL_PERM_DESCRIPTOR_HH__
#define __CPREL_CPREL_PERM_DESCRIPTOR_HH__

#include <bdddomain/exception.hh>
#include <set>

namespace MPG {

/// Exception indicating invalid permutation description
struct InvalidPermDescriptor : virtual ExceptionBase {};

class DescIterator;
/**
 * \brief Describes a permutation on a relation
 * \ingroup GRelation
 *
 * A Permutation descriptor describes the way to perform a given permutation on
 * a relation.
 * \todo example!
 */
class PermDescriptor {
private:
  friend class DescIterator;
  /// Columns in the original relation
  std::vector<int> origin_;
  /// Corresponding columns in the permuted relation
  std::vector<int> permute_;
  /// Constructor from two vectors containing the description
  PermDescriptor(const std::vector<int>& origin, const std::vector<int>& perm)
    : origin_(origin), permute_(perm){}
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
    /// \todo Check if x and y already exist in the description and rise an exception
    origin_.push_back(x);
    permute_.push_back(y);
    assert(origin_.size() == permute_.size()
           && "Invalid state of permutation descriptor");
  }
  /// Returns the inverse of the permutation
  PermDescriptor inverse(void) const {
    return PermDescriptor(permute_,origin_);
  }
  /**
   * \brief Tests whether the description is valid for a relation of arity \a a
   *
   * - It specifies valid columns for a relation of arity \a a
   * - It does not contain the same column in defferent pairs.
   */
  bool valid(int a) const {
    using namespace std;
    if (a < static_cast<int>(size()))
      return false;
    set<int> domain, range;
    copy(origin_.begin(), origin_.end(),
              std::inserter(domain,domain.begin()));
    copy(permute_.begin(), permute_.end(),
              std::inserter(range,range.begin()));
    if (domain.size() != range.size())
      return false;
    return true;
  }
  /// Return the permutation as an array of pairs
  std::vector<std::pair<int,int>> getPerm(void) const {
    std::vector<std::pair<int,int>> p;
    for (unsigned int i = 0; i < origin_.size(); i++) {
      p.push_back({origin_.at(i),permute_.at(i)});
    }
    return p;
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
}

#endif
