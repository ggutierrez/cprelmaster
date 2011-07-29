#include <cptuple/varimp.hh>

namespace MPG { namespace CPTuple {

CPTupleVarImp::CPTupleVarImp(Space& home, const TupleSet& u)
  : CPTupleVarImpBase(home), dom_(u) {}

void CPTupleVarImp::dispose(Space&) {
  dom_.~GRelation();
}

ModEvent CPTupleVarImp::assign(Space& home, const Tuple& t) {
  TupleSet s(t.arity());
  s.add(t);
  return exclude(home,s.complement());
}

ModEvent CPTupleVarImp::exclude(Space& home, const Tuple& t) {
  TupleSet s(t.arity()); s.add(t);
  return exclude(home,s);
}

ModEvent CPTupleVarImp::exclude(Space& home, const TupleSet& s) {
  if (s.disjoint(dom_)) return ME_CPTUPLE_NONE;
  dom_.differenceAssign(s);
  if (dom_.empty()) return ME_CPTUPLE_FAILED;

  CPTupleDelta d(1,2);
  return notify(home, assigned() ? ME_CPTUPLE_VAL : ME_CPTUPLE_DOM, d);
}

}}

// Register a disposer for variable implementations
Gecode::VarImpDisposer<MPG::CPTuple::CPTupleVarImp> tuple_disposer;
