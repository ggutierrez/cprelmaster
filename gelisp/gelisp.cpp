#include <gelisp/gelisp.hh>
#include <gecode/search.hh>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;


namespace GeLisp {
  void test(void) {
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
    : iv(0), rv(0) {}
  GlSpace::~GlSpace(void) {}
  GlSpace::GlSpace(bool share, GlSpace& sp)
    : Space(share,sp), iv(sp.iv), rv(sp.rv) {
    for (int i = iv.size(); i--; )
      iv[i].update(*this,share,sp.iv[i]);
    for (int i = rv.size(); i--; )
      rv[i].update(*this,share,sp.rv[i]);
  }
  Space* GlSpace::copy(bool share) {
    return new GlSpace(share, *this);
  }
  void GlSpace::intBranch(void) {
    using namespace Gecode;
    if (iv.size() > 0) {
      IntVarArgs ivArgs;
      for (unsigned int i =0; i < iv.size(); i++) ivArgs << iv[i];
      branch(*this,ivArgs,INT_VAR_SIZE_MIN, INT_VAL_MIN);
      cerr << "Posted brancher on " << iv.size() << " variables" << endl;
    }
  }
  void GlSpace::relBranch(void) {
    using namespace MPG;
    if (rv.size() > 0) {
      CPRelVarArgs rvArgs;
      for (unsigned int i =0; i < rv.size(); i++) rvArgs << rv[i];
      branch(*this,rvArgs);
      cerr << "Posted brancher on " << rv.size() << " variables" << endl;
    }
  }
  
  void GlSpace::info(void) const {
    cerr << "Space information" << endl
	 << "\tInteger variables: " << iv.size() << endl;

    for (unsigned int i = 0; i < iv.size(); i++)
      cout << "\t\tIntVar(" << i << ") " << iv[i] << endl;
    
    cerr << "\tRelation variables: " << rv.size() << endl;
    for (unsigned int i = 0; i < rv.size(); i++)
      cout << "\t\tRelVar(" << i << ") " << rv[i] << endl;
  }
  
  // Variable declaration
  int intVar(GlSpace& home,const IntSet& is) {
    IntVar iv(home,is);
    home.iv.push_back(iv);
    int pos = home.iv.size()-1;
    //cerr << "Created IntVar: " << iv << endl;
    return pos;
  }
  
  int relVar(GlSpace& home, int a, int b) {
    CPRelVar rv(home,a,b);
    home.rv.push_back(rv);
    int pos = home.rv.size()-1;
    //cerr << "Created RelVar: " << rv << endl;
    return pos;
  }
  
  // searchAll
  void searchAll(GlSpace& root) {
    Gecode::DFS<GlSpace> e(&root);
    int solutions = 0;
    while (Space* s = e.next()) {
      static_cast<GlSpace*>(s)->info();
      solutions++;
      delete s;
    }
    cerr << "Finished search all: " << solutions << endl;
  }
}
