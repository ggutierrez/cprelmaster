#include <gecode/search.hh>
#include <cprel/cprel.hh>

/*
 * The Golomb ruler script
 *
 */

using namespace Gecode;

using namespace MPG;
using namespace CPRel;

class GolombRuler : public Gecode::Space {
protected:
  static const int n = 2;
  CPRelVarArray m;
public:
  GolombRuler(void)
    : m(*this,n,0,5) {

    branch(*this, m);
  }
  virtual void constrain(const Gecode::Space&) {

  }
  void print(void) const {
    std::cout << "\tm[" << m.size() << "] = " << m << std::endl;
  }
  GolombRuler(bool share, GolombRuler& s)
    : Gecode::Space(share,s) {
    m.update(*this, share, s.m);
  }
  virtual Space* copy(bool share) {
    return new GolombRuler(share,*this);
  }
};

int main(int, char**) {
  GolombRuler* g = new GolombRuler();
  Gecode::BAB<GolombRuler> e(g);
  delete g;

  while (Gecode::Space* s = e.next()) {
    static_cast<GolombRuler*>(s)->print();
    delete s;
  }
  return 0;
}
