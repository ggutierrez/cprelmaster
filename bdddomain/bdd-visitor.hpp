#include <vector>
#include <map>
#include <bdddomain/manager.hh>
#include <bdddomain/bdd.hh>

namespace MPG {
  namespace VarImpl {
    /**
     * \brief class to store the information of a range.
     */
    class OutputBuffer {
    private:
      int domain_;
      int lastLow_;
      int lastHigh_;
      bool done_;
      std::vector<std::pair<int,int>>& values_;
      int pos_;
    public:
      OutputBuffer(int domain, std::vector<std::pair<int,int>>& values) 
	: domain_(domain), lastHigh_(-2), done_(false), values_(values), pos_(0) {
	//values_.push_back(-5);
	//values_.push_back(-5);
      }
      
      void append(int low, int high) {
	if (low == lastHigh_ + 1)
	  lastHigh_ = high;
	else {
	  finish();
	  lastLow_ = low; lastHigh_ = high;
	}
      }
     
      void finish(void) {
	if (lastHigh_ != -2) {
	  if (done_) {
	    values_.push_back(std::make_pair(-5,-5));
	  }
	  if (lastLow_ == lastHigh_) {
	    values_[values_.size() - 1] = std::make_pair(lastHigh_, lastHigh_);
	    /*
	    auto size = values_.size();
	    values_[size - 2] = lastHigh_;
	    values_[size - 1] = lastHigh_;
	    */
	  } else {
	    values_[values_.size() - 1] = std::make_pair(lastLow_, lastHigh_);
	    /*
	    auto size = values_.size();
	    values_[size - 2] = lastLow_;
	    values_[size - 1] = lastHigh_;
	    */
	    }
	  lastHigh_ = -2;
	}
	done_ = true;
      }

      void append(int low) {
	append(low,low);
      }
    };
    
    inline
    void traverseSet_helper(OutputBuffer& sb, int value, int i, const std::vector<int>& set, 
			 const std::vector<int>& var, int maxSkip) 
    {
      if (i == maxSkip) {
	int bit = (1 << (i + 1)) - 1;
	int maxValue = value | bit;
	sb.append(value,maxValue);
	return;
      }
      int val = set.at(var.at(i));
      if (val == 0) {
	int temp = (value | (1 << i));
	traverseSet_helper(sb, temp, i-1, set, var, maxSkip);
      }
      traverseSet_helper(sb, value, i-1, set, var, maxSkip);
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
    void traverseSet_rec(Cudd& factory, const BDD& r, Functor& fc, std::vector<int>& set) {
      // the number of domains is the number of columns
      int fdvarnum = Limits::arity;
      
      if (r == factory.bddZero()) 
	return;
      else if (r == factory.bddOne()) {
	// we got a branch that goes to terminal 1
	std::vector<std::vector<std::pair<int,int>>> decoded(fdvarnum);
	for (int n = fdvarnum; n--;) {
	  /// \todo The decoding performed by this function relies on
	  /// the order of the bits. As each bit is represented by a
	  /// bdd variable and the way we encode the values is the
	  /// oposite we need the function domainIndices to reverse
	  /// the vector before returnng it. This is something that
	  /// should be fixed in a further release.
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
	    for (int i = 0; i < domain_n_varnum; ++i) {
	      int val = set.at(var.at(i));
	      if (val == 0) {
		hasDontCare = true;
		if (maxSkip == i-1)
		  maxSkip = i;
	      }
	    }

	    // decode the number represented by this column
	    int pos = 0;
	    //for (int i = 0; i < domain_n_varnum; i++) {
	    for (int i = domain_n_varnum - 1; i >= 0;  --i) {
	      pos <<= 1;
	      int val = set.at(var.at(i));
	      if (val == 2)
		pos |= 1;
	    }
	    
	    // print the number
	    if (!hasDontCare) {
	      decoded[n].push_back(std::make_pair(pos,pos)); 
	    } else {
	      // here we have to call the helper to get all the
	      // possibilities for the dont care.
	      OutputBuffer ob(n,decoded[n]);
	      traverseSet_helper(ob, pos, domain_n_varnum-1, set, var, maxSkip);
	      ob.finish();
	    }

	  }
	  else {
	    decoded[n].push_back(std::make_pair(-2,-2));
	  }
	}

	// call the functor
	fc(decoded);
      } else {
	set[var(r)] = 1;
	BDD lo = low(r);
	traverseSet_rec(factory, lo, fc, set);
	
	set[var(r)] = 2;
	BDD hi = high(r);
	traverseSet_rec(factory, hi, fc, set);
	
	set[var(r)] = 0;
      }
    }
    
    template <typename Functor>
    void traverseSet(Cudd& factory, const BDD& b, Functor& fc) {
      int variables = 1 << (Limits::bbv + Limits::ba);
      std::vector<int> set(variables,0);
      traverseSet_rec(factory, b, fc, set);
    }
  }
}
