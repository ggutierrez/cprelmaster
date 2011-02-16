#include <gecode/kernel.hh>
#include <gecode/int.hh>

using Gecode::Space;
using Gecode::Home;
using Gecode::Brancher;
using Gecode::Choice;
using Gecode::ExecStatus;
using Gecode::PropCond;
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

  /// Variable implementation
  class RelVarImp : public RelVarImpBase {
  protected:
    int l, u;
  public:
    /// \name Constructors
    RelVarImp(Space& home, int min, int max)
      : RelVarImpBase(home), l(min), u(max) {}
    /// \name Copying
    RelVarImp(Space& home, bool share, RelVarImp& y)
      : RelVarImpBase(home, share, y), l(y.l), u(y.u) {}
    RelVarImp* copy(Space& home, bool share) {
      if (copied())
        return static_cast<RelVarImp*>(forward());
      else
        return new (home) RelVarImp(home,share,*this);
    }
    /// \name Disposal
    void dispose(Space&) {}

    /// \name Access operations
    int min(void) const { return l; }
    int max(void) const { return u; }
    /// Assignment test
    bool assigned(void) const { return l == u; }
    /// Modification operations
    Gecode::ModEvent lq(Space& home, int n) {
      if (n >= u) return ME_REL_NONE;
      if (n < l) return ME_REL_FAILED;
      RelDelta d;
      u = n;
      return notify(home, assigned() ? ME_REL_VAL : ME_REL_LUB, d);
    }
    Gecode::ModEvent gq(Space& home, int n) {
      if (n <= l) return ME_REL_NONE;
      if (n > u) return ME_REL_FAILED;
      RelDelta d; l = n;
      return notify(home, assigned() ? ME_REL_VAL : ME_REL_GLB, d);
    }
    /// Propagators events
    void subscribe(Space& home, Gecode::Propagator& p, Gecode::PropCond pc,
                   bool schedule=true) {
      RelVarImpBase::subscribe(home,p,pc,assigned(),schedule);
    }
    void cancel(Space& home, Gecode::Propagator& p, PropCond pc) {
      RelVarImpBase::cancel(home,p,pc,assigned());
    }
    /// Advaisor events
    void subscribe(Space& home, Advisor& a) {
      RelVarImpBase::subscribe(home,a,assigned());
    }
    void cancel(Space& home, Advisor& a) {
      RelVarImpBase::cancel(home,a,assigned());
    }
    /// Delta information
  };
}}

namespace MPG {
  class RelVar : public VarImpVar<Rel::RelVarImp> {
  protected:
    using VarImpVar<Rel::RelVarImp>::x;
  public:
    /// Constructors
    RelVar(void) {}
    /// Copy constructor
    RelVar(const RelVar& y)
    : VarImpVar<Rel::RelVarImp>(y.varimp()) {}
    /// Variable creation
    RelVar(Space& home, int min, int max)
      : VarImpVar<Rel::RelVarImp>(new (home) Rel::RelVarImp(home,min,max)) {
      //std::cout << "Created variable " << min << " to " << max << std::endl;
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

namespace MPG {
  class NoneAny : public Brancher {
  protected:
    ViewArray<Rel::RelView> x;
    class PosVal : public Choice {
    public:
      int pos; int val;
      PosVal(const NoneAny& b, int p, int v)
        : Choice(b,2), pos(p), val(v) {}
      virtual size_t size(void) const { return sizeof(*this); }
    };
  public:
    /// \name Brancher creation
    //@{
    /// Constructor
    NoneAny(Home home, ViewArray<Rel::RelView>& x0)
      : Brancher(home), x(x0) {}
    /// Creates a branch on the variables in x
    static void post(Home home, ViewArray<Rel::RelView>& x) {
      (void) new (home) NoneAny(home,x);
      }
    //@}
    /// \name Cloning
    //@{
    /// Copy constructor
    NoneAny(Space& home, bool share, NoneAny& b)
      : Brancher(home,share,b) {
      x.update(home,share,b.x);
    }
    /// Copy
    virtual Brancher* copy(Space& home, bool share) {
      return new (home) NoneAny(home,share,*this);
    }
    //@}
    /// \name Branching
    //@{
    /// Tests if there are more variables in \a x to branch on
    virtual bool status(const Space& home) const {
      for (int i=0; i<x.size();i++)
        if (!x[i].assigned())
          return true;
      return false;
    }
    /// Choice point creation
    virtual Choice* choice(Space& home) {
      for (int i=0; true; i++)
        if (!x[i].assigned())
          return new PosVal(*this,i,x[i].min());
      GECODE_NEVER;
      return NULL;
    }
    /// Choice application
    virtual ExecStatus commit(Space& home,
                                      const Choice& c,
                                      unsigned int a) {
      using namespace Gecode;
      const PosVal& pv = static_cast<const PosVal&>(c);
      int pos = pv.pos, val = pv.val;
      if (a == 0)
        return me_failed(x[pos].lq(home,val)) ? ES_FAILED : ES_OK;
      else
        return me_failed(x[pos].gq(home,val+1)) ? ES_FAILED : ES_OK;
    }
    //@}
  };

  void branch(Home home, const RelVarArgs& x) {
    if (home.failed()) return;
    ViewArray<Rel::RelView> y(home,x);
    NoneAny::post(home,y);
  }

}
