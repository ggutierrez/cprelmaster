#include <cprel/prop/equality.hh>
#include <cprel/descriptor.hh>

namespace MPG {

using namespace CPRel;
using namespace CPRel::Prop;
void equal(Gecode::Space& home, CPRelVar A, CPRelVar B) {
  if (home.failed()) return;

  if (A.arity() != B.arity())
    throw ArityMismatch("equality constraint");

  CPRelView left(A);
  CPRelView right(B);
  GECODE_ES_FAIL((Equal<CPRelView,CPRelView>::post(home,left,right)));
}

void complement(Gecode::Space& home, CPRelVar A, CPRelVar B) {
  if (home.failed()) return;

  if (A.arity() != B.arity())
    throw ArityMismatch("complement constraint");

  CPRelView left(A);
  CPRelView vb(B);
  ComplementView<CPRelView> right(vb);
  GECODE_ES_FAIL((
    Equal<CPRelView,ComplementView<CPRelView> >::post(home,left,right))
                 );
}

void permutation(Gecode::Space& home, CPRelVar A, CPRelVar B, const PermDescriptor& desc) {
  if (home.failed()) return;

  if (A.arity() != B.arity())
    throw ArityMismatch("permutation constraint");

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
  if (!desc.valid(A.arity()))
    throw InvalidPermutation("permutation constraint (invalid especification)");

  CPRelView left(A);
  CPRelView right(B);
  PermutedView<CPRelView>  permRight(right,desc);
  GECODE_ES_FAIL((Equal<CPRelView,PermutedView<CPRelView> >::
                  post(home,left,permRight)));
}

}
