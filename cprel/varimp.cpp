#include <cprel/varimp.hh>

namespace MPG { namespace CPRel {

CPRelVarImp::CPRelVarImp(Space& home, const GRelation& l, const GRelation& u)
  : CPRelVarImpBase(home), glb_(l), lub_(u) {}

void CPRelVarImp::dispose(Space&) {
  //    std::cout << "Starting disposal" << std::endl;
  glb_.~GRelation();
  //    std::cerr << "--disposing--" << std::endl;
  lub_.~GRelation();
  //    std::cout << "Finishing disposal" << std::endl;
}

ModEvent CPRelVarImp::include(Space& home, const GRelation& r) {
  /*
    std::cout << "Inclusion. " << glb_ << "..." << lub_ << std::endl
    << "\tinclude " << r << std::endl;
  */
  if (r.subsetEq(glb_)) return ME_CPREL_NONE;
  if (!r.subsetEq(lub_)) return ME_CPREL_FAILED;
  // actual prunning
  glb_.unionAssign(r);
  assert(glb_.subsetEq(lub_));
  //std::cout << "\tResult: " << glb_ << "..." << lub_ << std::endl;
  CPRelDelta d(1,2);
  return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MIN, d);
}

ModEvent CPRelVarImp::exclude(Space& home, const GRelation& r) {
  /*
    std::cout << "Exclusion. " << glb_ << "..." << lub_ << std::endl
    << "\texclude " << r << std::endl;
  */
  if (!r.disjoint(glb_)) return ME_CPREL_FAILED;
  if (r.disjoint(lub_)) return ME_CPREL_NONE;
  lub_.differenceAssign(r);
  assert(glb_.subsetEq(lub_));
  //std::cout << "\tResult: " << glb_ << "..." << lub_ << std::endl;
  CPRelDelta d(1,2);
  return notify(home, assigned() ? ME_CPREL_VAL : ME_CPREL_MAX, d);
}


}}

// Register a disposer for variable implementations
Gecode::VarImpDisposer<MPG::CPRel::CPRelVarImp> d;
