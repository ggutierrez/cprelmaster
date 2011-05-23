#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;

pair<GRelation,GRelation> domR(void) {

  GRelation ub(2);
  ub.add(Tuple(0,0));
  ub.add(Tuple(0,1));

  // empty... {[0,0],[0,1]}]
  return make_pair(GRelation(2),ub);
}

pair<GRelation,GRelation> domQ(void) {

  GRelation ub(2);
  ub.add(Tuple(0,0));
  ub.add(Tuple(0,1));

  return make_pair(GRelation(2),ub);
}

pair<GRelation,GRelation> domT(void) {
  GRelation lb(2);
  //lb.add(Tuple(0,0));

  GRelation ub(2);
  ub.add(Tuple(0,0));
  ub.add(Tuple(0,1));

  return make_pair(lb,ub);
}

class AndTest : public Gecode::Space {
protected:
  CPRelVar r,q,t;
public:
  AndTest(void)  {

   pair<GRelation,GRelation> dr = domR();
   r = CPRelVar(*this,dr.first,dr.second);

   pair<GRelation,GRelation> dq = domQ();
   q = CPRelVar(*this,dq.first,dq.second);

   pair<GRelation,GRelation> dt = domT();
   t = CPRelVar(*this,dt.first,dt.second);

   //   intersect(*this,r,q,t);
   Union(*this,r,q,t);

   branch(*this,r);
   branch(*this,q);
   //   branch(*this,t);
  }
  void printHtml(std::ostream& os, CPRelVar v) const {
    os << "<td>" << v.glb() << "</td><td>"
       << v.unk() << "</td><td>" << v.oob() << "</td><td>"
       << (v.assigned()? "Yes" : "NO") << "</td>";
  }
  void print(std::ostream& os) const {
    os << "<b>Space</b>" << std::endl;
    os << "<table border=\"1\">"
       << "<tr><th>Var</th><th>GLB</th><th>UNK</th><th>OOB</th><th>ASG?</th></tr>"
       << "<tr><td><b>R</b></td>";
    printHtml(os,r);
    os << "</tr>"
       <<"<tr><td><b>Q</b></td>";
    printHtml(os,q);
    os << "</tr>"
       <<"<tr><td><b>T</b></td>";
    printHtml(os,t);
    os << "</tr></table>" << std::endl;

  }
  AndTest(bool share, AndTest& s)
    : Gecode::Space(share,s) {
    r.update(*this, share, s.r);
    q.update(*this, share, s.q);
    t.update(*this, share, s.t);
  }
  virtual Space* copy(bool share) {
    return new AndTest(share,*this);
  }
};

int main(int, char**) {
  AndTest* g = new AndTest();

  Gist::Print<AndTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;

  return 0;
}
