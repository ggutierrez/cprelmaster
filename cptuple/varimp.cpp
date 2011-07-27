#include <cptuple/varimp.hh>

namespace MPG { namespace CPTuple {

CPTupleVarImp::CPTupleVarImp(Space& home, const TupleSet& u)
  : CPTupleVarImpBase(home), dom_(u) {}

void CPTupleVarImp::dispose(Space&) {
  dom_.~GRelation();
}

ModEvent CPTupleVarImp::assign(Space& home, const CPRel::Tuple& t) {
  TupleSet s(t.arity());
  s.add(t);

  if (s.subsetEq(dom_))
    dom_ = s;
  else
    return ME_CPTUPLE_FAILED;

  CPTupleDelta d(1,2);
  return notify(home, ME_CPTUPLE_VAL, d);
}

ModEvent CPTupleVarImp::exclude(Space& home, const CPRel::Tuple& t) {
  TupleSet s(t.arity());
  s.add(t);

  if (s.disjoint(dom_)) return ME_CPTUPLE_NONE;
  dom_.differenceAssign(s);
  if (dom_.empty()) return ME_CPTUPLE_FAILED;

  CPTupleDelta d(1,2);
  return notify(home, assigned() ? ME_CPTUPLE_VAL : ME_CPTUPLE_MAX, d);
}

}}

// Register a disposer for variable implementations
Gecode::VarImpDisposer<MPG::CPTuple::CPTupleVarImp> tuple_disposer;
