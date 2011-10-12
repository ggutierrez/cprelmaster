#include <iostream>
#include <fstream>
#include <bdddomain/domainSpace.hh>

MPG::VarImpl::Bdd timesULeft(MPG::VarImpl::Bdd b, int arity) {
  auto last = arity;
  for (int i = last; i > 0; i--) {
    std::cout << "Swap  " << i << " --- " << i-1 << std::endl;
    
  }
  std::cout << "Finished " <<  std::endl;
}
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

  auto timesULeft1 = w.permute(ds.cube(1),ds.cube(2));
  auto timesULeft2 = timesULeft1.permute(ds.cube(0),ds.cube(1));

  //timesULeft(w,2);

  ds.print(std::cout,w);
  std::cout << "Another " << std::endl;
  ds.print(std::cout,timesULeft1);
  ds.print(std::cout,timesULeft2);

  std::vector<int> tuple = {1,2,3};
  auto mm = ds.represent(begin(tuple), end(tuple));
  ds.print(std::cout,mm);
  //std::cout << std::endl << w.countMinterm(6) << std::endl;

  return 0;
}
