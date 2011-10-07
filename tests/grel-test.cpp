#include <iostream>
#include <fstream>
#include <rel/grelation.hh>
#include <bdddomain/tuple.hh>


using std::cout;
using std::endl;
using std::vector;

int main(void) {
  using namespace MPG;

  GRelation x(2);
  
  x.add(make_Tuple(9,15));
  x.add(make_Tuple(15,15));
  x.add(make_Tuple(10,15));
  
  /*
  x.add(make_Tuple(0,3));
  x.add(make_Tuple(3,3));
  x.add(make_Tuple(2,3));
  */

  GRelation unique = x.unique(1);
  std::cout << "Returned relation " << unique << std::endl;

  return 0;
}
