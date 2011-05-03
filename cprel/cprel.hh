#ifndef __MPG_INT_HH__
#define __MPG_INT_HH__
#include <iostream>
#include <sstream>
#include <climits>

#include <gecode/kernel.hh>

using Gecode::Advisor;
using Gecode::ConstView;
using Gecode::Delta;
using Gecode::DerivedView;
using Gecode::Exception;
using Gecode::ModEvent;
using Gecode::ModEventDelta;
using Gecode::PropCond;
using Gecode::Propagator;
using Gecode::Space;
using Gecode::VarArgArray;
using Gecode::VarArray;
using Gecode::VarImpVar;
using Gecode::VarImpView;


// exceptions
namespace MPG { namespace CPRel {

class OutOfLimits : public Exception {
public:
  OutOfLimits(const char* l)
    : Exception(l,"Number out of limits") {}
};

class VariableEmptyDomain : public Exception {
public:
  VariableEmptyDomain(const char* l)
    : Exception(l,"Attempt to create variable with empty domain") {}
};

}}

// variable implementation
namespace MPG { namespace CPRel {

// limits
namespace Limits {
const int max = (INT_MAX / 2) - 1;
const int min = -max;
}
// delta for advisors
class CPRelDelta : public Delta {
private:
  int l, u;
public:
  CPRelDelta(int min, int max) : l(min), u(max) {}
  int min(void) const {
    return l;
  }
  int max(void) const {
    return u;
  }
};

class CPRelVarImp : public CPRelVarImpBase {
protected:
  int l, u;
public:
  CPRelVarImp(Space& home, int min, int max)
    : CPRelVarImpBase(home), l(min), u(max) {}
  /// Dispose
  void dispose(Space&) {
    std::cout << "Disposer was called" << std::endl;
  }
  // access operations
  int min(void) const {
    return l;
  }
  int max(void) const {
    return u;
  }
  // assignment test
  bool assigned(void) const {
    return l == u;
  }
  // modification operations
  ModEvent lq(Space& home, int n) {
    if (n >= u) return ME_CPREL_NONE;
    if (n < l) return ME_CPREL_FAILED;
    CPRelDelta d(n+1,u); u = n;
    return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MAX, d);
  }
  ModEvent gq(Space& home, int n) {
    if (n <= l) return ME_CPREL_NONE;
    if (n > u) return ME_CPREL_FAILED;
    CPRelDelta d(l,n-1); l = n;
    return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MIN, d);
  }
  // subscriptions
  void subscribe(Space& home, Propagator& p, PropCond pc,
                 bool schedule=true) {
    CPRelVarImpBase::subscribe(home,p,pc,assigned(),schedule);
  }
  void subscribe(Space& home, Advisor& a) {
    CPRelVarImpBase::subscribe(home,a,assigned());
  }
  void cancel(Space& home, Propagator& p, PropCond pc) {
    CPRelVarImpBase::cancel(home,p,pc,assigned());
  }
  void cancel(Space& home, Advisor& a) {
    CPRelVarImpBase::cancel(home,a,assigned());
  }
  // copying
  CPRelVarImp(Space& home, bool share, CPRelVarImp& y)
    : CPRelVarImpBase(home,share,y), l(y.l), u(y.u) {}
  CPRelVarImp* copy(Space& home, bool share) {
    if (copied())
      return static_cast<CPRelVarImp*>(forward());
    else
      return new (home) CPRelVarImp(home,share,*this);
  }
  // delta information
  static int min(const Delta& d) {
    return static_cast<const CPRelDelta&>(d).min();
  }
  static int max(const Delta& d) {
    return static_cast<const CPRelDelta&>(d).max();
  }
};

}}

// variable
namespace MPG {

class CPRelVar : public VarImpVar<CPRel::CPRelVarImp> {
protected:
  using VarImpVar<CPRel::CPRelVarImp>::x;
public:
  CPRelVar(void) {}
  CPRelVar(const CPRelVar& y)
    : VarImpVar<CPRel::CPRelVarImp>(y.varimp()) {}
  CPRelVar(CPRel::CPRelVarImp *y)
    : VarImpVar<CPRel::CPRelVarImp>(y) {}

  // variable creation
  CPRelVar(Space& home, int min, int max)
    : VarImpVar<CPRel::CPRelVarImp>
      (new (home) CPRel::CPRelVarImp(home,min,max)) {
    if ((min < CPRel::Limits::min) || (max > CPRel::Limits::max))
      throw CPRel::OutOfLimits("CPRelVar::CPRelVar");
    if (min > max)
      throw CPRel::VariableEmptyDomain("CPRelVar::CPRelVar");
  }
  // access operations
  int min(void) const {
    return x->min();
  }
  int max(void) const {
    return x->max();
  }
};

template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const CPRelVar& x) {
  std::basic_ostringstream<Char,Traits> s;
  s.copyfmt(os); s.width(0);
  if (x.assigned())
    s << x.min();
  else
    s << '[' << x.min() << ".." << x.max() << ']';
  return os << s.str();
}

}
// array traits
namespace MPG {
class CPRelVarArgs; class CPRelVarArray;
}

namespace Gecode {

template<>
class ArrayTraits<Gecode::VarArray<MPG::CPRelVar> > {
public:
  typedef MPG::CPRelVarArray  StorageType;
  typedef MPG::CPRelVar       ValueType;
  typedef MPG::CPRelVarArgs   ArgsType;
};
template<>
class ArrayTraits<MPG::CPRelVarArray> {
public:
  typedef MPG::CPRelVarArray  StorageType;
  typedef MPG::CPRelVar       ValueType;
  typedef MPG::CPRelVarArgs   ArgsType;
};
template<>
class ArrayTraits<Gecode::VarArgArray<MPG::CPRelVar> > {
public:
  typedef MPG::CPRelVarArgs   StorageType;
  typedef MPG::CPRelVar       ValueType;
  typedef MPG::CPRelVarArgs   ArgsType;
};
template<>
class ArrayTraits<MPG::CPRelVarArgs> {
public:
  typedef MPG::CPRelVarArgs  StorageType;
  typedef MPG::CPRelVar      ValueType;
  typedef MPG::CPRelVarArgs  ArgsType;
};

}
// variable arrays
namespace MPG {

class CPRelVarArgs : public VarArgArray<CPRelVar> {
public:
  CPRelVarArgs(void) {}
  explicit CPRelVarArgs(int n) : VarArgArray<CPRelVar>(n) {}
  CPRelVarArgs(const CPRelVarArgs& a) : VarArgArray<CPRelVar>(a) {}
  CPRelVarArgs(const VarArray<CPRelVar>& a) : VarArgArray<CPRelVar>(a) {}
  CPRelVarArgs(Space& home, int n, int min, int max)
    : VarArgArray<CPRelVar>(n) {
    for (int i=0; i<n; i++)
      (*this)[i] = CPRelVar(home,min,max);
  }
};

class CPRelVarArray : public VarArray<CPRelVar> {
public:
  CPRelVarArray(void) {}
  CPRelVarArray(const CPRelVarArray& a)
    : VarArray<CPRelVar>(a) {}
  CPRelVarArray(Space& home, int n, int min, int max)
    : VarArray<CPRelVar>(home,n) {
    for (int i=0; i<n; i++)
      (*this)[i] = CPRelVar(home,min,max);
  }
};

}

// integer view
namespace MPG { namespace CPRel {

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
  int min(void) const {
    return x->min();
  }
  int max(void) const {
    return x->max();
  }
  // modification operations
  ModEvent lq(Space& home, int n) {
    return x->lq(home,n);
  }
  ModEvent gq(Space& home, int n) {
    return x->gq(home,n);
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
    s << x.min();
  else
    s << '[' << x.min() << ".." << x.max() << ']';
  return os << s.str();
}

}}
// constant integer view
namespace MPG { namespace CPRel {

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
  // update during cloning
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

void branch(Gecode::Home home, const CPRelVarArgs& x);
}


#endif
