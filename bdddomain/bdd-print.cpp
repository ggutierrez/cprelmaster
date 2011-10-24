#include <vector>
#include <map>
#include <string>
#include <boost/lexical_cast.hpp>
#include <bdddomain/manager.hh>
#include <bdddomain/bdd.hh>

namespace MPG { 
  namespace VarImpl {
    /**
     * \brief Helper function to traverse a bdd.
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
      os << ri << " -> " << hi << " [style=solid];" << std::endl;
      
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
      std::vector<bool> visited(variables + 2, false);
      visited[0] = true; visited[1] = true;
      
      // a map to store pointers to names (integer ids)
      std::map<DdNode*,int> names;
      names[factory().bddZero().getNode()] = 0;
      names[factory().bddOne().getNode()] = 1;

      printdot_rec(b, os, 1, visited, names);

      os << "}";
    }

    void printSet(const BDD& b, int columns,  std::ostream& os) {
      Cudd& f = factory();
      if (b == f.bddZero()) 
	os << "F";
      else if (b == f.bddOne())
	os << "T";
      else {
	// The header of the relation
        os << std::endl;
        
        os << setiosflags(std::ios::left);
	for (auto i = Limits::arity; i--;)
	  os << "C-" << i << std::setw(7) << " " ;
        os << std::endl;
	// define the functor that will print the content of b this
	// functor is called wih a vector of vectors of integers.
	auto functor = [&](std::vector<std::vector<int>>& r) {
	  for (auto i = r.size(); i--;) {
	    // every element in \a i can contain one or more
	    // integers. When there are several integers is because a
	    // range of values is represented in the same branch in
	    // the bdd and therefore we can offer a compressed way to
	    // output them.
            std::string text;
	    auto& range = r.at(i);
	    bool single = range.size() == 1;
	    if (!single)
	      text = "{";
	    for (auto e = begin(range); e != end(range);) { 
	      int element = *e;
	      if (element != -2) {
		text += boost::lexical_cast<std::string>(element);
	      } else {
		text += "-";
	      }
	      ++e;
	      if (e != end(range)) {
		text +=  ",";
	      } else if (!single)
		text += "}";
	    }
            os << std::setw(10) << text;
          }
	  os << std::endl;
	};

	traverseSet(f, b, functor);
      }
    }

  }
}
