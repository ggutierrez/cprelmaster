#include <cptuple/varimp.hh>

namespace MPG { namespace CPTuple {

CPTupleVarImp::CPTupleVarImp(Space& home, const TupleSet& u)
  : CPTupleVarImpBase(home), dom_(u) {}

void CPTupleVarImp::dispose(Space&) {
  dom_.~GRelation();
}

ModEvent CPTupleVarImp::assign(Space& home, const CPRel::Tuple& t) {
//  if (r.subsetEq(glb_)) return ME_CPREL_NONE;
//  if (!r.subsetEq(lub_)) return ME_CPREL_FAILED;
//  glb_.unionAssign(r);
//  assert(glb_.subsetEq(lub_));
  CPTupleDelta d(1,2);
  return notify(home, assigned() ? ME_CPTUPLE_VAL : ME_CPTUPLE_MIN, d);
}

ModEvent CPTupleVarImp::exclude(Space& home, const CPRel::Tuple& t) {
//  if (!r.disjoint(glb_)) return ME_CPREL_FAILED;
//  if (r.disjoint(lub_)) return ME_CPREL_NONE;
//  lub_.differenceAssign(r);
//  assert(glb_.subsetEq(lub_));
  CPTupleDelta d(1,2);
  return notify(home, assigned() ? ME_CPTUPLE_VAL : ME_CPTUPLE_MAX, d);
}

}}

// Register a disposer for variable implementations
Gecode::VarImpDisposer<MPG::CPTuple::CPTupleVarImp> d;
