
namespace MPG { namespace Rel {
inline
RelVarImp* RelVarImp::copy(Space& home, bool share) {
  if (copied())
    return static_cast<RelVarImp*>(forward());
  else
    return new (home) RelVarImp(home,share,*this);
  }

inline
void RelVarImp::dispose(Space&) {}

inline
int RelVarImp::min(void) const { return l; }

inline
int RelVarImp::max(void) const { return u; }

inline
bool RelVarImp::assigned(void) const { return l == u; }

inline
ModEvent RelVarImp::lq(Space& home, int n) {
  if (n >= u) return ME_REL_NONE;
  if (n < l) return ME_REL_FAILED;
  RelDelta d;
  u = n;
  return notify(home, assigned() ? ME_REL_VAL : ME_REL_LUB, d);
  }

inline
ModEvent RelVarImp::gq(Space& home, int n) {
  if (n <= l) return ME_REL_NONE;
  if (n > u) return ME_REL_FAILED;
  RelDelta d; l = n;
  return notify(home, assigned() ? ME_REL_VAL : ME_REL_GLB, d);
  }

inline
void RelVarImp::subscribe(Space& home, Propagator& p, PropCond pc,
                          bool schedule) {
  RelVarImpBase::subscribe(home,p,pc,assigned(),schedule);
  }

inline
void RelVarImp::cancel(Space& home, Propagator& p, PropCond pc) {
  RelVarImpBase::cancel(home,p,pc,assigned());
  }

inline
void RelVarImp::subscribe(Space& home, Advisor& a) {
  RelVarImpBase::subscribe(home,a,assigned());
  }

inline
void RelVarImp::cancel(Space& home, Advisor& a) {
  RelVarImpBase::cancel(home,a,assigned());
  }
}}
