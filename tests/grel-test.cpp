#include <iostream>
#include <vector>
#include <cprel/grelation.hh>
#include <cprel/tuple.hh>

using std::cout;
using std::endl;
using std::vector;

int main(void) {
  using namespace MPG::CPRel;
  vector<Tuple> x0;
  x0.reserve(4);
  x0.push_back(Tuple(2,3));
  x0.push_back(Tuple(0,0));
  x0.push_back(Tuple(2,1));
  x0.push_back(Tuple(1,1));

  vector<Tuple> y0;
  y0.reserve(4);
  y0.push_back(Tuple(5,3));
  y0.push_back(Tuple(2,0));
  y0.push_back(Tuple(2,7));
  y0.push_back(Tuple(1,1));

//  using namespace CPRel::BDDImpl;
  GRelation r(create(x0));
  GRelation s(create(y0));

  cout << r.differenceAssign(s) << endl;
  cout << "new card " << r.cardinality() << endl;


  return 0;
}
