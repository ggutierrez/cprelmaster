#include <cprel/prop/follow.hh>
#include <cprel/cprel.hh>

namespace MPG {

using namespace CPRel;
using namespace CPRel::Prop;
void follow(Gecode::Space& home, CPRelVar A, int j, CPRelVar B, CPRelVar C) {
  if (home.failed()) return;

  /// \todo Handle cases of malformed relations with exceptions

  CPRelView a(A);
  CPRelView b(B);
  CPRelView c(C);
  GECODE_ES_FAIL((Follow<CPRelView,CPRelView,CPRelView>::post(home,a,j,b,c)));
}
}
