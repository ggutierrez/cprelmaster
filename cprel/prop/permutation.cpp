#include <cprel/prop/permutation.hh>

// The reason for this undefne statement is because CUDD defines a
// macro with this name and this confusses gcc and clang with the fail
// method of the Space class that is called by the GECODE_ES_FAIL
// macro.
#undef fail 

namespace MPG {
  
  using namespace CPRel;
  using namespace CPRel::Prop;
  
  void permutation(Gecode::Space& home, CPRelVar A, CPRelVar B, std::vector<std::pair<int,int> > desc) {
    if (home.failed()) return;

    if (A.arity() != B.arity())
      throw ArityMissmatch("permutation constraint");
    
    if (desc.size() == 0) {
      std::cerr << "Posting permutation with emtpy description does nothing"
		<< std::endl;
      return;
    }

    /*
    // If this constraint is posted on the same variable then the permutation must
    // be the identity function
    if (A.same(B)) {
    for (DescIterator it(desc); it(); ++it) {
    const std::pair<int,int>& x = it.val();
    if (x.first != x.second) {
    std::cerr << "permutation is expected to be the identity because both "
    << "arguments the same variable";
    return ES_FAILED;
    }
    }
    return ES_OK;
    }
    */
    //if (!desc.valid(A.arity()))
    //  throw InvalidPermutation("permutation constraint (invalid especification)");

    CPRelView left(A);
    CPRelView right(B);
    GECODE_ES_FAIL((Permute::post(home,left,right,desc)));
}

}
