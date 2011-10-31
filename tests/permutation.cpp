#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;

pair<GRelation,GRelation> domR(void) {
  GRelation ub(3);
  ub.add({
   {1,3,2},
   {4,5,7},
   {8,2,2},
   {6,5,3}
  });

  GRelation lb(3);
  lb.add(make_Tuple(8,2,2));

  return make_pair(GRelation(3),ub);
}

pair<GRelation,GRelation> domS(void) {
  GRelation ub(3);
  ub.add({
      {3,1,2},
      {5,4,7},
      {2,8,2},
      {5,6,3}
  });
  return make_pair(GRelation(3),ub);
}

class PermutationTest : public Gecode::Space {
protected:
  CPRelVar r,s;
public:
  PermutationTest(void)  {

    auto dr = domR();
    r = CPRelVar(*this,dr.first,dr.second);

    //std::cout << "RelationR " << r.lub() << std::endl;

    auto ds = domS();
    s = CPRelVar(*this,ds.first,ds.second);


    PermDescriptor d;
    d.permute(1,2);
    
    //std::cout << "RelationS " << s.lub() << std::endl;
    //std::cout << "Relation----S " << s.lub().permute(d) << std::endl;
    
    permutation(*this,s,r,d);
    branch(*this,r);
  }
  void print(std::ostream& os) const {
    std::cout << "Relation R " << r << std::endl; 
    std::cout << "Relation S " << s << std::endl;  
  }
  PermutationTest(bool share, PermutationTest& sp)
    : Gecode::Space(share,sp) {
    r.update(*this, share, sp.r);
    s.update(*this, share, sp.s);
  }
  virtual Space* copy(bool share) {
    return new PermutationTest(share,*this);
  }
};

int main(int, char**) {
  // Setup the way tuples are printed
  //  std::cout << TupleIO("<td>","</td>"," ");
  // Setup the way relations are printed
  //  std::cout << GRelationIO("<table>","</table>","<tr>","</tr>");
  
  PermutationTest* g = new PermutationTest();

  Gecode::DFS<PermutationTest> e(g);
  delete g;
  
  int solutionsFound = 0;
  std::cout << "### Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<PermutationTest*>(s)->print(std::cout);
    solutionsFound++;
    std::cout << "**** End solution ****" << std::endl; 
    delete s;
  }
  std::cout << "### search ends, Total solutions: " << solutionsFound << std::endl; 

  /* 
  Gist::Print<PermutationTest> p("PermutationTest");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;
  */
  return 0;
}
