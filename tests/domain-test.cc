#include <iostream>
#include <vector>
#include <bdddomain/tuple.hh>
#include <bdddomain/rel-impl.hh>

//#include <bdddomain/relations.hh>

int main(void) {

//  cout << TupleIO("(",")"," ");

  std::cout << "Starting the test " << std::endl;
  {
    
    MPG::VarImpl::RelationImpl unary(1), binary(2), ternary(3), fourthary(5), f(5);
    
    // fill some elements in the binary relation
    binary.add({3,1});
    binary.add({2,3});
    binary.add({3,1});
    binary.add({3,2});
    binary.add({5,4});
    binary.add({1,0});
    binary.add({0,1});
    std::cout << "Binary relation: (" << binary.cardinality() << "): " << binary << std::endl;
    //auto uq = binary.unique(0);
    //std::cout << "Unique quantification yields: " << uq << std::endl;
    
    ternary.add({0,0,1});
    ternary.add({0,1,1});
    ternary.add({0,0,2});
    ternary.add({1,2,3});
    std::cout << "Ternary relation: (" << ternary.cardinality() << "): " << ternary << std::endl;
    //auto tuq = ternary.unique({0,1});
    //std::cout << "Unique quantification yields: " << tuq << std::endl;
    //auto teq = ternary.exists({0,1});
    //std::cout << "Unique quantification yields: " << teq << std::endl;
    
    //MPG::VarImpl::RelationImpl j1 = binary.join(1, ternary);
    //std::cout << "Join on one column (" << j1.cardinality() << "): " << j1 << std::endl;
   
    MPG::VarImpl::RelationImpl discard  = ternary.discard(1, 2);
    std::cout << "Discard one column of ternarry  (" << discard.cardinality() << ")#(" << discard.arity() << "): " << discard << std::endl;
    
    /*
    std::cout << "Ternary replace: " 
              << ternary.replace({{1,0}})
              << std::endl;

    std::cout << "Ternary swap: " 
              << ternary.swap({{0,1}})
              << std::endl;
    */
    /*
    fourthary = ternary.timesURight(2);
    std::cout << "U times Ternary: " << (fourthary.cardinality()) << fourthary << std::endl;
    fourthary.add({1,2,3,4,5});
    f.add({1,2,3,4,5});
    fourthary.intersect(f);
    
    std::cout << "After adding: " << (fourthary.cardinality()) 
	      << fourthary << std::endl;
    */
  }
  return 0;
}

