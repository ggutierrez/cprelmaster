#include <bdddomain/encoding.hh>
#include <bdddomain/bdd.hh>

namespace MPG { namespace VarImpl {

    /**
     * \brief Helper class used to construct a permutation arrya for
     * the replace operation.
     *
     * This design is taken and modified from JavaBDD.
     */
    class BDDPairing {
    private:
      /// Vector to store the permutation that will be performed
      std::vector<int> perm_;
    public:
      /**
       * \brief Initializes the permutation.
       *
       * By default the permutation is build to not permute
       * anything. This is, the array is filled with the variables in
       * the manager that are in the same order.
       */
      BDDPairing(void) 
        : perm_(Limits::variables) 
      {
        for (int i = 0; i < Limits::variables; i++)
          perm_[i] = i; 
      }
      /**
       * \brief Represents the change of variable at index \a oldVar by
       * variable at index \a newVar.
       */
      void set(int oldVar, int newVar) {
	perm_[oldVar] = newVar;
      }
      /**
       * \brief Represents the change of all the variables at the
       * indices in \a oldVars by the corresponding variables at
       * indices in \a newVars.
       */
      void set(const std::vector<int>& oldVars, const std::vector<int>& newVars) {
        assert(oldVars.size() == newVars.size() && "Invalid permutation");
        for (unsigned int i = 0; i < oldVars.size(); i++)
          set(oldVars.at(i),newVars.at(i));
      }
      /**
       * \brief Returns the permutation specification
       */
      std::vector<int> get(void) {
        return perm_;
      };
    };
    
    BDD replace(const std::vector<std::pair<int,int> >& pairing, const BDD& r) {
      BDDPairing p;
      // elements in pairing are columns, first we need to get those
      // columns in terms of BDD variables.
      for (const auto& i : pairing) {
        //std::cout << "Must permute " << i.first << " by " << i.second << std::endl;
        const auto& oldVars = domainIndices(i.first);
        const auto& newVars = domainIndices(i.second);
        p.set(oldVars,newVars);
      }
      auto perm = p.get();
      return r.Permute(&perm[0]);
    }

    BDD swap(const std::vector<std::pair<int,int>>& pairing, const BDD& r) {
      int bddVars = Limits::bitsPerInteger * pairing.size();
      BDDvector oldVars(bddVars), newVars(bddVars);
      
      int last = 0;
      for (const auto& i : pairing) {
        //std::cout << "Must swap " << i.first << " by " << i.second << std::endl;
        const auto& ov = domainIndices(i.first);
        const auto& nv = domainIndices(i.second);
        assert(ov.size() == nv.size() && "Unexpected size in returned vector");
        for (unsigned int v = 0; v < ov.size(); v++, last++) {
          oldVars[last] = factory().bddVar(ov.at(v));
          newVars[last] = factory().bddVar(nv.at(v));
        }
      }
      //std::cout << "Last: " << last << " variables: " << bddVars << std::endl;
      assert(last == bddVars  && "Unexpected size for swapping");
      return r.SwapVariables(oldVars,newVars);
    }
  }
}
