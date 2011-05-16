#include <cprel/prop/and.hh>

namespace MPG {

using namespace CPRel;
using namespace CPRel::Prop;
void intersect(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C) {
  if (home.failed()) return;
  CPRelView a(A);
  CPRelView b(B);
  CPRelView c(C);
  GECODE_ES_FAIL(Intersect::post(home,a,b,c));
}
}
