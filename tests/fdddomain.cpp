#include <iostream>
#include <fstream>
#include <bdddomain/domainSpace.hh>

int main(void) {
  //std::cout << "This is a main program " << std::endl;
  
  MPG::VarImpl::DomainSpace ds(3,3);
  MPG::VarImpl::Bdd x(ds.represent({0,0,1}));
  MPG::VarImpl::Bdd z(ds.represent({1,0,3}));
  MPG::VarImpl::Bdd y(ds.represent({3,0,1}));

  
  
  x.orWith(y);
  x.orWith(z);

  MPG::VarImpl::Bdd e = x.exists(ds.cube(0,1));
  //x.orWith(z);
  //std::cout << "Equal to zero? " << x.isZero() << std::endl;
  //x.printDot(std::cout);
  //ds.print(std::cout,x);
  //std::cout << "And now the quantification " << std::endl;
  //ds.print(std::cout, e);
  ds.print(std::cout,z,1);
  //ds.permute(0,2);
  return 0;
}
