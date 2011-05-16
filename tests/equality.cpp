#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>
#include <cprel/prop/equality.hh>
#include <vector>

using namespace Gecode;

using namespace MPG;
using namespace MPG::CPRel;
using namespace MPG::CPRel::Prop;

GRelation domRu(void) {
  vector<Tuple> rl;
  rl.reserve(4);
  rl.push_back(Tuple(2,3)); rl.push_back(Tuple(0,0));
  rl.push_back(Tuple(2,1)); rl.push_back(Tuple(1,1));

  return GRelation(CPRel::create(rl));
}

GRelation domQu(void) {
/*  vector<Tuple> rl;
  rl.reserve(4);
  rl.push_back(Tuple(2,3)); rl.push_back(Tuple(0,0));
  rl.push_back(Tuple(2,1)); rl.push_back(Tuple(1,1));
  //rl.push_back(Tuple(6,7));

  return GRelation(CPRel::create(rl));
  */
  return GRelation::create_full(2);
}

class EqualityTest : public Gecode::Space {
protected:
  CPRelVar r,q;
public:
  EqualityTest(void)  {

    GRelation lbr(2);
    r = CPRelVar(*this,lbr,domRu());
    GRelation lbq(2); //lbq.add(Tuple(2,3));
    q = CPRelVar(*this,lbq,domQu());

    //std::cerr << "R:" << r << std::endl;
    //std::cerr << "Q:" << q << std::endl;

    //equal(*this,r,q);
    complement(*this,r,q);
    branch(*this,r);
  }
  void print(std::ostream& os) const {
    os << "<b>Space</b>" << std::endl;
    os << "<b>R</b>: " << r << std::endl
       << "<b>Q</b>: " << q.oob() << std::endl;
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
