#include <cprel/prop/functional.hh>
#include <cprel/cprel.hh>

// The reason for this undefne statement is because CUDD defines a
// macro with this name and this confusses gcc and clang with the fail
// method of the Space class that is called by the GECODE_ES_FAIL
// macro.
#undef fail 

namespace MPG {
  using namespace CPRel;
  using namespace CPRel::Prop;
  void function(Gecode::Space& home, CPRelVar A) {
    if (home.failed()) return;
    
    if (A.arity() != 2)
      assert(false);
    
    CPRelView a(A);
    GECODE_ES_FAIL((Function<CPRelView>::post(home,a)));
  }
}
