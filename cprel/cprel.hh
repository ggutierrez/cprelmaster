#ifndef __CPREL_CPREL_HH__
#define __CPREL_CPREL_HH__

#include <iostream>
#include <sstream>
#include <climits>

#include <gecode/kernel.hh>
#include <cprel/grelation.hh>

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

/**
 * \brief Relation domain.
 *
 * A relation domain is approximated by means of a greatest lower
 * bound \c glb and a least upper bound \c lub.
 */
class CPRelVarImp : public CPRelVarImpBase {
protected:
  /// Greatest lower bound
  GRelation glb_;
  /// Least upper bound
  GRelation lub_;
public:
  /// \a Constructors and disposer
  //@{
  /// Constructor for a variable with empty lower bound and
  CPRelVarImp(Space& home, const GRelation& l, const GRelation& u)
    : CPRelVarImpBase(home), glb_(l), lub_(u) {}
  /// Resources disposal
  void dispose(Space&) {
//    std::cout << "Starting disposal" << std::endl;
    glb_.~GRelation();
//    std::cerr << "--disposing--" << std::endl;
    lub_.~GRelation();
//    std::cout << "Finishing disposal" << std::endl;
  }
  //@}
  /// \name Bound access
  //@{
  /// Returns a relation representing the greatest lower bound of the variable
  GRelation glb(void) const {
    return glb_;
  }
  /// Returns a relation representing the least upper bound of the variable
  GRelation lub(void) const {
    return lub_;
  }
  //@}
  /// \name Pruning operations
  //@{
  /**
   * \brief Prune the variable by doing: \f$ glb = glb \cup r \f$
   *
   */
  ModEvent include(Space& home, const GRelation& r) {
    if (r.superset(lub_)) return ME_CPREL_FAILED;
    if (r.subsetEq(glb_)) return ME_CPREL_NONE;
    glb_.unionAssign(r);
    CPRelDelta d(1,2);
    return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MIN, d);
  }
  /**
   * \brief Prune the variable by doing: \f$ lub = lub \setminus r \f$
   *
   */
  ModEvent exclude(Space& home, const GRelation& r) {
    if (!r.disjoint(glb_)) return ME_CPREL_FAILED;
    if (r.disjoint(lub_)) return ME_CPREL_NONE;
    lub_.differenceAssign(r);
    CPRelDelta d(1,2);
    return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MAX, d);
  }
  //@}
  /// \name Domain tests
  //@{
  /// Tests for assignment \f$ glb = lub \f$
  bool assigned(void) const {
    return glb_.eq(lub_);
  }
  //@}
  /// \name Subscriptions handling
  //@{
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
  //@}
  /// \name Copying
  //@{
  CPRelVarImp(Space& home, bool share, CPRelVarImp& y)
    : CPRelVarImpBase(home,share,y), glb_(y.glb_), lub_(y.lub_) {}

  CPRelVarImp* copy(Space& home, bool share) {
    if (copied())
      return static_cast<CPRelVarImp*>(forward());
    else
      return new (home) CPRelVarImp(home,share,*this);
  }
  //@}
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
  CPRelVar(Space& home, const CPRel::GRelation& l, const CPRel::GRelation& u)
    : VarImpVar<CPRel::CPRelVarImp>
      (new (home) CPRel::CPRelVarImp(home,l,u)) {
    if (!l.subsetEq(u))
      throw CPRel::VariableEmptyDomain("CPRelVar::CPRelVar");
  }
  // access operations
  CPRel::GRelation glb(void) const {
    return x->glb();
  }
  CPRel::GRelation lub(void) const {
    return x->lub();
  }
};

template<class Char, class Traits>
std::basic_ostream<Char,Traits>&
operator <<(std::basic_ostream<Char,Traits>& os, const CPRelVar& x) {
  std::basic_ostringstream<Char,Traits> s;
  s.copyfmt(os); s.width(0);
  if (x.assigned())
    s << "val:{" << x.glb() << "}#" << "C";
  else
    s << "glb:{" << x.glb() << "}, unk:{"
      << x.lub().difference(x.glb()) << "}";
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
  CPRelVarArgs(Space& home, int n,
               const CPRel::GRelation& l, const CPRel::GRelation& u)
    : VarArgArray<CPRelVar>(n) {
    for (int i=0; i<n; i++)
      (*this)[i] = CPRelVar(home,l,u);
  }
};

class CPRelVarArray : public VarArray<CPRelVar> {
public:
  CPRelVarArray(void) {}
  CPRelVarArray(const CPRelVarArray& a)
    : VarArray<CPRelVar>(a) {}
  CPRelVarArray(Space& home, int n,
                const CPRel::GRelation& l, const CPRel::GRelation& u)
    : VarArray<CPRelVar>(home,n) {
    for (int i=0; i<n; i++)
      (*this)[i] = CPRelVar(home,l,u);
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
  GRelation glb(void) const {
    return x->glb();
  }
  GRelation lub(void) const {
    return x->lub();
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

void branch(Gecode::Home home, CPRelVar x);
}


#endif
