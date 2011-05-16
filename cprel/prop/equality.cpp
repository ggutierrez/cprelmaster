#include <cprel/prop/equality.hh>

namespace MPG {

using namespace CPRel;
using namespace CPRel::Prop;
void equal(Gecode::Space& home, CPRelVar A, CPRelVar B) {
  if (home.failed()) return;

  CPRelView left(A);
  CPRelView right(B);
  GECODE_ES_FAIL(Equal::post(home,left,right));
}
}
