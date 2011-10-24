#include <bdddomain/manager.hh>

namespace MPG { namespace VarImpl {

    std::vector<int> domainIndices(int c) {
      assert(c < Limits::arity && c >= 0 && "Column out of range.");
      std::vector<int> vars(Limits::bitsPerInteger,-1);

      for (int i = (1 << Limits::bbv); i--;) {
	vars[i] = (i << Limits::ba) + c;
      }
      std::reverse(begin(vars), end(vars));
      return vars;
    }
  
  }
}
