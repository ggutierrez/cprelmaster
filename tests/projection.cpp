#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using std::cout;
using std::cerr;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;

pair<GRelation,GRelation> domR(void) {

  GRelation ub(2);
  ub.add({0,0});
  //ub.add({0,1});

  return make_pair(GRelation(2),ub);
}

pair<GRelation,GRelation> domQ(void) {

  GRelation ub(1);
  ub.add({0});
  ub.add({1});

  return make_pair(GRelation(1),ub);
}

class EqualityTest : public Gecode::Space {
protected:
  CPRelVar r,q;
public:
  EqualityTest(void)  {

    pair<GRelation,GRelation> dr = domR();
    r = CPRelVar(*this,dr.first,dr.second);
    pair<GRelation,GRelation> dq = domQ();
    q = CPRelVar(*this,dq.first,dq.second);

    ProjDescriptor p({0});

    //std::cerr << "R:" << r.lub().project(p) << std::endl;
    //std::cerr << "Q:" << q << std::endl;

    projection(*this,p,r,q);
    branch(*this,r);
//    branch(*this,q);
}

void printHtml(std::ostream& os, const char* varName, CPRelVar v) const {
  os << "<tr><td><b>" << varName << "</b></td>";
  os << "<td>" << v.glb() << "</td><td>"
     << v.unk() << "</td><td>" << v.oob() << "</td><td>"
     << (v.assigned()? "Yes" : "NO") << "</td></tr>";
}

  void print(std::ostream& os) const {
  os << "<b>Space</b>" << std::endl;
  os << "<table border=\"1\">"
     << "<tr><th>Var</th><th>GLB</th><th>UNK</th><th>OOB</th><th>ASG?</th></tr>";

  printHtml(os,"R",r);
  printHtml(os,"Q",q);
  os << "</table>" << std::endl;
  }

  EqualityTest(bool share, EqualityTest& s)
    : Gecode::Space(share,s) {
    r.update(*this, share, s.r);
    q.update(*this, share, s.q);
  }
  virtual Space* copy(bool share) {
    return new EqualityTest(share,*this);
  }
};

int main(int, char**) {
  EqualityTest* g = new EqualityTest();

  Gist::Print<EqualityTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;

  return 0;
}
