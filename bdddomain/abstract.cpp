#include <bdddomain/encoding.hh>
#include <bdddomain/bdd.hh>

namespace MPG { namespace VarImpl {
    /*
     * Existential quantifier
     */
    
    BDD exists(const std::vector<int>& indices, BDD r) {
      return r.ExistAbstract(makeCube(indices));
    }

    BDD exists(int c, BDD r) {
      return r.ExistAbstract(makeCube(c));
    }

    BDD exists(int first, int last, BDD r) {
      assert(first <= last && "emtpy column identifier set");
      std::vector<int> indices;
      for (int i = first; i <= last; i++)
	indices.push_back(i);
      
      return exists(indices, r);
    }
    
    /*
     * Unique quantifier
     */
       
    /**
     * \brief Recursive method helper for unique quantification
     */
    BDD uniqueRec(BDD r, BDD c) {
      if (r == one() || r == zero() || c == one())
	return r;
      
      int levelR = var2Level(var(r));
      int levelC = var2Level(var(c));
      
      if (levelR > levelC) {
	return factory().bddZero();
      }

      BDD res;
      if (levelR == levelC) {
	BDD u_low = uniqueRec(low(r), high(c));
	BDD e_high = high(r).ExistAbstract(high(c));
	
	BDD u_high = uniqueRec(high(r), high(c));
	BDD e_low = low(r).ExistAbstract(high(c));
	
	res = (u_low & !e_high) | (u_high & !e_low);
      } else {
	BDD u_low = uniqueRec(low(r), c);
	BDD u_high = uniqueRec(high(r), c);
	//BDD root = bdd_ithvar(bdd_var(rel));
	BDD root(factoryPtr(),Cudd_Regular(r.getNode()));
	res = factory().bddVar(var(r)).Ite(u_high,u_low);
      }
      return res;
    }
    
    /**
     * \brief Abstract the variables in \a r by unique quantification
     * with respect to variables in \a c.
     */
    BDD unique(BDD r, BDD c) {
      if (c == one()) 
	return r;
      
      return uniqueRec(r,c);
    }

    BDD unique(int c, BDD r) {
      return unique(r,makeCube(c));
    }

    BDD unique(const std::vector<int>& c, BDD r) {
      return unique(r,makeCube(c));
    }

    /*
     * Universal quantifier
     */
    
    BDD forall(int c, BDD r) {
      return r.UnivAbstract(makeCube(c));
    }
  }
}
