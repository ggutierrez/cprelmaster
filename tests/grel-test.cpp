#include <iostream>
#include <fstream>
#include <cprel/grelation.hh>
#include <bdddomain/tuple.hh>


using std::cout;
using std::endl;
using std::vector;

int main(void) {
  using namespace MPG::CPRel;

  std::ifstream input2("/Users/gg/Work/cprelmaster/tests/ground-relations/small-rel.txt");
  GRelation w = read(input2,3);
  cout << "The relation: " << w << endl;

  std::ifstream input3("/Users/gg/Work/cprelmaster/tests/ground-relations/small-rel2.txt");
  GRelation y = read(input3,2);
  GRelation z = y.timesU(1, true);
  //cout << "The other relation: " << z << endl;

  std::vector<int> q(1,0);
  q[0] = 2;
 
  GRelation x = w.unique(q).intersect(w);
  //cout << "Relation " << endl << x << endl;

  GRelation to_include = x.intersect(z);
  cout << "To include Relation " << endl << to_include << endl;
  
  return 0;
}
