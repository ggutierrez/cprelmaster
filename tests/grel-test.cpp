#include <iostream>
#include <fstream>
#include <rel/grelation.hh>
#include <bdddomain/tuple.hh>


using std::cout;
using std::endl;
using std::vector;

int main(void) {
  using namespace MPG;

  GRelation a(3);
  a.add({{0,1,0},{1,2,1},{2,2,2},{3,2,3},
         {0,1,4},{1,2,5},{2,2,6},{3,2,7},
         {0,1,8},{1,2,9},{2,2,10},{3,2,11},
         {0,1,12},{1,2,13},{2,2,14},{3,2,15}});
  cout << "A relation: " << a << endl;

  GRelation b(3);
  b.add({{2,2,9},{2,3,8},{2,2,7}});
  cout << "B relation: " << b << endl;

  GRelation fall_a = a.forall(0);
  cout << "Result relation: " << fall_a << endl;

//  GRelation exp(2);
//  exp.add({{1,9},{1,7},{2,9},{2,7},{3,9},{3,7}});
//  cout << "Expected relation: " << exp << endl;

//  GRelation follow = a.follow(2,b);
//  cout << "Follow relation: " << follow << endl;

//  assert(follow.eq(exp) && "Unexpected returned relation");

  return 0;
}
