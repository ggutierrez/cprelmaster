#ifndef __BDDDOMAIN_DOMAIN_SPACE_HH__
#define __BDDDOMAIN_DOMAIN_SPACE_HH__

#include <bdddomain/cuddFactory.hh>
#include <bdddomain/domain.hh>
#include <initializer_list>
#include <vector>

namespace MPG { namespace VarImpl {
    
    /**
     * \brief A Domain space object support the domain representation
     * of relations.
     *
     * For now only one object of this class can be created (singleton pattern).
     *
     * Important note: The implementation of this class is based on
     * the implementation present in the JavaBDD framework. Al the
     * credit for its design is to the developers of this package. The
     * package can be downloaded from the website:
     * http://javabdd.sourceforge.net/ . This development is based on
     * that package, the way it abstracts over the different bdd
     * implementations and some of the algorithms presented here are
     * modifications of the ones in the original package.
     */
    class DomainSpace {
    private:
      CUDDBDDFactory factory_;
      std::vector<BDDDomain> columns_;
      int usedVariables;
    public:
      /**
       * \brief Constructs a domain space.
       *
       * The number of columns is set to \a columns and the maximum
       * value that can appear in a column is \a maximum.
       */
      DomainSpace(int columns, int maximum) : factory_(100,1000), columns_(columns) {
	if (maximum < 0) {
	  std::cout << "Attempt to create domain with negative maximum " << maximum << std::endl;
	  assert(false);
	}
	int vars = BDDDomain::neededVariables(maximum);	
	std::vector<std::vector<int>> columnVars(columns);
	
	usedVariables = vars * columns;
	// distribute the variables indexes
	for (int i = 0; i < usedVariables;) {
	  for (int j = 0; j < columns; j++) {
	    columnVars[j].push_back(i);
	    i++;
	  }
	}
	/// create the BDDDomain for every column
	for (int i = 0; i < columns; i++) {
	  columns_[i] = BDDDomain(&factory_,i,maximum,columnVars.at(i));
	}
	//std::cout << "Needed variables per column " << vars << std::endl;
      }
      /**
       * \brief Returns a bdd representing the tuple descirbed by the list.
       */
      Bdd represent(std::initializer_list<int> tuple) {
	if (tuple.size() > columns_.size()) {
	  std::cout << "Big problem, the domain space is not configure for this length " 
		    << tuple.size() << std::endl;
	  assert(0);
	}
	Bdd r = factory_.one();
	int i = 0;
	for (int e : tuple) {
	  r.andWith(columns_[i].ithVar(e));
	  i++;
	}
	return r;
      }
      
      /**
       * \brief Returns a cube with the bdd variables used to represent
       */
      /**
       * \brief Prints \a bdd as a set to \a os.
       */
      void print(std::ostream& os, Bdd& bdd) {
	std::vector<int> set(usedVariables,0);
	printsetRec(os, bdd, set, columns_.size());
      }
      /**
       * \brief Prints the first \a n columns of the set represented by \a bdd.
       */
      void print(std::ostream& os, Bdd& bdd, int n) {
	if (static_cast<unsigned int>(n) > columns_.size()) {
	  std::cout << "Asked to print more columns that existent " << n << std::endl;
	  assert(false);
	}
	std::vector<int> set(usedVariables,0);
	printsetRec(os, bdd, set, n);
      }
    private:
      /**
       * \brief Class to represent a subrange of the elements that
       * have to be printed.
       */
      class OutputBuffer {
	/// Stream where the elements have to be printed at the end
	std::ostream& os_;
	/// Column where the data stored belongs to
	int column_;
	int lastLow_;
	int lastHigh_;
	bool done;
      public:
	/// Constructor
	OutputBuffer(std::ostream& os, int column)
	  : os_(os), column_(column), lastLow_(0), lastHigh_(-2) {}
	/// Append an interval to the buffer
	void append(int low, int high) {
	  if (low == (lastHigh_ + 1)) {
	    lastHigh_ = high;
	  } else {
	    finish();
	    lastLow_ = low;
	    lastHigh_ = high;
	  }
	}
	/// Append an element to the buffer
	void append(int low) {
	  append(low,low);
	}
	/// Write the information contained in the buffer
	void finish(void) {
	  if (lastHigh_ != -2) {
	    if (done) os_ << "/";
	    if (lastLow_ == lastHigh_)
	      os_ << lastHigh_;
	    else
	      os_ << "[" << lastLow_ << "..." << lastHigh_ << "]";
	    lastHigh_ = -2;
	  }
	  done = true;
	}
      };
      void printSetHelper(OutputBuffer& ob, int value, int i, std::vector<int>& set,
			  std::vector<int> var, int maxSkip) {
	if (i == maxSkip) {
	  int maxValue = value | ((1 << (i+1)) - 1);
	  ob.append(value, maxValue);
	  return;
	}
	int val = set.at(var.at(i));
	if (val == 0) {
	  int temp = value | (1<<i);
	  printSetHelper(ob, temp, i-1, set, var, maxSkip);
	}
	printSetHelper(ob, value, i-1, set, var, maxSkip);
      }
      /**
       * \brief Helper method for printing \a bdd as a set to \a os.
       */
      void printsetRec(std::ostream& os, Bdd& bdd, std::vector<int>& set, int numCols) {
	if (bdd.isZero()) {
	  // we do not care about this set
	  return;
	} else if (bdd.isOne()) {
	  // for now print set
	  os << "<";

	  bool first = true;
	  bool used = false;
	  
	  for (int n = 0; n < numCols; n++) {
	    BDDDomain& domain_n = columns_.at(n);
	    const std::vector<int> domain_n_ivar = domain_n.vars();
	    int domain_n_varnum = domain_n_ivar.size();
	    for (int m = 0; m < domain_n_varnum; m++)
	      if (set.at(domain_n_ivar.at(m)) != 0) 
		used = true;

	    if (used) {
	      if (!first)
		os << ", ";
	      first = false;
	      os << "\"" << n << "\":";
	      
	      std::vector<int> var(domain_n_ivar);
	      int pos = 0;
	      int maxSkip = -1;
	      bool hasDontCare = false;
	      for(int i = 0; i < domain_n_varnum; ++i) {
		int val = set.at(var.at(i));
		if (val == 0) {
		  hasDontCare = true;
		  if (maxSkip == i-1)
		    maxSkip = i;
		}
	      }
	      
	      for (int i = domain_n_varnum-1; i >= 0; --i) {
		pos = pos << 1;
		int val = set.at(var.at(i));
		if (val == 2) {
		  pos = pos|1;
		}
	      }
	      if (!hasDontCare) {
		os << pos;
	      } else {
		OutputBuffer ob(os,n);
		printSetHelper(ob, pos, domain_n_varnum-1, set, var, maxSkip);
		ob.finish();
	      }
	    }
	  }
	  os << ">" << std::endl;
	} else {
	  // we are in a node
	  set[bdd.var()] = 1;
	  Bdd lo = bdd.low();
	  printsetRec(os, lo, set, numCols);
	  
	  set[bdd.var()] = 2;
	  Bdd hi = bdd.high();
	  printsetRec(os, hi, set, numCols);
	  
	  set[bdd.var()] = 0;
	}
	
      }
      
    };
  }}

#endif
