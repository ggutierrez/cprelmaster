#include <cprel/grelation.hh>
#include <iostream>
#include <fstream>

int main(void) {
  using namespace std;
  using namespace MPG::CPRel;

  cout << "Midi test: 4 seasons" << endl;
  ifstream ons("/home/gg/Work/cprel-music/d4saison/d4saisonON.csv");
  GRelation on = read(ons,5);

  ifstream offs("/home/gg/Work/cprel-music/d4saison/d4saisonOFF.csv");
  GRelation off = read(offs,5);

  ifstream tempos("/home/gg/Work/cprel-music/d4saison/d4saisonTEMPO.csv");
  GRelation tempo = read(tempos,3);

  cerr << "Finished:" << endl
       << "\t|On|: " << on.cardinality() << endl
       << "\t|Off|: " << off.cardinality() << endl
       << "\t|Tempo|: " << tempo.cardinality() << endl;

  return 0;
}
