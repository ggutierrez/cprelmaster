#include <cprel/cprel.hh>

namespace MPG {
  using namespace CPRel;
  
  void include(Gecode::Space& home, CPRelVar A, GRelation r) {
    if (home.failed()) return;
    if (r.arity() != A.arity())
      throw ArityMissmatch("Include constraint");
    CPRelView a(A);
    auto me = a.include(home,r);
    if (me == Gecode::ES_FAILED) {
      home.fail();
    }
  }

  void exclude(Gecode::Space& home, CPRelVar A, GRelation r) {
    if (home.failed()) return;
    if (r.arity() != A.arity())
      throw ArityMissmatch("Exclude constraint");
    CPRelView a(A);
    auto me = a.exclude(home,r);
    if (me == Gecode::ES_FAILED) {
      home.fail();
    }
  }
  
}
