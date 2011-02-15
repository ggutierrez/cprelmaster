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
    /// \name Disposal
    void dispose(Gecode::Space& home) {
      // TODO: this is not working for the moment.
      std::cout << "Disposed variable" << std::endl;
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
    Gecode::ModEvent gq(Gecode::Space& home, int n) {
      if (n <= l) return ME_REL_NONE;
      if (n > u) return ME_REL_FAILED;
      RelDelta d; l = n;
      return notify(home, assigned() ? ME_REL_VAL : ME_REL_GLB, d);
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
    int max(void) const {
    return x->max();
    }
  };

  template<class Char, class Traits>
  std::basic_ostream<Char,Traits>&
  operator <<(std::basic_ostream<Char,Traits>& os, const RelVar& x) {
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

namespace MPG {
  namespace Rel {
    class RelView : public Gecode::VarImpView<RelVar> {
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
      Gecode::ModEvent lq(Gecode::Space& home, int n) { return x->lq(home,n); }
      Gecode::ModEvent gq(Gecode::Space& home, int n) { return x->gq(home,n); }
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

namespace MPG {
  class NoneAny : public Gecode::Brancher {
  protected:
    Gecode::ViewArray<Rel::RelView> x;
    class PosVal : public Gecode::Choice {
    public:
      int pos; int val;
      PosVal(const NoneAny& b, int p, int v)
        : Choice(b,2), pos(p), val(v) {}
      virtual size_t size(void) const { return sizeof(*this); }
    };
  public:
  NoneAny(Gecode::Home home, Gecode::ViewArray<Rel::RelView>& x0)
    : Brancher(home), x(x0) {}
    static void post(Gecode::Home home, Gecode::ViewArray<Rel::RelView>& x) {
      (void) new (home) NoneAny(home,x);
    }

    virtual bool status(const Gecode::Space& home) const {
      for (int i=0; i<x.size();i++)
        if (!x[i].assigned())
          return true;
      return false;
    }

    virtual Gecode::Choice* choice(Gecode::Space& home) {
      for (int i=0; true; i++)
        if (!x[i].assigned())
          return new PosVal(*this,i,x[i].min());
      GECODE_NEVER;
      return NULL;
    }

    virtual Gecode::ExecStatus commit(Gecode::Space& home,
                                      const Gecode::Choice& c,
                                      unsigned int a) {
      using namespace Gecode;
      const PosVal& pv = static_cast<const PosVal&>(c);
      int pos = pv.pos, val = pv.val;
      if (a == 0)
        return Gecode::me_failed(x[pos].lq(home,val)) ? ES_FAILED : ES_OK;
      else
        return me_failed(x[pos].gq(home,val+1)) ? ES_FAILED : ES_OK;
    }

  };

  class ValAny : public Gecode::ValSelBase<Rel::RelView,int> {
  public:
    ValAny(void) {}
    ValAny(Gecode::Space& home, const Gecode::ValBranchOptions& vbo)
      : Gecode::ValSelBase<Rel::RelView,int>(home,vbo) {}
    int val(Gecode::Space& home, View x) const {
      return x.min();
    }
    Gecode::ModEvent tell(Gecode::Space& home, unsigned int a, Rel::RelView x, int n) {
      return (a == 0) ? x.lq(home,n) : x.gq(home,n+1);
    }
  };

  void branch(Gecode::Home home, const RelVarArgs& x) {
    if (home.failed()) return;
    Gecode::ViewArray<Rel::RelView> y(home,x);
    Gecode::ViewSelNone<Rel::RelView> sn; ValAny vm;
    Gecode::ViewValBrancher<Gecode::ViewSelNone<Rel::RelView>,ValAny>::post(home,y,sn,vm);
  }

}
