#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tr1/tuple>
#include <tr1/unordered_map>
#include <list>

#include <cdd-obj/cdd.hh>

using namespace std;
using namespace tr1;
using namespace Cdd;

void encode(unordered_map<int,int>& s, int v) {
  s[v] = s.size();
}
/**
  Reads a file containing a binary relation. One pair per line.
 */
BDD read_binrel(const char* fname) {
  unordered_map<int,int> normalize;

  BDD br = Cudd::zero();

  string line;
  ifstream input(fname);
  if (input.is_open()) {
    while (input.good()) {
      getline(input,line);
      stringstream ss(line);
      int first, second;
      ss >> first; ss >> second;

      encode(normalize,first);
      encode(normalize, second);
      br |= BDD::create(make_tuple(first,second));
      //tuples.push_back(make_pair(first,second));
    }
    input.close();
    cout << "Pairs in the relation: " << br.minterms(2) << endl;
    //Cudd::stats();
  } else {
    cerr << "Unable to open file: " << fname << endl;
  }
  return br;
}
Relation xor_test(void) {

  BDD lbr = Cudd::zero();
  BDD ubr = Cudd::zero();

  lbr |=
    BDD::create(make_tuple(5,5)) ||
    BDD::create(make_tuple(5,6)) ||
    BDD::create(make_tuple(1,0));

  ubr |=
    BDD::create(make_tuple(5,5)) ||
    BDD::create(make_tuple(5,6)) ||
    BDD::create(make_tuple(1,0)) ||
    BDD::create(make_tuple(2,3)) ||
    BDD::create(make_tuple(3,1)) ||
    BDD::create(make_tuple(1,1));

  Relation r(lbr, ubr, 2);


  BDD lbs = Cudd::zero();
  BDD ubs = Cudd::zero();

  lbs |=
    BDD::create(make_tuple(2,0)) ||
    BDD::create(make_tuple(3,1));

  ubs |=
    BDD::create(make_tuple(2,0)) ||
    BDD::create(make_tuple(3,1)) ||
    BDD::create(make_tuple(5,5));

  Relation s(lbs, ubs, 2);

  cout << s << endl;
  cout << "r" << endl;
  cout << r <<endl;

  BDD doms = s.dom();
  BDD domr = r.dom();

//  BDD t =
  Relation x(doms ^ domr, 2);
  cout << "The lower bound" << endl;
  x.glb().printTuples(2,cout);
  cout << "the unkown" << endl;
  x.unk().printTuples(2,cout);

}

Relation var_test(void) {


  BDD lb = Cudd::zero();
  BDD ub = Cudd::zero();

  //{(1,2)}..{(1,2),(2,3),(0,0)}
  lb |= BDD::create(make_tuple(1,2));
  ub |= BDD::create(make_tuple(1,2));
  ub |= BDD::create(make_tuple(2,3));
  ub |= BDD::create(make_tuple(0,0));

  // initialize r = [lb,ub]
  Relation r(lb, ub, 2);

  //BDD glb = r.glb();
  //BDD dom = r.dom();
  //dom.dot("dom.dot");
  cout << r << endl;

  // include glb in r

  BDD g = Cudd::zero();
  g |= BDD::create(make_tuple(2,3));

  //r <<= ub;
  //r >>= g;

  cout << "after " << r << endl;

  return r;
}

void rel_test(void) {
  Relation r = xor_test();

}

BDD delta_test() {
  BDD zero = Cudd::zero();
  BDD unk = Cudd::unk();
  BDD one = Cudd::one();

  BDD v0(0,true);
  BDD v1(1,true);
  BDD v2(2,true);
  BDD v3(3,true);

  BDD t = unk;
  t &= v0; t &= v1; t &= v2; t &= v3;

  //t &= unk;
  BDD u = t.delta();
  t.dot("input.dot");
  u.dot("out.dot");
  //BDD v = unk.delta();

  //cout << "Expected ret one " << (u == unk) << endl;
  //cout << "Expected ret one " << (u == one) << endl;
  //cout << "Expected ret unk " << (v == unk) << endl;


  return t;
}

int main(void) {
  cout << "Start of the test" << endl;
  BDD deps = read_binrel("deps-guake.txt");
  BDD confs = read_binrel("conf-guake.txt");
  BDD provs = read_binrel("prov-guake.txt");

  Relation Dep(deps,Cudd::one(),2);
  Relation Conf(confs,Cudd::one(),2);
  Relation Prov(Cudd::zero(),provs,2);

  //cout << "elements in all " << all.size() << endl;
  //cout << "This number should be zero: " << Cudd::references() << endl;
  return 0;
}
