#include <iostream>
#include <vector>
#include <bdddomain/relation.hh>

using std::ostream;
using std::vector;

ostream& operator<<(ostream& os, const vector<int>& tuple) {
  os << "<";
  for (unsigned int i = 0; i < tuple.size(); i++) {
    os << tuple[i];
    if (i != tuple.size()-1)
      os << ",";
  }
  os << ">";
  return os;
}

int main(void) {
  using namespace std;
  using namespace BDDImpl;
  {
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
    /*
    BddIter it2(rr.tuples());


    it.val();
    it.val();
    it.val();
    cout << "Valid? " << it() << endl;
    it2.val();
    cout << "Valid? " << it2() << endl;
    */
  }
  cout << "References before exit: " << BDDConfig::references() << endl;

  return 0;
}
