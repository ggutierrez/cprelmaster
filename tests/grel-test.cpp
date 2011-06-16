#include <iostream>
#include <fstream>
#include <cprel/grelation.hh>
#include <bdddomain/tuple.hh>


using std::cout;
using std::endl;
using std::vector;

int main(void) {
  using namespace MPG::CPRel;

  std::ifstream input2("/home/gg/Work/cprel/tests/ground-relations/small-rel.txt");
  GRelation w = read(input2,3);
  cout << "The relation: " << w << endl;

//  PermDescriptor d;
//  d.permute(0,1);

  GRelation x = w.unique(0);//.project(1);
  cout << x << endl;


  return 0;
}
