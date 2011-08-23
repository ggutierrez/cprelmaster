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
  GRelation ub(3);
  ub.add({
   {1,3,2},
   {4,5,7},
   {8,2,2},
   {6,5,3}
  });

  GRelation lb(3);
  lb.add(make_Tuple(8,2,2));

  //return make_pair(lb,ub);
  return make_pair(GRelation(3),ub);
  }

pair<GRelation,GRelation> domS(void) {
  GRelation ub(2);
  ub.add({
   {5,1},
   {2,8},
  // {2,9},
   {7,4}
  });
  return make_pair(GRelation(2),ub);
}

pair<GRelation,GRelation> domT(void) {
  GRelation ub = create_full(3);

  GRelation lb(3);
  //lb.add(make_Tuple(8,2,8));

  return make_pair(lb,ub);
}

class FollowTest : public Gecode::Space {
protected:
  CPRelVar r,s,t;
public:
  FollowTest(void)  {

   pair<GRelation,GRelation> dr = domR();
   r = CPRelVar(*this,dr.first,dr.second);

   pair<GRelation,GRelation> ds = domS();
   s = CPRelVar(*this,ds.first,ds.second);

   pair<GRelation,GRelation> dt = domT();
   t = CPRelVar(*this,dt.first,dt.second);

   follow(*this,r,1,s,t);
//   branch(*this,r);
//   Gist::stopBranch(*this);
//   branch(*this,s);
//   Gist::stopBranch(*this);
   branch(*this,t);
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
    print(os,"R",r);
    print(os,"S",s);
    print(os,"T",t);
    os << "</table>" << std::endl;
  }
  FollowTest(bool share, FollowTest& sp)
    : Gecode::Space(share,sp) {
    r.update(*this, share, sp.r);
    t.update(*this, share, sp.t);
    s.update(*this, share, sp.s);
  }
  virtual Space* copy(bool share) {
    return new FollowTest(share,*this);
  }
};

int main(int, char**) {
  // Setup the way tuples are printed
//  std::cout << TupleIO("<td>","</td>"," ");
  // Setup the way relations are printed
//  std::cout << GRelationIO("<table>","</table>","<tr>","</tr>");

  FollowTest* g = new FollowTest();

  Gist::Print<FollowTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;

  return 0;
}
