#include <cprel/prop/tuple.hh>

namespace MPG {

using namespace CPRel;
using namespace CPTuple;
using namespace CPRel::Prop;
void dom(Gecode::Space& home, CPTupleVar t, CPRelVar R) {
  if (home.failed()) return;

  if (t.arity() != R.arity())
    throw ArityMissmatch("domain constraint");

  CPTupleView tuple(t);
  CPRelView rel(R);
  GECODE_ES_FAIL((Dom<CPTupleView,CPRelView>::post(home,tuple,rel)));
}

}
