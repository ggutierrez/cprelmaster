#include <iostream>
#include <vector>
#include <rel/grelation.hh>

using namespace std;
using namespace MPG;

int main(void) {
  cout << "permutation test" << endl;
  GRelation r(4);
  r.add(Tuple({1,2,3,4}));

  std::vector<pair<int,int> > perm;
  perm.push_back({0,1});
  perm.push_back({1,2});

  std::vector<pair<int,int> > perm_inv;
  perm_inv.push_back({2,1});
  perm_inv.push_back({1,0});
  // perm_inv.push_back({2,1});

  GRelation p = r.permute(perm);
  //p = r.permute(perm);

  cout << "Initial relation: " << endl
       << r << endl;
  cout << "Permutation : " << endl
       << p << endl;

  cout << "Back again:" << endl
       << p.permute(perm_inv) << endl;

  GRelation s(2);
  s.add(Tuple({1,2}));
  
  cout << "Other relation " << s << endl;
  std::vector<std::pair<int,int>> perm2;
  perm2.push_back(std::make_pair(0,1));
  perm2.push_back(std::make_pair(1,0));
  cout << "permuted " << s.permute(perm2) << endl;
  return 0;
}
