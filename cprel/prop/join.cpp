#include <cprel/prop/join.hh>
#include <cprel/cprel.hh>

namespace MPG {

using namespace CPRel;
using namespace CPRel::Prop;
void join(Gecode::Space& home, CPRelVar A, int j, CPRelVar B, CPRelVar C) {
  if (home.failed()) return;

  CPRelView a(A);
  CPRelView b(B);
  CPRelView c(C);
  GECODE_ES_FAIL((Join<CPRelView,CPRelView,CPRelView>::post(home,a,j,b,c)));
}
}
