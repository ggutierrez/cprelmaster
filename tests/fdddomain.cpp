#include <iostream>
#include <fstream>
#include <bdddomain/domainSpace.hh>

int main(void) {
  //std::cout << "This is a main program " << std::endl;
  
  // declare a domain space for a maximum of five columns, and a maximum element 4
  MPG::VarImpl::DomainSpace ds(5,4);

  MPG::VarImpl::Bdd
    x(ds.represent({0}));
  x.orWith(ds.represent({1}));
  //x.orWith(ds.represent({3}));

  MPG::VarImpl::Bdd
    w(ds.represent({0,0}));
  w.orWith(ds.represent({1,1}));
  w.orWith(ds.represent({3,3}));

  MPG::VarImpl::Bdd
    full(ds.one());

  ds.print(std::cout,full);
  std::cout << std::endl << w.countMinterm(6) << std::endl;

  return 0;
}
