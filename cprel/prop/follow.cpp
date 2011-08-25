#include <cprel/prop/follow.hh>
#include <cprel/cprel.hh>

namespace MPG {

using namespace CPRel;
using namespace CPRel::Prop;
void follow(Gecode::Space& home, CPRelVar A, int f, CPRelVar B, CPRelVar C) {
  if (home.failed()) return;

  typedef boost::error_info<struct tag_invalid_follow,std::string>
      invalid_follow;

  if (f > A.arity() ||
      f > B.arity() ||
      C.arity() != (A.arity()+B.arity()-(2*f)))
    throw InvalidFollow()
      << errno_code(errno)
      << invalid_follow("Invalid arity of the variables.");


  CPRelView a(A);
  CPRelView b(B);
  CPRelView c(C);
  GECODE_ES_FAIL((Follow<CPRelView,CPRelView,CPRelView>::post(home,a,f,b,c)));
}

void divide(Gecode::Space& home, CPRelVar A, int d, CPRelVar B, CPRelVar C) {
  if (home.failed()) return;

  /// \todo Change the type of the exception to match the constraint being posted.
  typedef boost::error_info<struct tag_invalid_follow,std::string>
      invalid_follow;

  if (d > A.arity() ||
      d > B.arity() ||
      C.arity() != (A.arity()+B.arity()-(2*d)))
    throw InvalidFollow()
      << errno_code(errno)
      << invalid_follow("Invalid arity of the variables.");


  CPRelView a(A);
  CPRelView b(B);
  CPRelView c(C);

  typedef ComplementView<CPRelView> CView;
  CView cb(b);
  CView cc(c);
  GECODE_ES_FAIL((Follow<CPRelView,CView,CView>::post(home,a,d,cb,cc)));
}


}
