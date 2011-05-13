#ifndef __CPREL_CPREL_VIEW_HH__
#define __CPREL_CPREL_VIEW_HH__

#include <gecode/kernel.hh>

using Gecode::ConstView;
using Gecode::VarImpView;

namespace MPG { namespace CPRel {
/// Relation view for relation variables
class CPRelView : public VarImpView<CPRelVar> {
protected:
  using VarImpView<CPRelVar>::x;
public:
  CPRelView(void) {}
  CPRelView(const CPRelVar& y)
    : VarImpView<CPRelVar>(y.varimp()) {}
  CPRelView(CPRelVarImp* y)
    : VarImpView<CPRelVar>(y) {}
  // access operations
  GRelation glb(void) const {
    return x->glb();
  }
  GRelation lub(void) const {
    return x->lub();
  }
  GRelation unk(void) const {
    return x->unk();
  }
  // modification operations
  ModEvent include(Space& home, const GRelation& r) {
    return x->include(home,r);
  }
  ModEvent exclude(Space& home, const GRelation& r) {
    return x->exclude(home,r);
  }
  // delta information
  int min(const Delta& d) const {
    return CPRelVarImp::min(d);
  }
  int max(const Delta& d) const {
    return CPRelVarImp::max(d);
  }
};

template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator<<(std::basic_ostream<Char,Traits>& os, const CPRelView& x) {
  std::basic_ostringstream<Char,Traits> s;
  s.copyfmt(os); s.width(0);
  if (x.assigned())
    s << "val:{" << x.glb() << "}#" << "C";
  else
    s << "glb:{" << x.glb() << "}, unk:{"
      << x.lub().difference(x.glb()) << "}";
  return os << s.str();
}

}}

// constant integer view
namespace MPG { namespace CPRel {
/// Constant view on a relation variable
class ConstCPRelView : public ConstView<CPRelView> {
protected:
  int x;
public:
  ConstCPRelView(void) : x(0) {}
  ConstCPRelView(int n) : x(n) {}

  int min(void) const {
    return x;
  }
  int max(void) const {
    return x;
  }
  ModEvent lq(Space&, int n) {
    return (x <= n) ? ME_CPREL_NONE : ME_CPREL_FAILED;
  }
  ModEvent gq(Space&, int n) {
    return (x >= n) ? ME_CPREL_NONE : ME_CPREL_FAILED;
  }
  // delta information
  int min(const Delta&) const {
    GECODE_NEVER; return 0;
  }
  int max(const Delta&) const {
    GECODE_NEVER; return 0;
  }
  /// Update during cloning
  void update(Space& home, bool share, ConstCPRelView& y) {
    ConstView<CPRelView>::update(home,share,y);
    x = y.x;
  }
};
// view tests
inline bool same(const ConstCPRelView& x, const ConstCPRelView& y) {
  return x.min() == y.min();
}
inline bool before(const ConstCPRelView& x, const ConstCPRelView& y) {
  return x.min() < y.min();
}

template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const ConstCPRelView& x) {
  return os << x.min();
}

}

void branch(Gecode::Home home, CPRelVar x);
}



#endif
