#include <vector>
#include <map>
#include <bdddomain/manager.hh>
#include <bdddomain/bdd.hh>

namespace MPG { namespace VarImpl {

    std::vector<int> domainIndices(int c) {
      std::vector<int> vars(Limits::bitsPerInteger,-1);

      for (int i = (1 << Limits::bbv); i--;) {
	vars[i] = (i << Limits::ba) + c;
      }
      return vars;
    }
  
  }
}
