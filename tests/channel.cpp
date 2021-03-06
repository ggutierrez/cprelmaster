#include <boost/chrono.hpp>
#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <gecode/set.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;

pair<GRelation,GRelation> domA(void) {
  GRelation ub(3);
  ub.add({
  {1,3,2},
  {4,5,7},
  {8,2,2},
  {6,5,3}
  });
  return make_pair(GRelation(3),ub);
}

class ChannelTest : public Gecode::Space {
protected:
  CPRelVar a,b;
  SetVar s;
public:
  ChannelTest(void)  {

    pair<GRelation,GRelation> da = domA();
    a = CPRelVar(*this,da.first,da.second);

    b = CPRelVar(*this,GRelation(1),GRelation::create_full(1));

    s = SetVar(*this,IntSet::empty,IntSet(0,7));

    // b is the projection on the first column of a
    projection(*this,1,a,b);
    // Channel b with a set variable
    channel(*this,b,s);
    // Constraint the cardinality of the set
    cardinality(*this,s,2,2);

    // branch on b
    branch(*this,b);
    // visualize when branching on b is exhausted
    Gist::stopBranch(*this);
    // branch on a
    branch(*this,a);
  }
  void print(std::ostream& os, const char* varName, CPRelVar v) const {
    os << "<tr><td><b>" << varName << "</b></td>"
       << "<td>" << v.glb() << "</td>"
       << "<td>" << v.unk() << "</td>"
       << "<td>" << (v.assigned()? "Yes" : "NO") << "</td>";
  }
  void print(std::ostream& os) const {
    os << "<table border=\"1\">"
       << "<tr><th>Var</th><th>GLB</th><th>UNK</th><th>ASG?</th></tr>";
    print(os,"A",a);
    print(os,"B",b);
    os << "</table>";
    os << "Set: " << s << std::endl;
  }
  ChannelTest(bool share, ChannelTest& sp)
    : Gecode::Space(share,sp) {
    a.update(*this, share, sp.a);
    b.update(*this, share, sp.b);
    s.update(*this, share, sp.s);
  }
  virtual Space* copy(bool share) {
    return new ChannelTest(share,*this);
  }
};

int main(int, char**) {
  boost::chrono::system_clock::time_point
      start = boost::chrono::system_clock::now();

  // Setup the way tuples are printed
  std::cout << TupleIO("<td>","</td>"," ");
  // Setup the way relations are printed
  std::cout << GRelationIO("<table>","</table>","<tr>","</tr>");

  ChannelTest* g = new ChannelTest();

  Gist::Print<ChannelTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;

  boost::chrono::duration<double>
      runtime = boost::chrono::system_clock::now() - start;
  std::cout << "Took " << runtime.count() << " seconds\n";
  return 0;
}
