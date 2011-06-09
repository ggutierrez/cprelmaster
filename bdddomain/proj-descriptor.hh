#ifndef __CPREL_CPREL_PROJ_DESCRIPTOR_HH__
#define __CPREL_CPREL_PROJ_DESCRIPTOR_HH__

#include <bdddomain/exception.hh>
#include <set>

namespace MPG { namespace CPRel {

/// Exception indicating invalid projection description
struct InvalidProjDescriptor : virtual ExceptionBase {};

/**
 * \brief Describes a permutation on a relation
 * \ingroup GRelation
 *
 * A Permutation descriptor describes the way to perform a given permutation on
 * a relation.
 * \todo example!
 */
class ProjDescriptor {
private:
  /// Columns to project on
  std::vector<int> columns_;
public:
  /// Default constructor for an empty projection
  ProjDescriptor(void) {}
  /// Constructor for a projection on the columns specified by \a v
  ProjDescriptor(const std::vector<int>& v)
    : columns_(v) {
    std::sort(columns_.begin(),columns_.end());
  }
  /// Copy constructor
  ProjDescriptor(const ProjDescriptor& d)
    : columns_(d.columns_) {}
  /// returns the size of the projection (number of columns to project on)
  unsigned int size(void) const { return columns_.size(); }
  /**
   * \brief Returns the columns that are not part of the projection for a relation
   * of arity \a a.
   */
  std::vector<int> complement(int a) const {
    std::vector<int> all, result;
    all.reserve(columns_.size());

    for (int i = 0 ; i < a; i++) all.push_back(i);

    std::set_difference(all.begin(), all.end(), columns_.begin(), columns_.end(),
                        back_inserter(result));

    return result;
  }
  /**
   * \brief Tests whether the description is valid for a relation of arity \a a
   *
   * - It specifies valid columns for a relation of arity \a a
   */
  bool valid(int a) const {
    if (a < static_cast<int>(size()))
      return false;

    for (unsigned int i = 0; i < columns_.size(); i++)
      if (columns_[i] >= a) return false;
    return true;
  }
};
}}

#endif
