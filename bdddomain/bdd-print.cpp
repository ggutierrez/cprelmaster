#include <vector>
#include <map>
#include <bdddomain/manager.hh>
#include <bdddomain/bdd.hh>

namespace MPG { 
  namespace VarImpl {
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

     /**
     * \brief Function to traverse every cube in BDD \a r and to apply \a fc.
     *
     * \a fc must be a functor that takes as argument a
     * std::vector<std::vector<int>>.  every possition in the vector
     * represents a column and inside it is the vector with the
     * possible values for that column on a particular branch.
     */
    template <typename Functor>
    void printSet_rec(Cudd& factory, const BDD& r, Functor& fc, std::vector<int>& set) {
      // the number of domains is the number of columns
      int fdvarnum = Limits::arity;
      
      if (r == factory.bddZero()) 
	return;
      else if (r == factory.bddOne()) {
	// we got a branch that goes to terminal 1
	std::vector<std::vector<int>> decoded(fdvarnum);
	for (int n = fdvarnum; n--;) {
	  const std::vector<int>& domain_n_ivar = domainIndices(n);
	  int domain_n_varnum = domain_n_ivar.size();
	  
	  bool used = false;
	  // detect if the branch represented by \a set uses a
	  // variable of the current column
	  for (int m = 0; m < domain_n_varnum; m++) {
	    if (set.at(domain_n_ivar.at(m)) != 0) {
	      used = true;
	    }
	  }

	  if (used) {
	    std::vector<int> var(domain_n_ivar);
	    int maxSkip = -1;
	    bool hasDontCare = false;
	    
	    // detect if there are dont cares in \a set for the
	    // current column. A dont care is represented by a value
	    // of zero in set. Not that at this point we are sure that
	    // the column is being used.
	    for (int i = 0; i < domain_n_varnum; i++) {
	      int val = set.at(var.at(i));
	      if (val == 0) {
		hasDontCare = true;
		if (maxSkip == i-1)
		  maxSkip = i;
	      }
	    }

	    // decode the number represented by this column
	    int pos = 0;
	    for (int i = 0; i < domain_n_varnum; i++) {
	      pos <<= 1;
	      int val = set.at(var.at(i));
	      if (val == 2)
		pos |= 1;
	    }
	    
	    // print the number
	    if (!hasDontCare) {
	      decoded[n].push_back(pos); 
	    } else {
	      decoded[n].push_back(-1);
	    }

	  }
	  else {
	    decoded[n].push_back(-2);
	  }
	}

	// call the functor
	fc(decoded);
      } else {
	set[var(r)] = 1;
	BDD lo = low(r);
	printSet_rec(factory, lo, fc, set);
	
	set[var(r)] = 2;
	BDD hi = high(r);
	printSet_rec(factory, hi, fc, set);
	
	set[var(r)] = 0;
      }
    }

    void printSet(const BDD& b, std::ostream& os) {
      Cudd& f = factory();
      if (b == f.bddZero()) 
	os << "F";
      else if (b == f.bddOne())
	os << "T";
      else {
	int variables = 1 << (Limits::bbv + Limits::ba);
	std::vector<int> set(variables,0);
	
	// The header of the relation
	for (auto i = Limits::arity; i--;) {
	  os << "| Col_{" << i << "}";
	}
	os << "|" << std::endl;
	auto functor = [&](std::vector<std::vector<int>>& r) {
	  for (auto i = r.size(); i--;) {
	    os << "| " << r.at(i).at(0) ;
	  }
	  os << "|" << std::endl;
	};

	printSet_rec(f, b, functor, set);

      }
    }

  }
}
