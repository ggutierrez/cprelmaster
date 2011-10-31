#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using std::cout;
using std::cerr;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;

pair<GRelation,GRelation> domR(void) {
  GRelation ub(2);
  ub.add(make_Tuple(1,2));
  ub.add(make_Tuple(0,2));
  ub.add(make_Tuple(5,2));
  ub.add(make_Tuple(5,5));
  
  return make_pair(GRelation(2),ub);
}

pair<GRelation,GRelation> domQ(void) {
  GRelation ub(1);
  ub.add(make_Tuple(2));
  ub.add(make_Tuple(0));
  
  return make_pair(GRelation(1),ub);
}

class ProjectionTest : public Gecode::Space {
protected:
  CPRelVar r,q;
public:
  ProjectionTest(void)  {
    
    pair<GRelation,GRelation> dr = domR();
    r = CPRelVar(*this,dr.first,dr.second);
    pair<GRelation,GRelation> dq = domQ();
    q = CPRelVar(*this,dq.first,dq.second);
    
    //    std::cerr << "R:" << r << std::endl;
    //    std::cerr << "Q:" << q << std::endl;
    
    projection(*this,1,r,q);

    branch(*this,q);
    branch(*this,r);
  }
  
  void print(std::ostream& os) const {
    std::cout << "Relation R " << r << std::endl; 
    std::cout << "Relation Q " << q << std::endl; 
  }
  
  ProjectionTest(bool share, ProjectionTest& s)
  : Gecode::Space(share,s) {
    r.update(*this, share, s.r);
    q.update(*this, share, s.q);
  }
  virtual Space* copy(bool share) {
    return new ProjectionTest(share,*this);
  }
};

int main(int, char**) {
  ProjectionTest* g = new ProjectionTest();

  Gecode::DFS<ProjectionTest> e(g);
  delete g;
  
  int solutionsFound = 0;
  std::cout << "### Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<ProjectionTest*>(s)->print(std::cout);
    solutionsFound++;
    std::cout << "**** End solution ****" << std::endl; 
    delete s;
  }
  std::cout << "### search ends, Total solutions: " << solutionsFound << std::endl; 

  /*
  Gist::Print<ProjectionTest> p("Print solution");
  Gist::Options o;
  o.inspect.click(&p);
  Gist::dfs(g,o);
  delete g;
  */
  return 0;
}
