#include <iostream>
#include <vector>
#include <bdddomain/relation.hh>

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
    BddIter it(rr.tuples());
    BddIter it2(rr.tuples());

    it.val();
    it.val();
    it.val();
    it.val();
    cout << "Valid? " << it() << endl;
    it2.val();
    cout << "Valid? " << it2() << endl;
  }
  cout << "References before exit: " << BDDConfig::references() << endl;
  //Cudd_Quit(BDDConfig::dd);
  return 0;
}
