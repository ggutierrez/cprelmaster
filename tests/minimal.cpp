#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>
#include <cptuple/cptuple.hh>
#include <vector>

using namespace Gecode;

using std::vector;
using namespace MPG;
using namespace MPG::CPRel;
using namespace MPG::CPTuple;

class MinimalTest : public Gecode::Space {
protected:
  CPRelVar r;
  CPTupleVar t;
public:
  MinimalTest(void)  {
    CPRel::GRelation lb(2);// lb.add(make_Tuple(0,0));
    GRelation ub(2);
    ub.add({{1,3}, {4,5}, {8,2}, {6,5}});

    r = CPRelVar(*this,lb,ub);
    t = CPTupleVar(*this,ub);
//    std::cerr << "Relation: " << r << std::endl;
    std::cerr << "Tuple: " << t << std::endl;
    //branch(*this,r);
    branch(*this,t);
  }
  virtual void constrain(const Gecode::Space&) {

  }
  void print(std::ostream& os) const {
    os << "\trel " << r << std::endl;
    os << "\ttuple " << t << std::endl;
  }
  MinimalTest(bool share, MinimalTest& s)
    : Gecode::Space(share,s) {
    r.update(*this, share, s.r);
    t.update(*this, share, s.t);
  }
  virtual Space* copy(bool share) {
    return new MinimalTest(share,*this);
  }
};

int main(int, char**) {
  MinimalTest* g = new MinimalTest();
/*
  Gecode::BAB<MinimalTest> e(g);
  delete g;
  std::cout << "Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<MinimalTest*>(s)->print(std::cout);
    delete s;
  }
*/

  Gist::Print<MinimalTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;

  return 0;
}
