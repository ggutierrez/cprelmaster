#include <iostream>
#include <fstream>
#include <bdddomain/domainSpace.hh>

int main(void) {
  //std::cout << "This is a main program " << std::endl;
  
  MPG::VarImpl::DomainSpace ds(2,3);
  MPG::VarImpl::Bdd x(ds.represent({0,0}));
  MPG::VarImpl::Bdd z(ds.represent({1}));
  MPG::VarImpl::Bdd y(ds.represent({3,0}));

  
  
  x.orWith(y);
  //x.orWith(z);
  std::cout << "Equal to zero? " << x.isZero() << std::endl;
  //x.printDot(std::cout);
  ds.print(std::cout,x);
  ds.print(std::cout,z,1);
  
  return 0;
}
