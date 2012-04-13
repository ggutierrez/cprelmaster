#include <gecode/search.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;

std::vector<std::pair<int,int>> permJoin(int n, int m, int j, int k) {
  // | n-j | j | m-j |
  // ->
  // | n-j | m-j | j |
  std::vector<std::pair<int,int>> result;
  
  for (int i = 0; i < m-j; i++)
    result.push_back(std::make_pair(i,i+j));
  int s = 0;
  for (int i = m-j; i < m; i++)  {
    result.push_back(std::make_pair(i,s));
    s++;
  }
  return result;
}
  
// Find the permutation for the result of the join
std::vector<std::pair<int,int>> permC(int n, int m, int j, int k) {
  // | n-j | m-j | j |
  // ->
  // | n-j | j | m-j |
  std::vector<std::pair<int,int>> result;
  
  for (int i = 0; i < j; i++)
    result.push_back(std::make_pair(i,i+m-j));
  int s = 0;
  for (int i = j; i < m; i++)  {
    result.push_back(std::make_pair(i,s));
    s++;
  }
  return result;
}


pair<GRelation,GRelation> domR(void) {
  GRelation gr(3);
  gr.add({1,2,3});
  gr.add({1,2,4});
  return make_pair(gr,gr);
}

pair<GRelation,GRelation> domS(void) {
  GRelation gr(3);
  gr.add({
      {2,3,7},
      {2,4,7},
        //     {2,4,9}
    });
  return make_pair(gr,gr);
}

pair<GRelation,GRelation> domT(void) {
  GRelation ub(4);
  ub.add({
      {1,2,3,7},
      {1,2,4,7}
    });
  return make_pair(GRelation(4),ub);
}

class JoinTest : public Gecode::Space {
protected:
  CPRelVar r,s,t;
  CPRelVar joinResult, followAllResult;
public:
  JoinTest(void)  {

    pair<GRelation,GRelation> dr = domR();
    r = CPRelVar(*this,dr.first,dr.second);

    pair<GRelation,GRelation> ds = domS();
    s = CPRelVar(*this,ds.first,ds.second);

    pair<GRelation,GRelation> dt = domT();
    t = CPRelVar(*this,dt.first,dt.second);

    int n = r.arity();
    int m = s.arity();
    int k = t.arity();
    int j = 2;
    int arityJoin = r.arity() + s.arity() - j;
    int arityFollow = arityJoin - j;
    
    joinResult = CPRelVar(*this, GRelation(arityJoin), GRelation::create_full(arityJoin));
    join(*this,r,j,s,joinResult);    

    followAllResult = CPRelVar(*this,GRelation(arityFollow),GRelation::create_full(arityFollow));
    followAll(*this,r,j,s,followAllResult);

    CPRelVar u(*this,GRelation(j),GRelation::create_full(j));
    CPRelVar times(*this,GRelation(arityJoin),GRelation::create_full(arityJoin));
    join(*this,followAllResult,0,u,times);

    CPRelVar permJoinResult(*this,GRelation(arityJoin),GRelation::create_full(arityJoin));
    permutation(*this,joinResult,permJoinResult,permJoin(n,m,j,k));

    CPRelVar permT(*this,GRelation(k),GRelation::create_full(k));
    permutation(*this,t,permT,permC(n,m,j,k));
    
    branch(*this,t);
    //restrJoinAll(*this,r,j,s,t);
  }
  void print(std::ostream& os) const {
    os << "Print method called" << std::endl;
    os << "R: " << r << endl;
    os << "S: " << s << endl;
    os << "T: " << t << endl;

    //os << "JoinResult: " << joinResult << endl;
    //os << "followAllResult: " << followAllResult << endl;
    

  }
  JoinTest(bool share, JoinTest& sp)
    : Gecode::Space(share,sp) {
    r.update(*this, share, sp.r);
    t.update(*this, share, sp.t);
    s.update(*this, share, sp.s);
    
    joinResult.update(*this, share, sp.joinResult);
    followAllResult.update(*this, share, sp.followAllResult);
  }
  virtual Space* copy(bool share) {
    return new JoinTest(share,*this);
  }
};

int main(int, char**) {
  JoinTest* g = new JoinTest();
  DFS<JoinTest> e(g);

  std::cout << "Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<JoinTest*>(s)->print(std::cout);
    delete s;
  }
  delete g;
  return 0;
}
