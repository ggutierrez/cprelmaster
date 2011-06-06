#include <iostream>
#include <vector>
#include <bdddomain/tuple.hh>
#include <bdddomain/rel-impl.hh>


using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::make_pair;

using namespace MPG::CPRel;
using namespace MPG::CPRel::VarImpl;

RelationImpl test0(void) {
  RelationImpl ub(2);
  ub.add(Tuple({0,0}));
//  ub.add(make_Tuple(1,0));
//  ub.add(make_Tuple(2,0));
//  ub.add(make_Tuple(3,0));
//  ub.add(make_Tuple(4,0));
//  ub.add(make_Tuple(5,0));
  return ub;
}


int main(void) {
  cout << "Tests starts" << endl;
  {
    RelationImpl r = test0();
    cout << r << endl;

  }
  return 0;
}

