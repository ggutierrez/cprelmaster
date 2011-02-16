#include <cprel/rel.hh>

using namespace MPG::Rel;

RelVarImp::RelVarImp(Space& home, int min, int max)
  : RelVarImpBase(home), l(min), u(max) {}

RelVarImp::RelVarImp(Space& home, bool share, RelVarImp& y)
    : RelVarImpBase(home, share, y), l(y.l), u(y.u) {}

//Gecode::VarImpDisposer<MPG::Rel::RelVarImp> disp;
