#include <iostream>
#include <vector>
#include <cprel/tuple.hh>
#include <bdddomain/rel-impl.hh>

using std::cout;
using std::endl;

int main(void) {
  using namespace MPG::CPRel;
  using namespace MPG::CPRel::VarImpl;
  cout << "Tests starts" << endl;
  {
    RelationImpl s(2);
    //RelationImpl t(s);
    s.add(Tuple(0,1));
    s.add(Tuple(3,2));

    cout << "S: " << s << endl;
    RelationImpl nr(s.swap_columns(0,1));
    cout << "NR: " << nr << endl;

//    cout << "S': " << s << endl;
//    cout << "NR: " << nr << endl;
  }
  return 0;
}

