#include <iostream>
#include <set>
#include <rel/grelation.hh>

using namespace std;
using namespace MPG;


int main(void) {
  cout << "something" << endl;

  GRelation a(3);
  a.add({{0,1,0},{1,2,1},{2,2,2},{3,2,3},
         {0,1,4},{1,2,5},{2,2,6},{3,2,7},
         {0,1,8},{1,2,9},{2,2,10},{3,2,11},
         {0,1,12},{1,2,13},{2,2,14},{3,2,15}});
  cout << "A relation: " << a << endl;

  GRelation b(3);
  b.add({{2,2,9},{2,3,8},{2,4,7}});
  cout << "B relation: " << b << endl;


  //  GRelation uq = a.unique(0).intersect(a);
  GRelation x = unique(a,{0});
  cout << "Unique a(0)" << x << endl;

  GRelation y = unique(b,{2,1});
  cout << "Unique b(1)" << y << endl;
  return 0;
}
  