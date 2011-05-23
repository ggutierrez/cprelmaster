#include <cprel/prop/and.hh>
#include <cprel/cprel.hh>

namespace MPG {

using namespace CPRel;
using namespace CPRel::Prop;
void intersect(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C) {
  if (home.failed()) return;
  if (A.arity() != B.arity())
    throw CPRel::ArityMismatch("intersect constraint");
  if (A.arity() != C.arity())
    throw CPRel::ArityMismatch("intersect constraint");
  if (C.arity() != B.arity())
    throw CPRel::ArityMismatch("intersect constraint");

  CPRelView a(A);
  CPRelView b(B);
  CPRelView c(C);
  GECODE_ES_FAIL((Intersect<CPRelView,CPRelView,CPRelView>::post(home,a,b,c)));
}

void Union(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C) {
  if (home.failed()) return;
  if (A.arity() != B.arity())
    throw ArityMismatch("intersect constraint");
  if (A.arity() != C.arity())
    throw ArityMismatch("intersect constraint");
  if (C.arity() != B.arity())
    throw ArityMismatch("intersect constraint");

  CPRelView a(A), b(B), c(C);

  typedef ComplementView<CPRelView> CView;
  CView complementA(a), complementB(b), complementC(c);
  GECODE_ES_FAIL((Intersect<CView,CView,CView>::
                  post(home,complementA,complementB,complementC)));
}

void subset(Gecode::Space& home, CPRelVar A, CPRelVar B) {
  if (home.failed()) return;
  if (A.arity() != B.arity())
    throw ArityMismatch("intersect constraint");

  CPRelView a(A);
  CPRelView b(B);

  typedef ComplementView<CPRelView> CView;
  CView complementB(b);

  GRelation empty(A.arity());
  ConstCPRelView emptyView(empty);

  GECODE_ES_FAIL((Intersect<CPRelView,CView,ConstCPRelView>::
                  post(home,a,complementB,emptyView)));
}

void disjoint(Gecode::Space& home, CPRelVar A, CPRelVar B) {
  if (home.failed()) return;
  if (A.arity() != B.arity())
    throw ArityMismatch("intersect constraint");

  CPRelView a(A);
  CPRelView b(B);

  GRelation empty(A.arity());
  ConstCPRelView emptyView(empty);

  GECODE_ES_FAIL((Intersect<CPRelView,CPRelView,ConstCPRelView>::
                  post(home,a,b,emptyView)));
}

void implies(Gecode::Space& home, CPRelVar A, CPRelVar B, CPRelVar C) {
  if (home.failed()) return;
  if (A.arity() != B.arity())
    throw ArityMismatch("intersect constraint");
  if (A.arity() != C.arity())
    throw ArityMismatch("intersect constraint");
  if (C.arity() != B.arity())
    throw ArityMismatch("intersect constraint");

  CPRelView a(A), b(B), c(C);
  typedef ComplementView<CPRelView> CView;
  CView complementB(b), complementC(c);

  GECODE_ES_FAIL((Intersect<CPRelView,CView,CView>::
                  post(home,a,complementB,complementC)));
}
}
