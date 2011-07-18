#include <cprel/prop/channel.hh>
#include <bdddomain/perm-descriptor.hh>

namespace MPG {

using namespace CPRel;
using namespace CPRel::Prop;
using namespace Gecode;
using namespace Gecode::Set;

void channel(Gecode::Space& home, CPRelVar R, SetVar S) {
  if (home.failed()) return;

  if (R.arity() != 1)
    throw ArityMissmatch("channel constraint expects a unary relation");

  /// \todo Here we should check the limits of the set domain with the configuration
  /// of the bdd package and throw an exception if they do not coincide.
  CPRelView rel(R);
  Gecode::Set::SetView set(S);
  GECODE_ES_FAIL((Channel<CPRelView,Gecode::Set::SetView>::post(home,rel,set)));
}

}
