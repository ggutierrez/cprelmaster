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
    { // Permute some columns in the fourthary relation
      MPG::VarImpl::RelationImpl p01  = fourthary.permute({{0,1}});
      //std::cout << "Permutation of 0 1 " << p01 << std::endl; 
      
      MPG::VarImpl::RelationImpl p01p  = p01.permute({{0,1}});
      //std::cout << "Permutation of 0 1 p " << p01p << std::endl; 
      std::cout << "###Success: " << p01p.equal(fourthary)  << std::endl; 

      MPG::VarImpl::RelationImpl p03  = fourthary.permute({{0,3},{1,2}});
      std::cout << "Permutation of 0 r p " << p03 << std::endl; 
    }

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
      /*
      std::cout << "Discard [3..4[ from fourthary "
		<< discard4 << std::endl;
      */
      std::cout << "###Success: " << discard3Expected.equal(discard4) << std::endl; 
    }
    {
      // particular example with discard:
      MPG::VarImpl::RelationImpl d(5);
      d.add({0, 0, 1, 1, 0});
      d.add({1, 2, 3, 0, 1});
      //std::cout << "Input relation " <<  d << std::endl; 
      //std::cout << "Discarding result: " << d.discard(2,4) << std::endl; 
      
    }
    { // join binary and ternary
      MPG::VarImpl::RelationImpl join1bt = binary.join(1,ternary);
      //std::cout << "Join binary and ternary on 1 "  << join1bt << std::endl; 

    }
    { // follow binary and ternary
      MPG::VarImpl::RelationImpl follow1bt = ternary.follow(2,fourthary);
      //std::cout << "Follow binary and ternary on 1 "  << follow1bt << std::endl; 
    }
    { // project a relation
      MPG::VarImpl::RelationImpl p1 = ternary.project(1); 
      //std::cout << "Ternary projected on first column " << p1 << std::endl;
     
      MPG::VarImpl::RelationImpl p2 = ternary.project(2); 
      //std::cout << "Ternary projected on first two columns " << p2 << std::endl;
      
      MPG::VarImpl::RelationImpl p3 = fourthary.project(3); 
      //std::cout << "Fourthary projected on first three columns " << p3 << std::endl;      
    }

    { // shift the columns of a relation to the right
      MPG::VarImpl::RelationImpl p1 = ternary.shiftRight(1); 
      //std::cout << "Relation shifted 1 "  << p1 << std::endl; 

      MPG::VarImpl::RelationImpl p11 = p1.shiftRight(1); 
      //std::cout << "Relation p1 shifted 1 "  << p11 << std::endl; 

      MPG::VarImpl::RelationImpl p2 = ternary.shiftRight(2); 
      //std::cout << "Relation shifted 2 "  << p2 << std::endl; 

      std::cout << "###Success: " << (p11.equal(p2)) << std::endl;  
    }
  }
  return 0;
}

