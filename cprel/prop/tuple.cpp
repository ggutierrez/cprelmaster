#include <cprel/prop/tuple.hh>

namespace MPG {

using namespace CPRel;
using namespace CPTuple;
using namespace CPRel::Prop;
void included(Gecode::Space& home, CPTupleVar t, CPRelVar R) {
  if (home.failed()) return;

  if (t.arity() != R.arity())
    throw ArityMissmatch("domain constraint");

  CPTupleView tuple(t);
  CPRelView rel(R);
  GECODE_ES_FAIL((Dom<CPTupleView,CPRelView>::post(home,tuple,rel)));
}

void excluded(Gecode::Space& home, CPTupleVar t, CPRelVar R) {
  if (home.failed()) return;

  if (t.arity() != R.arity())
    throw ArityMissmatch("domain constraint");

  CPTupleView tuple(t);
  CPRelView rel(R);
  typedef ComplementView<CPRelView> CView;
  CView complementR(rel);
  GECODE_ES_FAIL((Dom<CPTupleView,CView>::post(home,tuple,complementR)));
}
}
