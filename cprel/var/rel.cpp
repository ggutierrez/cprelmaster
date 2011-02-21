#include <cprel/rel.hh>

namespace MPG {

RelVar::RelVar(void) {}

RelVar::RelVar(const RelVar &y) : VarImpVar<Rel::RelVarImp>(y.varimp()) {}

RelVar::RelVar(Space &home, int min, int max)
  : VarImpVar<Rel::RelVarImp>(new (home) Rel::RelVarImp(home,min,max)) {
  std::cout << "Created variable " << min << " to " << max << std::endl;
  }
}

