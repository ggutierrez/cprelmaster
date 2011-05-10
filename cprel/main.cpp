#include <cprel/cprel.hh>



using Gecode::Home;
using Gecode::Space;
using Gecode::Choice;
using Gecode::ExecStatus;
using Gecode::ES_FAILED;
using Gecode::ES_OK;

namespace MPG { namespace CPRel { namespace Branch {
/*
 * Value selection classes
 *
 */


class NoneMin : public Gecode::Brancher {
protected:
  CPRelView x_;
  // choice definition
  class RelChoice : public Gecode::Choice {
  public:
    Tuple t_;
    RelChoice(const NoneMin& b, const Tuple& t)
      : Choice(b,2), t_(t) {}
    virtual size_t size(void) const {
      return sizeof(*this);
    }
  };
public:
  NoneMin(Home home, CPRelView x)
    : Brancher(home), x_(x) {}
  static void post(Home home, CPRelView x) {
    (void) new (home) NoneMin(home,x);
  }
  virtual size_t dispose(Space& home) {
    (void) Brancher::dispose(home);
    return sizeof(*this);
  }
  NoneMin(Space& home, bool share, NoneMin& b)
    : Brancher(home,share,b) {
    x_.update(home,share,b.x_);
  }
  virtual Brancher* copy(Space& home, bool share) {
    return new (home) NoneMin(home,share,*this);
  }
  // status
  virtual bool status(const Space& home) const {
    if (!x_.assigned())
      return true;
    return false;
  }
  // choice
  virtual Choice* choice(Space& home) {
    GRelationIter it(x_.lub().difference(x_.glb()));
    assert(it());
    return new RelChoice(*this,it.val());
  }
  // commit
  virtual ExecStatus commit(Space& home, const Choice& c, unsigned int a) {
    const RelChoice& ch = static_cast<const RelChoice&>(c);
    GRelation r(ch.t_.arity());
    r.add(ch.t_);
    if (a == 0)
      return Gecode::me_failed(x_.include(home,r)) ? ES_FAILED : ES_OK;
    else
      return Gecode::me_failed(x_.exclude(home,r)) ? ES_FAILED : ES_OK;
  }
};
}}

void branch(Home home, CPRelVar x) {
  using namespace CPRel::Branch;
  if (home.failed()) return;
  NoneMin::post(home,x);
}

}

// Register a disposer for variable implementations
Gecode::VarImpDisposer<MPG::CPRel::CPRelVarImp> d;
