#include <iostream>
#include <vector>
#include <cprel/tuple.hh>
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
  RelationImpl ub(2);
  ub.add(Tuple(0,0));
  ub.add(Tuple(1,0));
  ub.add(Tuple(2,0));
  ub.add(Tuple(3,0));
  ub.add(Tuple(4,0));
  ub.add(Tuple(5,0));
  return ub;
}


int main(void) {
  cout << "Tests starts" << endl;
  {
    PermDescriptor desc;
    desc.permute(0,1);

    RelationImpl r = test0();

    cout << "The relation: " << r << endl;

    RelationImpl p = r.permute(desc);

    cout << "The permutation: " << p << endl;

    //DdNode *eq = create_greaterXY();
    //Cudd_Ref(eq);
    //Cudd_RecursiveDeref(dd(),eq);

//    RelationImpl s(2);
//    //RelationImpl t(s);
//    s.add(Tuple(0,1));
//    s.add(Tuple(3,2));

//    cout << "S: " << s << endl;
//    RelationImpl nr(s.swap_columns(0,1));
//    cout << "NR: " << nr << endl;

//    cout << "S': " << s << endl;
//    cout << "NR: " << nr << endl;
  }
  return 0;
}

