#include <gecode/kernel.hh>
#include <gecode/int.hh>

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

  /// Variable implementation
  class RelVarImp : public RelVarImpBase {
  protected:
    int l, u;
  public:
    /// \name Constructors
    RelVarImp(Gecode::Space& home, int min, int max)
      : RelVarImpBase(home), l(min), u(max) {}
    /// \name Copying
    RelVarImp(Gecode::Space& home, bool share, RelVarImp& y)
      : RelVarImpBase(home, share, y), l(y.l), u(y.u) {}
    RelVarImp* copy(Gecode::Space& home, bool share) {
      if (copied())
        return static_cast<RelVarImp*>(forward());
      else
        return new (home) RelVarImp(home,share,*this);
    }
    /// \name Access operations
    int min(void) const { return l; }
    int max(void) const { return u; }
    /// Assignment test
    bool assigned(void) const { return l == u; }
    /// Modification operations
    Gecode::ModEvent lq(Gecode::Space& home, int n) {
      if (n >= u) return ME_REL_NONE;
      if (n < l) return ME_REL_FAILED;
      RelDelta d;
      u = n;
      return notify(home, assigned() ? ME_REL_VAL : ME_REL_LUB, d);
    }
    /// Propagators events
    void subscribe(Gecode::Space& home, Gecode::Propagator& p, Gecode::PropCond pc,
                   bool schedule=true) {
      RelVarImpBase::subscribe(home,p,pc,assigned(),schedule);
    }
    void cancel(Gecode::Space& home, Gecode::Propagator& p, Gecode::PropCond pc) {
      RelVarImpBase::cancel(home,p,pc,assigned());
    }
    /// Advaisor events
    void subscribe(Gecode::Space& home, Gecode::Advisor& a) {
      RelVarImpBase::subscribe(home,a,assigned());
    }
    void cancel(Gecode::Space& home, Gecode::Advisor& a) {
      RelVarImpBase::cancel(home,a,assigned());
    }
    /// Delta information
  };
}}

namespace MPG {
  class RelVar : public Gecode::VarImpVar<Rel::RelVarImp> {
  protected:
    using Gecode::VarImpVar<Rel::RelVarImp>::x;
  public:
    /// Constructors
    RelVar(void) {}
    /// Copy constructor
    RelVar(const RelVar& y)
    : Gecode::VarImpVar<Rel::RelVarImp>(y.varimp()) {}
    /// Variable creation
    RelVar(Gecode::Space& home, int min, int max)
      : Gecode::VarImpVar<Rel::RelVarImp>(new (home) Rel::RelVarImp(home,min,max)) {
      std::cout << "Created variable " << min << " to " << max << std::endl;
    }
    /// Access operations
    int min(void) const {
      return x->min();
    }
  };

  class RelVarArgs;
  class RelVarArray;
}

namespace Gecode {
  template<>
  class ArrayTraits<Gecode::VarArray<MPG::RelVar> > {
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
  class ArrayTraits<Gecode::VarArgArray<MPG::RelVar> > {
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
  class RelVarArgs : public Gecode::VarArgArray<RelVar> {
  public:
    RelVarArgs(void) {}
    explicit RelVarArgs(int n) : Gecode::VarArgArray<RelVar>(n) {}
    RelVarArgs(const RelVarArgs& a) : Gecode::VarArgArray<RelVar>(a) {}
    RelVarArgs(const Gecode::VarArray<RelVar>& a) : Gecode::VarArgArray<RelVar>(a) {}
    RelVarArgs(Gecode::Space& home, int n, int min, int max)
      : Gecode::VarArgArray<RelVar>(n) {
      for (int i=0; i<n; i++)
        (*this)[i] = RelVar(home,min,max);
    }
  };

  class RelVarArray : public Gecode::VarArray<RelVar> {
  public:
    RelVarArray(void) {}
    RelVarArray(const RelVarArray& a)
      : Gecode::VarArray<RelVar>(a) {}
    RelVarArray(Gecode::Space& home, int n, int min, int max)
      : Gecode::VarArray<RelVar>(home,n) {
      for (int i=0; i<n; i++)
        (*this)[i] = RelVar(home,min,max);
    }
  };
}
