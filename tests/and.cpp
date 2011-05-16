#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>
#include <cprel/prop/and.hh>
#include <vector>

using namespace Gecode;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;
using namespace MPG::CPRel::Prop;

pair<GRelation,GRelation> domR(void) {

  GRelation ub(2);
  ub.add(Tuple(0,0));
  ub.add(Tuple(0,1));

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
  lb.add(Tuple(0,0));

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

    intersect(*this,r,q,t);

    branch(*this,r);
  }
  void print(std::ostream& os) const {
    os << "<b>Space</b>" << std::endl;
    os << "<b>R</b>: " << r << std::endl
       << "<b>Q</b>: " << q << std::endl
       << "<b>T</b>: " << t << std::endl;
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
