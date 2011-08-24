#ifndef __CPREL_PROP_AND_HH__
#define __CPREL_PROP_AND_HH__

#include <cprel/cprel.hh>
#include <cprel/prop/equality.hh>

namespace MPG { namespace CPRel { namespace Prop {
/**
 * \brief Intersection propagator between two relations
 * \ingroup SetProp
 */
template <typename View0, typename View1, typename View2>
class Intersect : public Gecode::Propagator {
protected:
  /// First relation
  View0 a_;
  /// Second relation
  View1 b_;
  /// Third relation
  View2 c_;
public:
  /// Constructor
  Intersect(Gecode::Home home, View0 a, View1 b,View2 c);
  /**
   * \brief Intersect propagator posting
   *
   * \todo In order to make this method smarter, we can detect if two
   * of the views are the same and in that case prefer to post the
   * equality constraint. This will be only possible when the function
   * \a same is defined for all the views (i.e. Complement and
   * Constant). For now this function is missing.
   */
  static Gecode::ExecStatus
  post(Gecode::Home home, View0 a, View1 b, View2 c);

  virtual size_t dispose(Gecode::Space& home);

  Intersect(Gecode::Space& home, bool share, Intersect& p);

  virtual Gecode::Propagator* copy(Gecode::Space& home, bool share);

  virtual Gecode::PropCost cost(const Gecode::Space&,
                                const Gecode::ModEventDelta&) const;

  virtual Gecode::ExecStatus propagate(Gecode::Space& home,
                                       const Gecode::ModEventDelta&);
};
}}}

#include <cprel/prop/and.hpp>

#endif
