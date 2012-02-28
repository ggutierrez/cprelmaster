#include <cprel/prop/projection.hh>

// The reason for this undefne statement is because CUDD defines a
// macro with this name and this confusses gcc and clang with the fail
// method of the Space class that is called by the GECODE_ES_FAIL
// macro.
#undef fail 

namespace MPG {
  using namespace CPRel;
  using namespace CPRel::Prop;
  void projection(Gecode::Space& home, int p, CPRelVar A, CPRelVar B) {
    if (home.failed()) return;

    if (p > A.arity())
      throw InvalidProjection("Invalid projection description when posting projection constraint: p has to be valid for A");
    
    CPRelView left(A);
    CPRelView right(B);
    GECODE_ES_FAIL((Project<CPRelView,CPRelView>::post(home,p,left,right)));
  }
}
