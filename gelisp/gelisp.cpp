#include <gelisp/gelisp.hh>
#include <gecode/search.hh>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;


namespace GeLisp {
  void test(void) {
    freopen ("/tmp/mystdout_minimal.txt","w",stdout);
    freopen ("/tmp/mystderr_minimal.txt","w",stderr);
    cerr << "This is a test" << endl;
  }

  IntSet intSet(int n, int m) {
    return IntSet(n,m);
  }
  void printIntSet(const IntSet& is) {
    cout << is << endl;
  }

  // GlSpace
  GlSpace::GlSpace(void)
    : iv(0), iv_idx(0) {}
  GlSpace::~GlSpace(void) {}
  GlSpace::GlSpace(bool share, GlSpace& sp)
    : Space(share,sp), iv(sp.iv) {
    for (int i = iv.size(); i--; )
      iv[i].update(*this,share,sp.iv[i]);
  }
  Space* GlSpace::copy(bool share) {
    return new GlSpace(share, *this);
  }
  void GlSpace::intBranch(void) {
    using namespace Gecode;
    using namespace Int;
    if (iv.size() > 0) {
      IntVarArgs ivArgs;
      for (int i =0; i < iv.size(); i++) ivArgs << iv[i];
      branch(*this,ivArgs,INT_VAR_SIZE_MIN, INT_VAL_MIN);
      cerr << "Posted brancher on " << iv.size() << " variables" << endl;
    }
  }
  void GlSpace::info(void) const {
    cerr << "Space information" << endl
	 << "\tInteger variables: " << iv.size() << endl;

    for (int i = 0; i < iv.size(); i++)
      cout << "\t\tIntVar(" << i << ") " << iv[i] << endl;
  }
  
  // Variable declaration
  int intVar(GlSpace& home,const IntSet& is) {
    IntVar iv(home,is);
    home.iv.push_back(iv);
    int pos = home.iv.size()-1;
    home.iv_idx.push_back(pos);
    //cerr << "Created IntVar: " << iv << endl;
    return pos;
  }
  
  // searchAll
  void searchAll(GlSpace& root) {
    Gecode::DFS<GlSpace> e(&root);
    while (Space* s = e.next()) {
      static_cast<GlSpace*>(s)->info();
      delete s;
    }
    cerr << "Finished search all" << endl;
  }
}
