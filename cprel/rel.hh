#ifndef __GECODE_CPREL__
#define __GECODE_CPREL__

#include <gecode/kernel.hh>
#include <gecode/int.hh>

using Gecode::Space;
using Gecode::Home;
using Gecode::Brancher;
using Gecode::Choice;
using Gecode::ExecStatus;
using Gecode::PropCond;
using Gecode::Propagator;
using Gecode::Advisor;
using Gecode::VarImpVar;
using Gecode::VarArgArray;
using Gecode::VarArray;
using Gecode::ViewArray;
using Gecode::VarImpView;
using Gecode::ModEvent;

namespace MPG { namespace Rel {

  namespace Limits {
    /// Maximum element that can be member of a tuple in a relation
    const int max = Gecode::Int::Limits::max;
    /// Minum element that can be member of a tuple in a relation
    const int min = 0;
    /// Maximum arity of a relation
    const int arity = 8;
  }

  /**
   * \brief Delta information for advisors
   **/
  class RelDelta : public Gecode::Delta {
  public:
    RelDelta(void) {}
  };
}}

#include <cprel/var-imp.hpp>

namespace MPG {
  class RelVar : public VarImpVar<Rel::RelVarImp> {
  protected:
    using VarImpVar<Rel::RelVarImp>::x;
  public:
    /// Constructors
    RelVar(void);
    /// Copy constructor
    RelVar(const RelVar& y);
    /// Variable creation
    RelVar(Space& home, int min, int max);
    /// Access operations
    int min(void) const;
    int max(void) const;
  };

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const RelVar& x);
}

#include <cprel/var/rel.hpp>

// array traits
namespace MPG {
  class RelVarArgs;
  class RelVarArray;
}

namespace Gecode {
  template<>
  class ArrayTraits<VarArray<MPG::RelVar> > {
  public:
    typedef MPG::RelVarArray StorageType;
    typedef MPG::RelVar ValueType;
    typedef MPG::RelVarArgs ArgsType;
  };

  template<>
  class ArrayTraits<MPG::RelVarArray> {
  public:
    typedef MPG::RelVarArray StorageType;
    typedef MPG::RelVar ValueType;
    typedef MPG::RelVarArgs ArgsType;
  };

  template<>
  class ArrayTraits<VarArgArray<MPG::RelVar> > {
  public:
    typedef MPG::RelVarArgs StorageType;
    typedef MPG::RelVar ValueType;
    typedef MPG::RelVarArgs ArgsType;
  };

  template<>
  class ArrayTraits<MPG::RelVarArgs> {
  public:
    typedef MPG::RelVarArgs StorageType;
    typedef MPG::RelVar ValueType;
    typedef MPG::RelVarArgs ArgsType;
  };
}

namespace MPG {
  class RelVarArgs : public VarArgArray<RelVar> {
  public:
    RelVarArgs(void) {}
    explicit RelVarArgs(int n) : VarArgArray<RelVar>(n) {}
    RelVarArgs(const RelVarArgs& a) : VarArgArray<RelVar>(a) {}
    RelVarArgs(const VarArray<RelVar>& a) : VarArgArray<RelVar>(a) {}
    RelVarArgs(Space& home, int n, int min, int max)
      : VarArgArray<RelVar>(n) {
      for (int i=0; i<n; i++)
        (*this)[i] = RelVar(home,min,max);
    }
  };

  class RelVarArray : public VarArray<RelVar> {
  public:
    RelVarArray(void) {}
    RelVarArray(const RelVarArray& a)
      : VarArray<RelVar>(a) {}
    RelVarArray(Space& home, int n, int min, int max)
      : VarArray<RelVar>(home,n) {
      for (int i=0; i<n; i++)
        (*this)[i] = RelVar(home,min,max);
    }
  };
}

namespace MPG {
  namespace Rel {
    class RelView : public VarImpView<RelVar> {
    protected:
      using VarImpView<RelVar>::x;
    public:
      /// Constructors
      RelView(void) {}
      RelView(const RelVar& y)
        : VarImpView<RelVar>(y.varimp()) {}
      RelView(RelVarImp* y)
        : VarImpView<RelVar>(y) {}
      /// Access operations
      int min(void) const { return x->min(); }
      int max(void) const { return x->max(); }
      /// Modification operations
      ModEvent lq(Space& home, int n) { return x->lq(home,n); }
      ModEvent gq(Space& home, int n) { return x->gq(home,n); }
    };

    template<class Char, class Traits>
    std::basic_ostream<Char,Traits>&
    operator << (std::basic_ostream<Char,Traits>& os, const RelView& x) {
      std::basic_ostringstream<Char,Traits> s;
      s.copyfmt(os); s.width(0);
      if (x.assigned())
        s << x.min();
      else
        s << '[' << x.min() << ".." << x.max() << ']';
      return os << s.str();
    }
  }
}

#include <cprel/branch.hpp>

#endif
