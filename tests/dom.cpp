#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>
#include <cptuple/cptuple.hh>
#include <vector>

using namespace Gecode;
using namespace MPG;
using namespace MPG::CPRel;
using namespace MPG::CPTuple;

class DomTest : public Gecode::Space {
protected:
  CPRelVar r;
  CPTupleVar t;
public:
  DomTest(void)  {
    GRelation lb(2);
    lb.add(make_Tuple(1,3));
    GRelation ub(2);
    ub.add({{1,3}, {4,5}, {8,2}, {6,5}});

    r = CPRelVar(*this,lb,ub);
    t = CPTupleVar(*this,ub);
    //included(*this,t,r);
    excluded(*this,t,r);

    branch(*this,r);
    Gist::stopBranch(*this);
    branch(*this,t);
  }
  void print(std::ostream& os, const char* varName, CPRelVar v) const {
    os << "<tr><td><b>" << varName << "</b></td>"
       << "<td>" << v.glb() << "</td>"
       << "<td>" << v.unk() << "</td>"
       << "<td>" << (v.assigned()? "Yes" : "NO") << "</td>";
  }
  void print(std::ostream& os, const char* varName, CPTupleVar v) const {
    os << "<tr><td><b>" << varName << "</b></td>"
       << "<td>" << v.domain() << "</td>"
       << "<td>---</td>" << "<td>" << (v.assigned()? "Yes" : "NO") << "</td>";
  }
  void print(std::ostream& os) const {
    os << "<table border=\"1\">"
       << "<tr><th>Var</th><th>GLB</th><th>UNK</th><th>ASG?</th></tr>";
    print(os,"R",r);
    print(os,"T",t);
    os << "</table>" << std::endl;
  }
  DomTest(bool share, DomTest& s)
    : Gecode::Space(share,s) {
    r.update(*this, share, s.r);
    t.update(*this, share, s.t);
  }
  virtual Space* copy(bool share) {
    return new DomTest(share,*this);
  }
};

int main(int, char**) {
  // Setup the way tuples are printed
//  std::cout << TupleIO("<td>","</td>"," ");
  // Setup the way relations are printed
//  std::cout << GRelationIO("<table>","</table>","<tr>","</tr>");

  DomTest* g = new DomTest();

  Gist::Print<DomTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;

  return 0;
}
