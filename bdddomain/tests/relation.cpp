#include <iostream>
#include <vector>
#include <bdddomain/grelation.hh>

using namespace std;
using namespace MPG;

int main(void) {
  cout << "Testing relation implementation" << endl;
  Tuple a(1,2,3,4);
  MPG::GRelation r(4);
  r.add(a);

  vector<Tuple> v = {{5,6,7,8},{11,22,33,44}};
  r.add(v);

  cout << r << endl;
  return 0;
}