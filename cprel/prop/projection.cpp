#include <cprel/prop/projection.hh>

namespace MPG {
using namespace CPRel;
using namespace CPRel::Prop;
void projection(Gecode::Space& home, int p, CPRelVar A, CPRelVar B) {
  if (home.failed()) return;

  typedef boost::error_info<struct tag_proj_descriptor,std::string>
      proj_descriptor;
  if (p > A.arity())
    throw InvalidProjection()
      << errno_code(errno)
      << proj_descriptor("Invalid projection description when posting projection constraint: p has to be valid for A");

  CPRelView left(A);
  CPRelView right(B);
  GECODE_ES_FAIL((Project<CPRelView,CPRelView>::post(home,p,left,right)));
}
}
