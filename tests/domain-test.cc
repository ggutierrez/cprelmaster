#include <iostream>
#include <vector>
#include <cprel/tuple.hh>
#include <bdddomain/rel-impl.hh>

using std::cout;
using std::endl;
//using std::ostream;
//using std::vector;

//ostream& operator<<(ostream& os, const vector<int>& tuple) {
//  os << "<";
//  for (unsigned int i = 0; i < tuple.size(); i++) {
//    os << tuple[i];
//    if (i != tuple.size()-1)
//      os << ",";
//  }
//  os << ">";
//  return os;
//}

int main(void) {
  using namespace CPRel;
  using namespace CPRel::BDDImpl;

  {

    Tuple a(2,3);
    Tuple b(0,0);
    RelationImpl r(2);
    r.add(a);
    r.add(b);

    Tuple c(5,4);
    Tuple d(0,0);
    RelationImpl s(2);
    s.add(c);
    s.add(d);

    cout << "Cardinality r" << r.cardinality() << endl;
    cout << "Cardinality s " << s.cardinality() << endl;
    //s.remove(r);
    RelationImpl t = Union(r,s);
    t.complement();
    cout << "Cardinality t " << t.cardinality() << endl;
    //r.remove(a);
    //cout << "Cardinality " << r.cardinality() << endl;

//    DdNode *f = one();
    //DdNode * x = encode(3,0);
    //DdNode * y = encode(2,1);
//    DdNode *xy = encode(a);
//    Cudd_RecursiveDeref(dd(),xy);


    //  BddManager::instance();
  }
//  CPRel::BDDImpl::BDDConfig::manager();
//  CPRel::BDDImpl::BDDConfig::manager();
  /*
    vector<pair<int,int> > data(4);
    data.push_back(make_pair(0,0));
    data.push_back(make_pair(0,1));
    data.push_back(make_pair(1,0));
    data.push_back(make_pair(1,1));

    GRelation rr(2,data);
    rr.print(cerr);

    BddIter it(rr.tuples());
    for (; it(); )
      cout << it.val() << endl;
    */
    /*
    BddIter it2(rr.tuples());


    it.val();
    it.val();
    it.val();
    cout << "Valid? " << it() << endl;
    it2.val();
    cout << "Valid? " << it2() << endl;
    */
//}


  return 0;
}
