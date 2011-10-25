#include <iostream>
#include <vector>
#include <bdddomain/tuple.hh>
#include <bdddomain/rel-impl.hh>

//#include <bdddomain/relations.hh>

int main(void) {

//  cout << TupleIO("(",")"," ");

  std::cout << "Starting the test " << std::endl;
  {
    
    MPG::VarImpl::RelationImpl unary(1), binary(2), ternary(3), fourthary(4), eightary(8);
    
    // fill some elements in the binary relation
    binary.add({3,1});
    binary.add({2,3});
    binary.add({3,1});
    binary.add({3,2});
    binary.add({5,4});
    binary.add({1,0});
    binary.add({0,1});
    /*
    std::cout << "Binary relation "
	      << binary << std::endl;
    */
    //auto uq = binary.unique(0);
    //std::cout << "Unique quantification yields: " << uq << std::endl;
    
    ternary.add({0,0,1});
    ternary.add({0,1,1});
    ternary.add({0,0,2});
    ternary.add({1,2,3});
    /*
    std::cout << "Ternary relation: "
	      << ternary << std::endl;
    */

    fourthary.add({3,1,2,3});
    fourthary.add({2,3,0,1});
    fourthary.add({3,1,1,0});
    fourthary.add({3,2,5,4});
    fourthary.add({5,4,3,1});
    fourthary.add({1,0,5,2});
    fourthary.add({0,1,1,0});

    std::cout << "Fourthary relation: "
	      << fourthary << std::endl;
    
    //auto tuq = ternary.unique({0,1});
    //std::cout << "Unique quantification yields: " << tuq << std::endl;
    //auto teq = ternary.exists({0,1});
    //std::cout << "Unique quantification yields: " << teq << std::endl;
    
    //MPG::VarImpl::RelationImpl j1 = binary.join(1, ternary);
    //std::cout << "Join on one column (" << j1.cardinality() << "): " << j1 << std::endl;
  
    /*
    MPG::VarImpl::RelationImpl discard12  = ternary.discard(1, 2);
    std::cout << "Discard [1..2[ from ternarry "
	      << discard12 << std::endl;
    */

    {
      // discard first column of a relation
      MPG::VarImpl::RelationImpl discard0  = ternary.discard(0, 1);
      MPG::VarImpl::RelationImpl discard0Expected(2);
      discard0Expected.add({0,0});
      discard0Expected.add({0,1});
      discard0Expected.add({1,2});
      /*
      std::cout << "Discard [0..1[ from ternarry "
		<< discard0 << std::endl;
      */
      std::cout << "###Success: " << discard0Expected.equal(discard0) << std::endl; 
    }

    {
      // discard last column of a relation
      MPG::VarImpl::RelationImpl discard3  = ternary.discard(3, 4);
      MPG::VarImpl::RelationImpl discard3Expected(2);
      discard3Expected.add({1,1});
      discard3Expected.add({0,1});
      discard3Expected.add({0,2});
      discard3Expected.add({2,3});

      /*
      std::cout << "Discard [3..4[ from ternarry "
		<< discard3 << std::endl;
      */
      std::cout << "###Success: " << discard3Expected.equal(discard3) << std::endl; 
    }

    {
      // discard all columns of a relation
      MPG::VarImpl::RelationImpl discardAll  = ternary.discard(0, 4);
      MPG::VarImpl::RelationImpl discardAllExpected(0);
      std::cout << "###Success: " << discardAllExpected.equal(discardAll) << std::endl; 
    }

    {
      // discard middle 
      MPG::VarImpl::RelationImpl discard4  = fourthary.discard(2, 4);
      MPG::VarImpl::RelationImpl discard3Expected(2);
      discard3Expected.add({2,3});
      discard3Expected.add({0,1});
      discard3Expected.add({1,0});
      discard3Expected.add({5,4});
      discard3Expected.add({3,1});
      discard3Expected.add({5,2});
      discard3Expected.add({1,0});
      
      std::cout << "Discard [3..4[ from fourthary "
		<< discard4 << std::endl;
      
      std::cout << "###Success: " << discard3Expected.equal(discard4) << std::endl; 
    }

     /*  
    MPG::VarImpl::RelationImpl discard0 = ternary.discard(0,1);
    std::cout << "Discard 0..0 from ternarry "
	      << discard0 << std::endl;
    
    */
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

