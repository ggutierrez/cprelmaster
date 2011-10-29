#include <bdddomain/encoding.hh>
#include <bdddomain/manager.hh>
#include <bdddomain/bdd.hh>
#include <set>

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

    std::vector<int> bddIndices(int c) {
      std::vector<int> vars;
      vars.reserve(Limits::bitsPerInteger);
      
      for (int i = (1 << Limits::bbv); i--;) {
	vars.push_back((i << Limits::ba) + c);
      }
      return vars;
    }

    BDD makeCube(const std::vector<int>& c) {
      assert(c.size() > 0 && "Trying to make a cube out of an empty column vector");
      assert(c.size() < static_cast<unsigned int>(Limits::arity) &&
	     "trying to make a cube with more columns than in the manager");
    
      const unsigned int size = c.size() * Limits::bitsPerInteger;
      std::vector<int> indices;
      indices.reserve(size);
      std::for_each(begin(c), end(c),
		    [&indices](int e) {
		      const auto& i = bddIndices(e);
		      std::copy(begin(i), end(i), std::back_inserter(indices));
		    });
      assert(size == indices.size());
      return factory().IndicesToCube(&indices[0],indices.size());
    }

    BDD makeCube(int c) {
      assert(c >= 0 && c < Limits::arity &&
	     "Trying to make a cube out of an invalid index empty");
      std::vector<int> columns(1,c);
      return makeCube(columns);
    }

    BDD oneTuple(int c, BDD r) {
     
      BDDvector vars(c * Limits::bitsPerInteger);
      int i = 0;
      for (int j = 0; j < c; j++) {
	for (auto v :  bddIndices(j)) {
	  vars[i] = factory().bddVar(v);
	  i++;
	}
      }
      return r.PickOneMinterm(vars);
    }

    BDD shiftLeft(BDD r, int arity, const int n) {
      const int numberOfVars = Limits::bitsPerInteger * Limits::arity;
      assert( (n + arity) <= numberOfVars && "The shift cannot be carried out");

      // This array is used to store the definition of the permutation
      int perm[numberOfVars];

      // Move the variables of the relation itself
      for (int i = 0; i < Limits::arity; i++) {
	for (int j = i; j < numberOfVars;) {
	  if (i < arity) {
	    // The current j is part of the representation of column i in the relation
	    perm[j] = j + n;
	  } else if (i >= arity && i < arity+n) {
	    perm[j] = j - n;
	  } else {
	    perm[j] = j;
	  }
	  j += Limits::arity;
	}
      }

      // perform the permutation
      return r.Permute(perm);
      /// \todo Do I have to existentially quantify on the first n columns of p?
    }

    BDD shiftRight(BDD r, int arity, const int n) {
      const int numberOfVars = Limits::bitsPerInteger * Limits::arity;
      assert( arity >= n && "The shift cannot be carried out");

      // This array is used to store the definition of the permutation
      int perm[numberOfVars];
      std::vector<int> quantIndices;
      quantIndices.reserve(n*Limits::bitsPerInteger);

      // Move the variables of the relation itself
      for (int i = 0; i < Limits::arity; i++) {
	//std::cout << "Column " << i << ": ";
	for (int j = i; j < numberOfVars;) {
	  //std::cout << " " << j;
	  if (i < n) {
	    perm[j] = j + arity;
	    quantIndices.push_back(j + arity);
	    //std::cout << "->*" << perm[j] << " ";
	  } else if (i < arity) {
	    // The current j is part of the representation of column i in the relation
	    perm[j] = j - n;
	    //std::cout << "->" << perm[j] << " ";
	  } else {
	    perm[j] = j;
	    //std::cout << "->" << perm[j] << " ";
	  }
	  j += Limits::arity;
	}
	//std::cout << std::endl;
      }

      // perform the permutation
      BDD p = r.Permute(perm);
    
      // quantify on the variables that were "removed" from the relation
      BDD cube = factory().IndicesToCube(&quantIndices[0],quantIndices.size());
      
      BDD q = p.ExistAbstract(cube);
      return q;
    }
  }
}


