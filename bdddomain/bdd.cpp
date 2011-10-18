#include <map>
#include <bdddomain/manager.hh>
#include <bdddomain/bdd.hh>

namespace MPG { namespace VarImpl {

    /**
     * \brief Helper function to print a bdd.
     */
    int printdot_rec(const BDD& b, std::ostream& os, int current, 
		     std::vector<bool>& visited, std::map<DdNode*,int>& names) {
      
      int ri;
      auto node = names.find(b.getNode());
      if (node == names.end()) {
	// get a new identifier for the node
	ri = ++current;
	names[b.getNode()] = ri;
      } else {
	ri = node->second;
      }

      if (visited.at(ri))
	return current;
      // mark the node as visited
      visited[ri] = true;
      
      os << ri << " [label=\"" << var(b) << "\"];" << std::endl;

      BDD l = low(b); BDD h = high(b);
      auto nodeL = names.find(l.getNode());
      int li;
      if (nodeL == names.end()) {
	// get a new identifier for the node
	li = ++current;
	names[l.getNode()] = li;
      } else {
	li = nodeL->second;
      }

      auto nodeH = names.find(h.getNode());
      int hi;
      if (nodeH == names.end()) {
	// get a new identifier for the node
	hi = ++current;
	names[h.getNode()] = hi;
      } else {
	hi = nodeH->second;
      }

      os << ri << " -> " << li << " [style=dotted];" << std::endl;
      os << ri << " -> " << hi << " [style=dotted];" << std::endl;
      
      current = printdot_rec(l, os, current, visited, names);
      current = printdot_rec(h, os, current, visited, names);
      
      return current;
    } 

    void printDot(const BDD& b, std::ostream& os) {
      os << "digraph G {" << std::endl;
      os << "0 [shape=box, label=\"0\", style=filled, shape=box, height=0.3, width=0.3];" << std::endl;
      os << "1 [shape=box, label=\"1\", style=filled, shape=box, height=0.3, width=0.3];" << std::endl;
      
      int variables = 1 << (Limits::bbv + Limits::ba);
      // make a vector big enough to store all the variables in the
      // manager and constants true and false
      std::vector<bool> visited(variables + 2);
      visited[0] = true; visited[1] = true;
      
      // a map to store pointers to names (integer ids)
      std::map<DdNode*,int> names;
      names[factory().bddZero().getNode()] = 0;
      names[factory().bddOne().getNode()] = 1;

      printdot_rec(b, os, 1, visited, names);
      /*
	for (auto e : names) {
	std::cout << "Element " << e.first << " --- " << e.second << std::endl;
      }
      */
      
    }

  }
}
