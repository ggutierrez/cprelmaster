#include <iostream>
#include <tr1/tuple>
#include <list>

#include "cdd.hh"

using namespace std;
using namespace tr1;
using namespace Cdd;

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
  
  BDD x = doms ^ domr;
  cout << "The lower bound" << endl;
  x.status(Cudd::one()).printTuples(2,cout);
  cout << "the unkown" << endl;
  x.status(Cudd::unk()).printTuples(2,cout);
  
  
  
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
  /*
   Relation r = var_test();
   BDD glb = r.glb();
   BDD unk = r.unk();
   */
  //BDD r = delta_test();
  rel_test();
  //unk.printTuples(2);
  
  //cout << "Minterms in glb " << lb.minterms(2) << " Nodes: " << lb.numNodes() << endl;
  //cout << "Minterms in lub " << ub.minterms(2) << " Nodes: " << ub.numNodes() << endl;		
  //cout << "test " << (r.path(2) == one) << endl;
	
  cout << "This number should be zero: " << Cudd::references() << endl;
  return 0;
}
