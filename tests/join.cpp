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

GRelationIO custom("<table><tr>","</tr></table>","","</tr><tr>");


pair<GRelation,GRelation> domR(void) {
  std::ifstream inputL("/home/gg/Work/cprelmaster/tests/ground-relations/r3.txt");
  GRelation ub = read(inputL,3);
  return make_pair(GRelation(3),ub);
}

pair<GRelation,GRelation> domS(void) {
  std::ifstream inputR("/home/gg/Work/cprelmaster/tests/ground-relations/s2.txt");
  GRelation ub = read(inputR,2);
  return make_pair(GRelation(2),ub);
}

pair<GRelation,GRelation> domT(void) {
  GRelation ub = create_full(4);
  //GRelation lb(4);
  //lb.add(Tuple({1,3,2,9}));
  //lb.add(Tuple({8,2,2,8}));
  return make_pair(GRelation(4),ub);
  //return make_pair(lb,ub);
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

   join(*this,r,1,s,t);
   //branch(*this,r);
   //branch(*this,s);
   branch(*this,t);
  }
  void printHtml(std::ostream& os, const char* varName, CPRelVar v) const {
    os << "<tr><td><b>" << varName << "</b></td>";
    os << "<td>" << v.glb() << "</td><td>"
       << v.unk() << "</td><td>" /*<< v.oob() */ << "</td><td>"
       << (v.assigned()? "Yes" : "NO") << "</td></tr>";
  }
  void print(std::ostream& os) const {
    std::string str;
    std::stringstream ss(str);

    ss << "<b>Space</b>";
    ss << "<table border=\"1\">"
       << "<tr><th>Var</th><th>GLB</th><th>UNK</th><th>OOB</th><th>ASG?</th></tr>";

    printHtml(ss,"R",r);
    printHtml(ss,"S",s);
    printHtml(ss,"T",t);
    ss << "</table>";

    os << ss.str() << std::endl;

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

  Gist::Print<JoinTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;

  return 0;
}
