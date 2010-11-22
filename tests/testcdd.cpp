#include <iostream>
#include "cdd.hh"

using namespace std;
using namespace Cdd;

int main(void) {
	{
		int t0[] = {1,2};
		int t1[] = {2,3};
		int t2[] = {0,0};
		
		Relation r(2);
		
		BDD lb = Cudd::zero();
		BDD ub = Cudd::zero();
		
		lb |= r.repr(t0);
    
		ub |= r.repr(t0);
		ub |= r.repr(t1);
		ub |= r.repr(t2);

    BDD x = lb && ub;
		
		x.printTuples(2);
		//ub.printTuples(2);
		
		//r.init(lb, ub);
		
		cout << "Minterms in glb " << lb.minterms(2) << " Nodes: " << lb.numNodes() << endl;
		cout << "Minterms in lub " << ub.minterms(2) << " Nodes: " << ub.numNodes() << endl;		
		//cout << "test " << (r.path(2) == one) << endl;
		cout << "References: " << Cudd::references() << endl;
	}
  cout << "This number should be zero: " << Cudd::references() << endl;
  return 0;
}
