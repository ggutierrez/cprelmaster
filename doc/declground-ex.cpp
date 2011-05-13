#include <vector>
#include <cprel/tuple.hh>
#include <cprel/grelation.hh>

void grelDecl(void) {
  using namespace MPG::CPRel;
  using namespace std;

  vector<Tuple> x0;
  x0.reserve(3);
  x0.push_back(Tuple(1,2,3));
  x0.push_back(Tuple(3,0,2));
  x0.push_back(Tuple(99,30,42));

  GRelation r(create(x0));

  cout << "R: " << r << endl;
}
