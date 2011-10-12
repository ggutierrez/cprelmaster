#include <iostream>
#include <vector>
#include <cassert>
#include <bdddomain/tuple.hh>
#include <bdddomain/rel-impl.hh>

using namespace std;
using namespace MPG;
using namespace MPG::VarImpl;

int main(void) {
  
  Tuple x({1,2,3});
  Tuple y({3,2,1});
  assert(x != y);
  Tuple z({1,2,3});
  assert(x == z);

  Tuple m = make_Tuple(1);
  Tuple q({1});
  assert(m == q);
  //  cout << TupleIO("(",")"," ");
  //std::cout << "Tuple x " << x << std::endl;
  //std::cout << "Tuple y " << y << std::endl;
  RelationImpl r(2);
  Tuple t0({1,2});
  r.add(t0);
  std::cout << "Relation " << r << std::endl;
  
  return 0;
}

