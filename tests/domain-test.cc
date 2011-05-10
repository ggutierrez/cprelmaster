#include <iostream>
#include <vector>
#include <cprel/tuple.hh>
#include <bdddomain/rel-impl.hh>

using std::cout;
using std::endl;

int main(void) {
  using namespace MPG::CPRel;
  using namespace MPG::CPRel::VarImpl;

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

    assert(subset(r,full));

    cout << "Relation s " << s << endl;
    cout << "Relation empty " << empty << endl;
/*
    RelationImpl inter(full);
    for (RelationImplIter it = inter.tuples(); it();) {
      cout << "Tuple " << it.val() << endl;
    }
*/
  }

  return 0;
}
