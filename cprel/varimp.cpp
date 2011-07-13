#include <cprel/varimp.hh>

namespace MPG { namespace CPRel {

CPRelVarImp::CPRelVarImp(Space& home, const GRelation& l, const GRelation& u)
  : CPRelVarImpBase(home), glb_(l), lub_(u) {}

void CPRelVarImp::dispose(Space&) {
  glb_.~GRelation();
  lub_.~GRelation();
}

ModEvent CPRelVarImp::include(Space& home, const GRelation& r) {
  if (r.subsetEq(glb_)) return ME_CPREL_NONE;
  if (!r.subsetEq(lub_)) return ME_CPREL_FAILED;
  glb_.unionAssign(r);
  assert(glb_.subsetEq(lub_));
  CPRelDelta d(1,2);
  return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MIN, d);
}

ModEvent CPRelVarImp::exclude(Space& home, const GRelation& r) {
  if (!r.disjoint(glb_)) return ME_CPREL_FAILED;
  if (r.disjoint(lub_)) return ME_CPREL_NONE;
  lub_.differenceAssign(r);
  assert(glb_.subsetEq(lub_));
  CPRelDelta d(1,2);
  return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MAX, d);
}


}}

// Register a disposer for variable implementations
Gecode::VarImpDisposer<MPG::CPRel::CPRelVarImp> d;
