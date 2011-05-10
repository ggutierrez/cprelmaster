#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>
#include <vector>

using namespace Gecode;

using namespace MPG;
using namespace MPG::CPRel;

class GolombRuler : public Gecode::Space {
protected:
  CPRelVar r;
public:
  GolombRuler(void)  {
    vector<Tuple> rl;
    rl.reserve(4);
    rl.push_back(Tuple(2,3)); rl.push_back(Tuple(0,0));
    rl.push_back(Tuple(2,1)); rl.push_back(Tuple(1,1));

    CPRel::GRelation lb(2);// lb.add(Tuple(0,0));
    CPRel::GRelation ub(CPRel::create(rl));

    r = CPRelVar(*this,lb,ub);
    branch(*this,r);
  }
  virtual void constrain(const Gecode::Space&) {

  }
  void print(std::ostream& os) const {
    os << "\tsol " << r << std::endl;
  }
  GolombRuler(bool share, GolombRuler& s)
    : Gecode::Space(share,s) {
    r.update(*this, share, s.r);
  }
  virtual Space* copy(bool share) {
    return new GolombRuler(share,*this);
  }
};

int main(int, char**) {
  GolombRuler* g = new GolombRuler();

  Gecode::BAB<GolombRuler> e(g);
  delete g;
  std::cout << "Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<GolombRuler*>(s)->print(std::cout);
    delete s;
  }

  /*
  Gist::Print<GolombRuler> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;
*/
  return 0;
}
