#include <cprel/prop/projection.hh>
#include <bdddomain/proj-descriptor.hh>

namespace MPG {
using namespace CPRel;
using namespace CPRel::Prop;
void projection(Gecode::Space& home, const ProjDescriptor& p, CPRelVar A, CPRelVar B) {
  if (home.failed()) return;

  // p has to be a valid projection description for A
  typedef boost::error_info<struct tag_proj_descriptor,std::string>
      proj_descriptor;
  if (!p.valid(A.arity()))
    throw InvalidProjDescriptor()
      << errno_code(errno)
      << proj_descriptor("Invalid projection description when posting projection constraint: p has to be valid for A");

  CPRelView left(A);
  CPRelView right(B);
  GECODE_ES_FAIL((Project<CPRelView,CPRelView>::post(home,p,left,right)));
}
}
