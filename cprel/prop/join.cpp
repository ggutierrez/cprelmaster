#include <cprel/prop/join.hh>
#include <cprel/cprel.hh>

// The reason for this undefne statement is because CUDD defines a
// macro with this name and this confusses gcc and clang with the fail
// method of the Space class that is called by the GECODE_ES_FAIL
// macro.
#undef fail 

namespace MPG {

using namespace CPRel;
using namespace CPRel::Prop;
void join(Gecode::Space& home, CPRelVar A, int j, CPRelVar B, CPRelVar C) {
  if (home.failed()) return;

  typedef boost::error_info<struct tag_invalid_join,std::string>
      invalid_join;

  if (j > A.arity() || j > B.arity() || C.arity() != (A.arity()+B.arity()-j))
    throw InvalidJoin()
      << errno_code(errno)
      << invalid_join("There are not enough columns for the join");


  CPRelView a(A);
  CPRelView b(B);
  CPRelView c(C);
  GECODE_ES_FAIL((Join<CPRelView,CPRelView,CPRelView>::post(home,a,j,b,c)));
}
}
