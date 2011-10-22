#include <iostream>
#include <vector>
#include <bdddomain/tuple.hh>
#include <bdddomain/rel-impl.hh>

//#include <bdddomain/relations.hh>

int main(void) {

//  cout << TupleIO("(",")"," ");

  std::cout << "Starting the test " << std::endl;
  {
    
    MPG::VarImpl::RelationImpl unary(1), binary(2), ternary(3), fourthary(4);
    
    // fill some elements in the binary relation
    binary.add({3,1});
    //binary.add({2,3});
    //  binary.add({3,1});
    binary.add({3,2});
    //binary.add({5,4});
    binary.add({1,0});
    binary.add({0,1});
    //std::cout << "Binary relation: (" << binary.cardinality() << "): " << binary << std::endl;
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
    std::cout << "Ternary replace: " 
              << ternary.replace({{1,0}})
              << std::endl;

    std::cout << "Ternary swap: " 
              << ternary.swap({{0,1}})
              << std::endl;

    /*
    auto t = MPG::Tuple({2,3,1});
    for (int i : t.value()) {
      std::cout << "Tuple has " << i << std::endl;
    }
    std::cout << std::endl;
    */
    /*
    // fill the unary relation

    unary.add(MPG::make_Tuple(0));
    */
    //std::cout << "Unary relation: " << unary << std::endl;
    
    //binary.remove({2,3});
    //binary.remove({5,5});
    //std::cout << "Binary relation: (" << binary.cardinality() << "): " << binary << std::endl;
    
    //auto e0binary = binary.exists(0);
    //std::cout << "Existentialy quant 0 in binary gives: " << e0binary << std::endl;


    //PermDescriptor d;
    //d.permute(0,2); d.permute(1,0); d.permute(2,1);

    //RelationImpl x(r.permute(d));

//    RelationImpl s = r.shiftRight(1);
//    cout << s << endl;
//    cout << "Shift right " <<  " Cardinality " << s.cardinality() << std::endl;

//    // initial relation
    //cout << "New relation " << x << " with cardinality " << x.cardinality() << std::endl;

  }
  return 0;
}

