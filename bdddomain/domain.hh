#ifndef __BDDDOMAIN_DOMAIN_HH__
#define __BDDDOMAIN_DOMAIN_HH__

#include <bdddomain/bdd.hh>
#include <bdddomain/cuddFactory.hh>
#include <string>
#include <vector>

namespace MPG { namespace VarImpl {
    /**
     * \brief Class to represent a column in a relation using a BDD.
     * 
     * This class is used to abstract a set of bdd variables that
     * represent a column in a relation. It helps to enforce some
     * properties during the creation of the bdd and during the
     * encoding of the data.
     */
    class BDDDomain {
    private:
      /// Reference to the factory used by this domain
      CUDDBDDFactory* factory_;
      /// Index of the domain
      int index_;
      /// Size of the represented domain
      int size_;
      /// Name for the domain
      std::string name_;
      /**
       * \brief Indices of the bdd variables representing the set.
       * 
       * This vector stores the index of the bdd variables used to
       * represent the domain. The number of variables depends on the
       * \a size of the domain.
       */
      std::vector<int> varIndexes_;
    public:
      /// Default constructor
      BDDDomain(void) {}
      /**
       * \brief Constructs a domain with index \a index that can
       * represent from 0 up to \a maximum.
       *
       */
      BDDDomain(CUDDBDDFactory* factory, int index, int maximum, std::vector<int>& varIndexes) 
	: factory_(factory), index_(index), size_(maximum), varIndexes_(varIndexes) {
	if (maximum < 0) {
	  std::cout << "Attempt to create domain with negative maximum " << maximum << std::endl;
	  assert(false);
	}
      }
      /**
       * \brief Returns a bdd representing the given value for this domain
       */
      Bdd ithVar(int val) const {
	if (val > size_ || val < 0) {
	  std::cout << "Value is out of range " << val << std::endl;
	  assert(false);
	}
	CUDDBDDFactory& factory = *factory_;
	Bdd x(factory.one());
	for (unsigned int n = 0; n < varIndexes_.size(); n++) {
	  if ( val & 1 ) 
	    x.andWith(factory.ithVar(varIndexes_.at(n)));
	  else
	    x.andWith(factory.nithVar(varIndexes_.at(n)));
	  val = val >> 1;
	}
	return x;
      }
      /**
       * \brief Computes the number of variables needed to represent a
       * value in the range [0,maximum]
       */
      static int neededVariables(int maximum) {
	int numberOfBits = 1;
	int maxRepresentable = (1 << numberOfBits) - 1;
	while (maxRepresentable < maximum) {
	  numberOfBits++;
	  maxRepresentable = (1 << numberOfBits) - 1;
	}
	std::cout << "Created domain for " << maximum << " with " << numberOfBits << " variables" << std::endl;
	return numberOfBits;
      }
      /**
       * \brief Returns a vector containing the indexes of the bdd
       * variables used to represent the domain.
       */
      const std::vector<int>& vars(void) const {
	return varIndexes_;
      }
      /**
       * \brief Return the maximum value of the domain
       */
      int maximum(void) const {
	return size_;
      }
      /**
       * \brief Returns a cube with the variables used to represent the domain
       */
      Bdd cube(void) {
	return factory_->cube(varIndexes_);
      }
    };
  }}

#endif
