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

using namespace MPG;
using namespace MPG::VarImpl;



RelationImpl test0(void) {
  RelationImpl ub(3);
  ub.add({0,3,9});
//, {1,2,8}, {2,1,7}, {3,0,6}, {4,0,5}, {5,0,4}});
  return ub;
}

int main(void) {

  cout << TupleIO("(",")"," ");

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

