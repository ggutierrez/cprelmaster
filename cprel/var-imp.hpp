namespace MPG { namespace Rel {

/// Variable implementation
class RelVarImp : public RelVarImpBase {
protected:
  int l, u;
public:
  /// \name Constructors
  //@{
  RelVarImp(Space& home, int min, int max);
  //@}
  /// \name Cloning
  //@{
  /// Copy constructor
  RelVarImp(Space& home, bool share, RelVarImp& y);
  /// Copy
  RelVarImp* copy(Space& home, bool share);
  //@}
  /// Disposal
  void dispose(Space&);
  /// \name Access operations
  //@{
  int min(void) const;
  int max(void) const;
  //@}
  /// Assignment test
  bool assigned(void) const;
  /// \name Modification operations
  //@{
  ModEvent lq(Space& home, int n);
  ModEvent gq(Space& home, int n);
  //@}
  /// \name Events
  //@{
  /// Propagators events
  void subscribe(Space& home, Propagator& p, PropCond pc, bool schedule=true);
  void cancel(Space& home, Propagator& p, PropCond pc);
  /// Advaisor events
  void subscribe(Space& home, Advisor& a);
  void cancel(Space& home, Advisor& a);
  //@}
  /// Delta information
  };
}}

#include <cprel/var-imp/rel.hpp>
