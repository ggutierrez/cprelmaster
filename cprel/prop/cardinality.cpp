#include <cprel/prop/cardinality.hh>

// The reason for this undefne statement is because CUDD defines a
// macro with this name and this confusses gcc and clang with the fail
// method of the Space class that is called by the GECODE_ES_FAIL
// macro.
#undef fail 

namespace MPG {

  using namespace CPRel;
  using namespace CPRel::Prop;
  using namespace Gecode;
  using namespace Gecode::Int;
  
  void cardinality(Gecode::Space& home, CPRelVar A, IntVar B) {
    if (home.failed()) return;
        
    CPRelView left(A);
    IntView right(B);
    GECODE_ES_FAIL((Cardinality<CPRelView,IntView>::post(home,left,right)));
  }
}
