#include <iostream>
#include <vector>
#include <bdddomain/tuple.hh>
#include <bdddomain/rel-impl.hh>
#include <rel/grelation.hh>
#include <fstream>
//#include <tests/ground-relations/midi-small.hh>

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::make_pair;
using std::ifstream;

using namespace MPG;
using namespace MPG::VarImpl;

int main(void) {
  cout << "Tests starts" << endl;
  {

    ifstream input("/home/gg/Work/cprel/tests/ground-relations/midi-medium.txt");
    GRelation r = read(input,5);
    PermDescriptor pd;
    pd.permute(0,1);
    GRelation s = r.permute(pd);
    cout << s << endl;
    cout << r << endl;
    cout << "Cardinality of the created relation: " << r.cardinality() << endl;


    ifstream input2("/home/gg/Work/cprel/tests/ground-relations/small-rel.txt");
    GRelation w = read(input2,2);
    GRelation z = w.permute(pd);
    cout << w << endl;
    cout << z << endl;
  }
  return 0;
}
