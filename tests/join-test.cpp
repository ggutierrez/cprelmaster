#include <iostream>
#include <vector>
#include <bdddomain/tuple.hh>
#include <bdddomain/rel-impl.hh>
#include <cprel/grelation.hh>
#include <fstream>
//#include <tests/ground-relations/midi-small.hh>

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::make_pair;
using std::ifstream;

using namespace MPG::CPRel;
using namespace MPG::CPRel::VarImpl;

int main(void) {
  cout << "Tests starts" << endl;
  {
/*
    ifstream input("/home/gg/Work/cprel/tests/ground-relations/midi-medium.txt");
    GRelation r = read(input,5);
    PermDescriptor pd;
    pd.permute(0,1);
    GRelation s = r.permute(pd);
    cout << s << endl;
    cout << r << endl;
    cout << "Cardinality of the created relation: " << r.cardinality() << endl;
*/

    ifstream inputL("/home/gg/Work/cprelmaster/tests/ground-relations/r3.txt");
    GRelation w = read(inputL,3);
    cout << "Left relation: " << w << endl << endl;

//    ifstream inputR("/home/gg/Work/cprelmaster/tests/ground-relations/s2.txt");
//    GRelation x = read(inputR,2);
//    cout << "Right relation: " << x << endl << endl;

//    GRelation j = w.join(1,x);
//    cout << "Join relation: " << j << endl << endl;

    /*
    GRelation k = j.shiftRight(1);
    cout << "Ca: " << k << endl << endl;


    */
    GRelation l = w.project(2);
    cout << "Cb: " << l << " with cardinality " << l.cardinality() << endl << endl;
  }
  return 0;
}
