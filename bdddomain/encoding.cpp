#include <bdddomain/encoding.hh>
#include <bdddomain/manager.hh>
#include <bdddomain/bdd.hh>
#include <set>

namespace MPG { namespace VarImpl {

    using std::pair;
    using std::vector;

    vector<int> bddIndices(int c) {
      vector<int> vars;
      vars.reserve(Limits::bitsPerInteger);

      for (int i = (1 << Limits::bbv); i--;) {
	vars.push_back((i << Limits::ba) + c);
      }
      return vars;
    }

    BDD makeCube(const std::vector<int>& c) {
      std::cout << "Called container cube " << std::endl;
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
      std::cout << "Called constant cube " <<  std::endl;
      std::vector<int> columns(1,c);
      return makeCube(columns);
    }

    BDD replace(const std::vector<std::pair<int,int>>& pairing, BDD r) {
      //std::vector<int>
      return r;
    }

    vector<DdNode*> bddVars(int c) {
      vector<DdNode*> vars;
      vars.reserve(Limits::bitsPerInteger);

      for (int i = Limits::bitsPerInteger; i--;) {
	int var = (i << Limits::ba)+c;
	vars.push_back(Cudd_bddIthVar(dd(),var));
      }
      return vars;
    }

    std::vector<int> decodeCube(int* cube, int arity) {
      const int cubeSize = 1 << (Limits::bbv + Limits::ba);
      // The maximum arity of a tuple with the current setup is maxTupleSize
      const int maxTupleSize = 1 << Limits::ba;

      // The working vector:
      // Reserve space in the vector to store a complete tuple stored in the bdd.
      // at the end we will not use all the information but we will perform some
      // operations on it.
      std::vector<int> ret(maxTupleSize,0);

      for (int i = cubeSize - 1; i >= 0; i--) {
	// In every iteration of this loop we are concerned only with tuples of arity
	// _arity. To iterate only in the elements in the cube that contain useful
	// information for this arity:
	//
	// *) (maxTupleSize -1) in binary is equivalent to ones at every place that
	//    has to be considered for the maximum arity.
	// *) Anding that number with the current bit of the cube  and comparing it
	//    less than the actual tuple arity will allow us to filter the bits in
	//    the cube that are meaningful for this tuple.
	if ((i & (maxTupleSize - 1)) < arity) {
	  // compute the column that is being affected by the current element in the
	  // cube.
	  int column = maxTupleSize - 1 - (i & (maxTupleSize - 1));
	  //int column = (i & (maxTupleSize - 1));

	  // compute the possition in the column for the current element of the cube
	  int bitInColumn = (Limits::bitsPerInteger-1-(i>>Limits::ba));

	  ret[column] &= ~(1 << bitInColumn);
	  ret[column] |= cube[i] << bitInColumn;
	}
      }
      return std::vector<int>(ret.begin()+(maxTupleSize-arity), ret.end());
    }

    BDD swap_columns(BDD r, const PermDescriptor& swapDesc) {
      const int numberOfVars = Limits::bitsPerInteger * Limits::arity;
      std::set<int> permuted;
      // This array is used to store the definition of the permutation
      int perm[numberOfVars];

      for (DescIterator i(swapDesc); i(); ++i) {
	std::vector<int> idx_orig = bddIndices(i.val().first);
	std::vector<int> idx_dest = bddIndices(i.val().second);
	for (unsigned int i = 0; i < idx_orig.size(); i++) {
	  perm[idx_orig[i]] = idx_dest[i];
	  permuted.insert(idx_orig[i]);
	}
      }

      for (int i = 0; i < numberOfVars; i++)
	if (permuted.find(i) == permuted.end())
	  perm[i] = i;

      return r.Permute(perm);
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

    BDD discard(BDD r, const int arity, const int left, const int right) {
      const int numberOfVars = Limits::bitsPerInteger * Limits::arity;
      /// \todo Add code to handle the cases when left and right are mistaken
      //assert( arity >= left && arity >= right && "The shift cannot be carried out");

      // This array is used to store the definition of the permutation
      int perm[numberOfVars];
      std::vector<int> quantIndices;
      quantIndices.reserve((left - right + 1)*Limits::bitsPerInteger);

      // Move the variables of the relation itself
      for (int i = 0; i < Limits::arity; i++) {
	for (int j = i; j < numberOfVars;) {
	  if (i >= right && i <= left) {
	    // The current variable is used to represent one of the columns that must
	    // be discarded
	    perm[j] = j + arity;
	    quantIndices.push_back(j + arity);
	  } else if (i > left && i < arity) {
	    // The current variable is after the range of columns that was moved.
	    // it will be moved back to replace the discarded columns
	    perm[j] = (j - left - right + 1);
	  } else {
	    perm[j] = j;
	  }
	  j += Limits::arity;
	}
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


