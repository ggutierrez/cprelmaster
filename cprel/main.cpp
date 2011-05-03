#include <cprel/cprel.hh>


namespace MPG { namespace CPRel {
/*
 * Value selection classes
 *
 */

// Select smallest value
    class ValMin : public Gecode::ValSelBase<CPRel::CPRelView,int> {
    public:
      ValMin(void) {}
      ValMin(Gecode::Space& home, const Gecode::ValBranchOptions& vbo)
	: Gecode::ValSelBase<CPRel::CPRelView,int>(home,vbo) {}
      int val(Gecode::Space&, View x) const {
	return x.min();
      }
      ModEvent tell(Gecode::Space& home, unsigned int a, CPRel::CPRelView x, int n) {
	return (a == 0) ? x.lq(home,n) : x.gq(home,n+1);
      }
};
  }
  
  /*
   * Branching post function
   *
   */
  
  // Branch by selecting the first unassigned variable, assigning its
  // smallest value first
  void branch(Gecode::Home home, const CPRelVarArgs& x) {
    using namespace CPRel;
    if (home.failed()) return;
    Gecode::ViewArray<CPRel::CPRelView> y(home,x);
    Gecode::ViewSelNone<CPRel::CPRelView> sn; ValMin vm;
    Gecode::ViewValBrancher<
      Gecode::ViewSelNone<CPRel::CPRelView>,ValMin>::post(home,y,sn,vm);
  }
}

Gecode::VarImpDisposer<MPG::CPRel::CPRelVarImp> d;
