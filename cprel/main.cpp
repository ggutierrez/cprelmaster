#include <cprel/cprel.hh>



using Gecode::Home;
using Gecode::Space;
using Gecode::Choice;
using Gecode::ExecStatus;
using Gecode::ES_FAILED;
using Gecode::ES_OK;

namespace MPG { namespace CPRel { namespace Branch {

/**
 * \brief Simple, tuple based brancher
 *
 * Simple brancher on a relation variable that selects a tuple and decides
 * to include and exclude it.
 */
class NoneMin : public Gecode::Brancher {
protected:
  /// Relation to branch on
  CPRelView x_;
  /// Simple, tuple-based relation choice
  class RelChoice : public Gecode::Choice {
  public:
    /// Tuple to branch on
    Tuple t_;
    /// Constructor
    RelChoice(const NoneMin& b, const Tuple& t)
      : Choice(b,2), t_(t) {}
    /// Returns the size of the object
    virtual size_t size(void) const {
      return sizeof(*this);
    }
  };
public:
  /// Constructor for a brancher on variable \a x
  NoneMin(Home home, CPRelView x)
    : Brancher(home), x_(x) {}
  /// Brancher posting
  static void post(Home home, CPRelView x) {
    (void) new (home) NoneMin(home,x);
  }
  /// Constructor for clonning
  NoneMin(Space& home, bool share, NoneMin& b)
    : Brancher(home,share,b) {
    x_.update(home,share,b.x_);
  }
  /// Brancher copying
  virtual Brancher* copy(Space& home, bool share) {
    return new (home) NoneMin(home,share,*this);
  }
  /// Brancher disposal
  virtual size_t dispose(Space& home) {
    (void) Brancher::dispose(home);
    return sizeof(*this);
  }
  /// Returns the status of the brancher
  virtual bool status(const Space&) const {
    if (!x_.assigned())
      return true;
    return false;
  }
  /// Creates a choice by selecting a tuple from the unknown of the variable
  virtual Choice* choice(Space&) {
    GRelationIter it(x_.unk());
    assert(it());
    return new RelChoice(*this,it.val());
  }
  /// Commit choice
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
