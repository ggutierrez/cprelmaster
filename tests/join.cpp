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
  ub.add(make_Tuple(0,0));
  // empty... {[0,0]}
  return make_pair(GRelation(2),ub);
}

pair<GRelation,GRelation> domQ(void) {
  GRelation ub(2);
  ub.add(make_Tuple(0,0));
  // empty... {[0,0]}
  return make_pair(GRelation(2),ub);
}

pair<GRelation,GRelation> domT(void) {
  GRelation ub(2);
  ub.add(make_Tuple(0,0));
  // empty... {[0,0]}
  return make_pair(GRelation(2),ub);
}

pair<GRelation,GRelation> domS(void) {
  GRelation ub = create_full(2);
  // empty...full
  return make_pair(GRelation(2),ub);
}

class AndTest : public Gecode::Space {
protected:
  CPRelVar r,q,t,s;
public:
  AndTest(void)  {

   pair<GRelation,GRelation> dr = domR();
   r = CPRelVar(*this,dr.first,dr.second);

   pair<GRelation,GRelation> dq = domQ();
   q = CPRelVar(*this,dq.first,dq.second);

   pair<GRelation,GRelation> dt = domT();
   t = CPRelVar(*this,dt.first,dt.second);

   pair<GRelation,GRelation> ds = domS();
   s = CPRelVar(*this,ds.first,ds.second);

   join(*this,r,1,q,t);
   branch(*this,t);
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
    printHtml(os,"S",s);
    printHtml(os,"Q",q);
    printHtml(os,"T",t);
    os << "</table>" << std::endl;

  }
  AndTest(bool share, AndTest& sp)
    : Gecode::Space(share,sp) {
    r.update(*this, share, sp.r);
    q.update(*this, share, sp.q);
    t.update(*this, share, sp.t);
    s.update(*this, share, sp.s);
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
