#include <iostream>
#include <fstream>
#include <bdddomain/domainSpace.hh>

int main(void) {
  //std::cout << "This is a main program " << std::endl;
  
  // declare a domain space for a maximum of five columns, and a maximum element 4
  MPG::VarImpl::DomainSpace ds(5,4);

  MPG::VarImpl::Bdd x(ds.represent({0,0,1}));
  x.orWith(ds.represent({1,0,3}));
  x.orWith(ds.represent({3,0,1}));


  ds.print(std::cout,x);
  auto swap = x.permute(ds.cube(2), ds.cube(1));
  //std::cout << "And now the permutation of columns 2,1 " << std::endl;
  //ds.print(std::cout, swap);

  auto a = ds.represent({2,1});
  a.orWith(ds.represent({1,0}));
  a.orWith(ds.represent({0,3}));
  a.orWith(ds.represent({0,1}));
  //std::cout << "Another relation " <<  std::endl;
  //ds.print(std::cout,a,2);

  //std::cout << "Swaping 0 and 1 " << std::endl;
  auto swap2 = a.permute(ds.cube(0),ds.cube(1));
  //ds.print(std::cout, swap2);

  //std::cout << "Is the other still ok? " <<  std::endl;
  //ds.print(std::cout, swap);

  //std::cout << "and now x " <<  std::endl;
  //ds.print(std::cout, x);

  return 0;
}
