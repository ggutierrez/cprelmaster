#include <gelisp/gelisp.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;


namespace GeLisp {
  void test(void) {
//    freopen ("/tmp/mystdout_minimal.txt","w",stdout);
//    freopen ("/tmp/mystderr_minimal.txt","w",stderr);
    cerr << "This is a test" << endl;
  }

  IntSet intSet(int n, int m) {
    return IntSet(n,m);
  }

  IntSet domain(int *d, int size) {
    cout << "Called dom" << endl;
    return IntSet(d,size);
  }

  void printIntSet(const IntSet& is) {
    cout << is << endl;
  }

  // GlSpace
  GlSpace::GlSpace(void)
    : iv(0)
    , rv(0)
  {}
  GlSpace::~GlSpace(void) {}
  GlSpace::GlSpace(bool share, GlSpace& sp)
    : Space(share,sp)
    , iv(sp.iv)
    , rv(sp.rv)
  {
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
      //CPRelVarArgs rvArgs;
      for (unsigned int i =0; i < rv.size(); i++) {
        //rvArgs << rv[i];
        branch(*this,rv[i]);
      }
      cerr << "Posted brancher on " << rv.size() << " variables" << endl;
    }
  }

  void GlSpace::print(std::ostream& os) const {
    os << "Space information" << endl
       << "\tInteger variables: " << iv.size() << endl;

    for (unsigned int i = 0; i < iv.size(); i++)
      os << "\t\tIntVar(" << i << ") " << iv[i] << endl;

    os << "\tRelation variables: " << rv.size() << endl;
    for (unsigned int i = 0; i < rv.size(); i++)
      os << "\t\tRelVar(" << i << ") " << rv[i] << endl;
  }

  int GlSpace::intVar(const IntSet &is) {
    iv.push_back(IntVar(*this,is));
    return iv.size()-1;
  }

  int GlSpace::relVar(void) {
    GRelation x(2);
    rv.push_back(CPRelVar(*this,x,x));
    return rv.size()-1;
  }

  GlSpace* newSpace(void) {
    return new GlSpace;
  }

  // Variable declaration
  int intVar(GlSpace& home,const IntSet& is) {
    return home.intVar(is);
  }

  int relVar(GlSpace& home) {
    //CPRelVar rv(home,lb,ub);
//    home.rv.push_back(rv);
//    int pos = home.rv.size()-1;
    //cerr << "Created RelVar: " << rv << endl;
//    return pos;
    return home.relVar();
  }

  // searchAll
  void searchAll(GlSpace& root) {
    /*
    Gecode::DFS<GlSpace> e(&root);
    int solutions = 0;
    while (Space* s = e.next()) {
      static_cast<GlSpace*>(s)->print(cout);
      solutions++;
      delete s;
    }
   cerr << "Finished search all: " << solutions << endl;
   */
    using namespace Gecode;
    Gist::Print<GlSpace> p("Print solution");
    Gist::Options o;
    o.inspect.click(&p);
    Gist::dfs(&root,o);

  }
}
