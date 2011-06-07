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

    ifstream input("/Users/ggutierrez/Work2/cprel/tests/ground-relations/midi-medium.txt");
    GRelation r = read(input,5);
    cout << r << endl;
    
//    std::for_each(midi_small.begin(),midi_small.end(),
//                  [&](const std::vector<int>& t) {
//                  Tuple v(t);
//                  r.add(v);
//                  });

  //cout << r << endl;
//  PermDescriptor pd;
//  pd.permute(0,4);
//  pd.permute(3,1);
//  RelationImpl s = r.permute(pd);
//  cout << s << endl;
//
//  assert(r.cardinality() == s.cardinality());
  }
  return 0;
}
