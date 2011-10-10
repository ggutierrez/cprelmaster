#include <bdddomain/domainSpace.hh>

namespace MPG { namespace VarImpl {

    DomainSpace::DomainSpace(int columns, int maximum) : factory_(100,1000), columns_(columns) {
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
      std::cout << "Needed variables per column " << vars << std::endl;
    }

    Bdd DomainSpace::represent(std::initializer_list<int> tuple) {
      if (tuple.size() > columns_.size()) {
	/// \todo: replace this code with an exception
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

    void printHeader(std::ostream& os, int cols) {
      os << "|";
      for (int i = 0; i < cols; i++) {
	os << "---";
	if (i != cols - 1)
	  os << "+";
      }
      os << "|" << std::endl << "|";
      for (int i = 0; i < cols; i++)
	os << "Col(" << i << ") |";
      
      os << std::endl << "|";
      for (int i = 0; i < cols; i++) {
	os << "---";
	if (i != cols - 1)
	  os << "+";
      }
      os << "|" << std::endl;      
    }

    void printFooter(std::ostream& os, int cols) {
      os << "|";
      for (int i = 0; i < cols; i++) {
	os << "---";
	if (i != cols - 1)
	  os << "+";
      }
      os << "|" << std::endl;
    }

    void DomainSpace::print(std::ostream& os, Bdd& bdd) {
      std::cout << "From the other print " <<  std::endl;
      printHeader(os, columns_.size());
      std::vector<int> set(usedVariables,0);
      printsetRec(os, bdd, set, columns_.size());
      printFooter(os, columns_.size());
    }

    void DomainSpace::print(std::ostream& os, Bdd& bdd, int n) {
      std::cout << "From print " << n << std::endl;
      if (static_cast<unsigned int>(n) > columns_.size()) {
	std::cout << "Asked to print more columns that existent " << n << std::endl;
	assert(false);
      } 
      if (bdd.isZero()) {
	os << "Empty" << std::endl;
      } else {
	printHeader(os,n);
	std::vector<int> set(usedVariables,0);
	printsetRec(os, bdd, set, n);
	printFooter(os,n);
      }
    }

    void DomainSpace::printSetHelper(OutputBuffer& ob, int value, int i, std::vector<int>& set,
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

    void DomainSpace::printsetRec(std::ostream& os, Bdd& bdd, std::vector<int>& set, int numCols) {
      if (bdd.isZero()) {
	// we do not care about this set
	return;
      } else if (bdd.isOne()) {
	// for now print set
	//os << "<";
	os << "|";

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
	      //os << ", ";
	      os << "| ";
	    first = false;
	    //os << "\"" << n << "\":";
	      
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
	//os << ">" << std::endl;
	os << "|" << std::endl;
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

    DomainSpace::OutputBuffer::OutputBuffer(std::ostream& os, int column)
      : os_(os), column_(column), lastLow_(0), lastHigh_(-2) {}

    void DomainSpace::OutputBuffer::append(int low, int high) {
      if (low == (lastHigh_ + 1)) {
	lastHigh_ = high;
      } else {
	finish();
	lastLow_ = low;
	lastHigh_ = high;
      }
    }

    void DomainSpace::OutputBuffer::append(int low) {
      append(low,low);
    }

    void DomainSpace::OutputBuffer::finish(void) {
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


  }}
