#include <iostream>
#include <vector>
#include <cprel/tuple.hh>
#include <bdddomain/rel-impl.hh>

using std::cout;
using std::endl;

int main(void) {
  using namespace CPRel;
  using namespace CPRel::BDDImpl;

  {

    RelationImpl r(2);
    RelationImpl empty(2);

    assert(r == empty);

    RelationImpl x = intersect(empty,r);
    assert(x==empty);
    assert(x.cardinality()==0);

    RelationImpl full = RelationImpl::create_full(2);
    assert(complement(full)==empty);

    r.add(Tuple(2,3));
    assert(r.cardinality()==1);

    assert(subset(empty,r));
    assert(!subset(r,empty));

    RelationImpl s(2);
    s.add(Tuple(0,0));
    assert(intersect(r,s)==empty);
    s.add(r);
    assert(s.arity()==2);
    assert(intersect(s,r).cardinality()==1);
    assert(intersect(s,r)==r);
    assert(subset(r,s));
    assert(r!=s);
/*
    RelationImpl inter(intersect(s,r));
    for (RelationImplIter it = inter.tuples(); it();) {
      cout << "Tuple " << it.val() << endl;
    }
*/

    //    cout << " Card inter " << x.cardinality() << endl;

//    cout << "Intersect card " << intersect(s,r).cardinality() << endl;
//
//    cout << "Cardinality of full " << full.cardinality() << endl;
    //cout << "Subset empty of r? " << subset(empty,r) << endl;
//    cout << "Subset r of empty? " << subset(r,empty) << endl;

    //cout << "Is really empty? " << empty.empty() << endl;
//    RelationImplIter it(empty.tuples());
//    cout << "Valid? " << it() << endl;

//    RelationImplIter it2(r.tuples());
//    cout << "Valid 2 ? " << it2() << endl;
//    cout << "it2 " << it2.val() << endl;
//    cout << "Valid 2 ? " << it2() << endl;
//    cout << "it2 " << it2.val() << endl;
//    cout << "Valid 2 ? " << it2() << endl;
    //cout << a << endl;

  }

  return 0;
}
