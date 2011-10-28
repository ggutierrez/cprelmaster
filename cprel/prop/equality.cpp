#include <cprel/prop/equality.hh>
#include <bdddomain/perm-descriptor.hh>

// The reason for this undefne statement is because CUDD defines a
// macro with this name and this confusses gcc and clang with the fail
// method of the Space class that is called by the GECODE_ES_FAIL
// macro.
#undef fail 

namespace MPG {

  using namespace CPRel;
  using namespace CPRel::Prop;
  void equal(Gecode::Space& home, CPRelVar A, CPRelVar B) {
    if (home.failed()) return;
    
    if (A.arity() != B.arity())
      throw ArityMissmatch("equality constraint");
    
    CPRelView left(A);
    CPRelView right(B);
    GECODE_ES_FAIL((Equal<CPRelView,CPRelView>::post(home,left,right)));
  }
  
  void complement(Gecode::Space& home, CPRelVar A, CPRelVar B) {
    if (home.failed()) return;

    if (A.arity() != B.arity())
      throw ArityMissmatch("complement constraint");

    CPRelView left(A);
    CPRelView vb(B);
    ComplementView<CPRelView> right(vb);
    GECODE_ES_FAIL((
		    Equal<CPRelView,ComplementView<CPRelView> >::post(home,left,right))
		   );
  }
}
