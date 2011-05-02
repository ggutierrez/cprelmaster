#ifndef __GELISP_GELISP_HH__
#define __GELISP_GELISP_HH__

#include <vector>
#include <gecode/int.hh>

using Gecode::IntSet;
using Gecode::Space;
using Gecode::IntVar;
using std::vector;

namespace GeLisp {
  void test(void);
  
  /// Function to create an IntSet
  IntSet intSet(int n, int m);
  /// Print an IntSet
  void printIntSet(const IntSet& is);

  /// Class to represent a constraint problem in lisp
  class GlSpace : public Space {
  private:
    friend int intVar(GlSpace& home, const IntSet& is);
    /// Cloning a space
    GlSpace(bool share, GlSpace&);
    /// Array of declared IntVar's
    vector<IntVar> iv;
    vector<int> iv_idx;
  public:
    /// Constructor for  an empty space
    GlSpace(void);
    /// Destructor
    virtual ~GlSpace(void);
    /// Copy
    virtual Space* copy(bool share);
    /// Print space information to stderr
    void info(void) const;
    // Temporal: branch on all the integer variables
    void intBranch(void);
  };

  int intVar(GlSpace& home, const IntSet& is);
  void searchAll(GlSpace& root);
}
#endif
