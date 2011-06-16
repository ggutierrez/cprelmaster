#ifndef __GELISP_GELISP_HH__
#define __GELISP_GELISP_HH__

#include <vector>
#include <gecode/int.hh>
#include <cprel/cprel.hh>

using Gecode::IntSet;
using Gecode::Space;
using Gecode::IntVar;
using MPG::CPRelVar;
using MPG::CPRel::GRelation;
using std::vector;

namespace GeLisp {
  void test(void);

  /// Function to create an IntSet
  IntSet intSet(int n, int m);
  IntSet domain(int *d, int size);
  /// Print an IntSet
  void printIntSet(const IntSet& is);

  /// Class to represent a constraint problem in lisp
  class GlSpace : public Space {
  private:
    /// Cloning a space
    GlSpace(bool share, GlSpace&);
    /// Array of declared IntVar's
    vector<IntVar> iv;
    /// Array of declared RelVar's
    vector<CPRelVar> rv;
  public:
    /// Constructor for  an empty space
    GlSpace(void);
    /// Destructor
    virtual ~GlSpace(void);
    /// Copy
    virtual Space* copy(bool share);
    /// Declares an integer variable
    int intVar(const IntSet &is);
    /// Declares a relation variable
    int relVar(void);
    /// Print space information to stderr
    void print(std::ostream& os) const;
    // Temporal: branch on all the integer variables
    void intBranch(void);
    // Temporal: branch on all relation variables
    void relBranch(void);
  };

  GlSpace* newSpace(void);
  /// Create an Integer variable with domain \a is in \a home
  int intVar(GlSpace& home, const IntSet& is);
  /// Create a Relation variable
  int relVar(GlSpace& home);

  void searchAll(GlSpace& root);
}
#endif
