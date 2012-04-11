#include <fstream>
#include <sstream>
#include <string>
#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;

pair<GRelation,GRelation> domR(void) {
  GRelation gr(3);
  gr.add({1,2,3});
  return make_pair(gr,gr);
}

pair<GRelation,GRelation> domS(void) {
  GRelation gr(3);
  gr.add({
      {2,3,7},
      {2,3,8},
      {2,4,9}
    });
  return make_pair(gr,gr);
}

pair<GRelation,GRelation> domT(void) {
  GRelation ub(4);
  ub.add({
      {1,2,3,7},
      {1,2,3,8}
    });
  return make_pair(GRelation(4),ub);
}

class JoinTest : public Gecode::Space {
protected:
  CPRelVar r,s,t;
public:
  JoinTest(void)  {

    pair<GRelation,GRelation> dr = domR();
    r = CPRelVar(*this,dr.first,dr.second);

    pair<GRelation,GRelation> ds = domS();
    s = CPRelVar(*this,ds.first,ds.second);

    pair<GRelation,GRelation> dt = domT();
    t = CPRelVar(*this,dt.first,dt.second);

    join(*this,r,2,s,t);
    branch(*this,r);
    branch(*this,s);
    //branch(*this,t);
  }
  void print(std::ostream& os) const {
    os << "Print method called" << std::endl;
    os << "R: " << r << endl;
    os << "S: " << s << endl;
    os << "T: " << t << endl;

  }
  JoinTest(bool share, JoinTest& sp)
    : Gecode::Space(share,sp) {
    r.update(*this, share, sp.r);
    t.update(*this, share, sp.t);
    s.update(*this, share, sp.s);
  }
  virtual Space* copy(bool share) {
    return new JoinTest(share,*this);
  }
};

int main(int, char**) {
  JoinTest* g = new JoinTest();
  DFS<JoinTest> e(g);

  std::cout << "Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<JoinTest*>(s)->print(std::cout);
    delete s;
  }

  /*
  Gist::Print<JoinTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  */
  delete g;

  return 0;
}
