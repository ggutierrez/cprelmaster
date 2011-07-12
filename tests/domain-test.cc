#include <iostream>
#include <vector>
#include <bdddomain/tuple.hh>
#include <bdddomain/rel-impl.hh>

#include <bdddomain/relations.hh>

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::make_pair;

using namespace MPG::CPRel;
using namespace MPG::CPRel::VarImpl;

RelationImpl test0(void) {
  RelationImpl ub(3);
  ub.add(make_Tuple(0,3,9));
  ub.add(make_Tuple(1,2,8));
  ub.add(make_Tuple(2,1,7));
  ub.add(make_Tuple(3,0,6));
  ub.add(make_Tuple(4,0,5));
  ub.add(make_Tuple(5,0,4));
  return ub;
}

int main(void) {
  cout << "Tests starts" << endl;
  {
    RelationImpl r = test0();

    cout << r << endl;

    RelationImpl s = r.shiftRight(1);
    cout << s << endl;
    cout << "Shift right " <<  " Cardinality " << s.cardinality() << std::endl;

    // initial relation
    cout << "Original relation " << r << " with cardinality " << r.cardinality() << std::endl;

  }
  return 0;
}

