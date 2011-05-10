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
  GRelation empty(2);
  GRelation r(create(x0));
  GRelation s(create(y0));

/*
  cout << "Relation r: " << r << endl;
  GRelation z(r);
  cout << "Relation z: " << z << endl;
  Tuple t(5,11);
  z.add(t);

  cout << "Relation r: " << r << endl;
  cout << "Relation z: " << z << endl;
*/
//  GRelationIter it(r);
//  assert(it());
//  cout << it.val() << endl;
//  ++it;
//  cout << it.val() << endl;
//  ++it;
//  cout << it.val() << endl;
//  ++it;
//  cout << it.val() << endl;
//  ++it;
//  assert(!it());
//  cout << endl;
//  GRelationIter it2(s);
//  for (; it2(); ++it2)
//    cout << it2.val() << endl;

//  GRelationIter it3(empty);
//  assert(!it3());

//  it3 = GRelationIter(r);
//  assert(it3());
//  for (; it3(); ++it3)
//    cout << it3.val() << endl;

  return 0;
}
