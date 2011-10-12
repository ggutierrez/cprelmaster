#include <bdddomain/bdd.hh>
#include <algorithm>

namespace MPG { namespace VarImpl {
    
    Bdd::Bdd(BDD bdd) : bdd_(bdd) {};

    Bdd::Bdd(void) {}

    Bdd::Bdd(const Bdd& other) : bdd_(other.bdd_) {}

    Bdd Bdd::operator ! (void) const {
      return !bdd_;
    }

    unsigned int Bdd::var(void) const {
      return bdd_.NodeReadIndex();
    }
    
    Bdd Bdd::high(void) const {
      auto node = bdd_.getNode();
      auto manager = bdd_.manager();
      auto res = Cudd_T(node);
      res = Cudd_NotCond(res, Cudd_IsComplement(node));
      
      return BDD(manager,res);
    }
    
    Bdd Bdd::low(void) const {
      auto node = bdd_.getNode();
      auto manager = bdd_.manager();
      auto res = Cudd_E(node);
      res = Cudd_NotCond(res, Cudd_IsComplement(node));
      
      return BDD(manager,res);
    }

    bool Bdd::isOne(void) const {
      auto manager = bdd_.manager();
      return manager->bddOne() == bdd_;

    }

    bool Bdd::isZero(void) const {
      auto manager = bdd_.manager();
      return manager->bddZero() == bdd_;
    }

    int Bdd::nodeCount(void) const {
      return Cudd_DagSize(bdd_.getNode())-1;
    }
     
    Bdd Bdd::id(void) const {
      return Bdd(bdd_);
    }
    
    Bdd Bdd::ite(const Bdd& bddThen, const Bdd& bddElse) const {
      return bdd_.Ite(bddThen.bdd_,bddElse.bdd_);
    }
    
    void Bdd::andWith(const Bdd& other) {
      bdd_ = bdd_ & other.bdd_;
    }
    
    void Bdd::orWith(const Bdd& other) {
      bdd_ = bdd_ | other.bdd_;
    }

    Bdd Bdd::forAll(const Bdd& cube) {
      return bdd_.UnivAbstract(cube.bdd_);
    }

    Bdd Bdd::exists(const Bdd& cube) {
      return bdd_.ExistAbstract(cube.bdd_);
    }

    Bdd Bdd::permute(const Bdd& s0, const Bdd& s1) {
      std::vector<DdNode*> varsS0, varsS1;
      Bdd x(s0), y(s1);
      // assumes s0 and s1 are cubes of the same number of variables
      while (!x.isOne()) {
	varsS0.push_back(x.bdd_.getNode());
	x = x.high();
	varsS1.push_back(y.bdd_.getNode());
	y = y.high();
      }
      // ensure both cubes have the same number of variables.
      assert(y.isOne());

      // transform both sets into BDDvectors
      BDDvector orig(varsS0.size(), x.bdd_.manager(),&varsS0[0]);
      BDDvector dest(varsS1.size(), x.bdd_.manager(),&varsS1[0]);
      // return the permutation
      return bdd_.SwapVariables(orig,dest);
    }

    double Bdd::countMinterm(int vars) const {
      return bdd_.CountMinterm(vars);
    }

    bool Bdd::isEqual(const Bdd& other) const {
      return bdd_ == other.bdd_;
    }
    /*    
    Bdd Bdd::permute(std::vector<int>& perm) const {
      return bdd_.Permute(&perm[0]);
    }
    */
    int Bdd::printdot_rec(std::ostream& os, int current, std::vector<bool>& visited,
			  std::vector<int>& names) const {

      // the identifier of the current node
      int nodeId = var() + 2;
      //std::cout << "Current Node " << nodeId << std::endl;
      if (names.at(nodeId) == -1) {
	// the node has not a name in dot yet
	names[nodeId] = ++current;
      } 
      //std::cout << "Current name " << names.at(nodeId) << std::endl;
      if (visited.at(nodeId)) {
	return current;
      }
      // declare the node as visited
      visited[nodeId] = true;
      
      auto nodeName = names.at(nodeId);
      os << nodeName << " [label=\"" << var() << "\"]" << std::endl;

      // printing the low part of the bdd
      Bdd l = low();
      if (l.isZero() || l.isOne()) {
	os << nodeName << " -> " << (l.isZero() ? 0 : 1)  << " [style=dotted];" << std::endl;
      } else {
	int leftId = l.var() + 2;
	if (names.at(leftId) == -1) {
	  names[leftId] = ++current;
	}
	os << nodeId << " -> " << names.at(leftId) << " [style=dotted];" << std::endl;
	current = l.printdot_rec(os, current, visited, names);
      }

      // printing the right part of the bdd
      Bdd h = high();
      if (h.isZero() || h.isOne()) {
	os << nodeName << " -> " << (h.isZero() ? 0 : 1) << " [style=filled];" << std::endl;
      } else {
	int rightId = h.var() + 2;
	std::cout << "Here??? " << rightId << " " << names.size() << std::endl;
	if (names.at(rightId) == -1) {
	  names[rightId] = ++current;
	}
	std::cout << "Here??? " << std::endl;
	os << nodeId << " -> " << names.at(rightId) << " [style=filled];" << std::endl;
	current = h.printdot_rec(os, current, visited, names);
      }
 
      return current;
    }
    void Bdd::printDot(std::ostream& os) const {
      os << "digraph G {" << std::endl;
      if (isZero()) {
	os << "0 [shape=box, label=\"F\", style=filled, shape=box, height=0.3, width=0.3];" << std::endl;
	os << "}" << std::endl;
      } else if (isOne()) {
	os << "1 [shape=box, label=\"T\", style=filled, shape=box, height=0.3, width=0.3];" << std::endl;
	os << "}" << std::endl;
      } else {
	os << "0 [shape=box, label=\"F\", style=filled, shape=box, height=0.3, width=0.3];" << std::endl;
	os << "1 [shape=box, label=\"T\", style=filled, shape=box, height=0.3, width=0.3];" << std::endl;
	std::vector<bool> visited(nodeCount()+2,false);
	visited[0] = true; visited[1] = true;
	std::vector<int> names(nodeCount()+2,-1);
	names[0] = 0;
	names[1] = 1;
	printdot_rec(os, 1, visited, names);
	
	os << "}" << std::endl;
      }
    }

    bool operator == (const Bdd& f, const Bdd& g) {
      return f.isEqual(g);
    }

  }}
