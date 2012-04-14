#include <gecode/search.hh>
#include <gecode/gist.hh>

#include <cprel/cprel.hh>

using namespace Gecode;
using namespace Gecode::Int;
using namespace MPG;
using namespace MPG::CPRel;

class FunctionalTest : public Gecode::Space {
protected:
  CPRelVar r;
public:
  FunctionalTest(void)  {
    GRelation lb(2);// lb.add(make_Tuple(0,0));
    GRelation ub(2);
    ub.add({{1,3},  {1, 5}, {4,5}, {8,2}, {6,5}});
    //lb.add({{1,3}, {4,5}, {8,2}, {6,5}});

    r = CPRelVar(*this,lb,ub);
    function(*this,r);
    //    std::cerr << "Relation: " << r << std::endl;
    branch(*this,r);
  }
  virtual void constrain(const Gecode::Space&) {

  }
  void print(std::ostream& os) const {
    os << std::endl << std::endl 
       << "Relation: " << std::endl
       << r << std::endl;
  }
  FunctionalTest(bool share, FunctionalTest& s)
    : Gecode::Space(share,s) {
    r.update(*this, share, s.r);
  }
  virtual Space* copy(bool share) {
    return new FunctionalTest(share,*this);
  }
};

int main(int, char**) {
  FunctionalTest* g = new FunctionalTest();

  Gecode::BAB<FunctionalTest> e(g);
  delete g;
  std::cout << "Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<FunctionalTest*>(s)->print(std::cout);
    delete s;
  }

  /*
  Gist::Print<MinimalTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;
  */
  std::cout << "Finishing " << std::endl;
  std::cout << "Foooo " << 332 << std::endl;
  return 0;
}

