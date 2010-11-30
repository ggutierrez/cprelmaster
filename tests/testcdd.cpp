#include <iostream>
#include "cdd.hh"

using namespace std;
using namespace Cdd;

Relation var_test(void) {
  int t0[] = {1,2};
  int t1[] = {2,3};
  int t2[] = {0,0};
  
  Relation r(2);
  
  BDD lb = Cudd::zero();
  BDD ub = Cudd::zero();
  
  //{(1,2)}..{(1,2),(2,3),(0,0)}
  lb |= r.repr(t0);
  //lb |= r.repr(t1);
  //lb |= r.repr(t2);
  
  ub |= r.repr(t0);
  ub |= r.repr(t1);
  ub |= r.repr(t2);
  
  // initialize r = [lb,ub]
  r.init(lb, ub);
  
  //BDD glb = r.glb();

  
  //BDD dom = r.dom();
/*
  dom.dot("dom.dot");
  cout << "Tuples in the lower bound: " << endl;
  dom.status(Cudd::one()).printTuples(2);
  cout << "Tuples in the unknown : " << endl;
  dom.status(Cudd::unk()).printTuples(2);
  */
  

  //dom.delta().printTuples(2);
  
  
  BDD ground = dom.delta();
  if (ground == Cudd::one()) {
    cout << "Variable is ground" << endl;
  } else {
    ground.status(Cudd::unk()).dot("ground-st-unk.dot");
    cout << "Tuples in the ground-st-unk: " << endl;
    ground.status(Cudd::unk()).printTuples(2,cout);
    //ground.dot("ground.dot");
  }
  
  
  //ground.printTuples(2);
  //cout << "should be one " << (ground == Cudd::one()) << endl;
  return r;
}

void rel_test(void) {
  Relation r = var_test();
  
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
