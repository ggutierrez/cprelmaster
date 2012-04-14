#include <gecode/search.hh>
#include <cprel/cprel.hh>
#include <vector>
#include <algorithm>

using namespace Gecode;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;


//put the joined part on the right
std::vector<std::pair<int,int>> putJoinedPartOnRight(int j, int m)  {
  std::vector<std::pair<int,int>> result;
  if (j >= m - j) {
    for (int i = 0; i < m - j  ; i++)
      result.push_back(std::make_pair(i,i + j));
  }
  else {
    cout << "m -j " << m - j << endl;
    cout << "m : " << m << " and j : " << j << endl;
    for (int i = 0; i <= j - 1 ; i++)
      result.push_back(std::make_pair(i,i + m - j ));
  }
  
  return result;
}

//create the permutation description needed to reorder one columns if j!= m - j
//indexStart is the number of the column that will be reorder
//indexEnd is the number of the column to which the column indexStart will go
std::vector<std::pair<int,int>> permJoinReorderOne(int indexStart, int indexEnd)  {
  std::vector<std::pair<int,int>> result;
  for (int i = indexStart; i < indexEnd ; i++)
    result.push_back(std::make_pair(i,i+1));
  return result;
}

//create the permutation description needed to reorder columns if j!= m - j
std::vector<std::pair<int,int>> permJoinReorder(int j, int m)  {
  std::vector<std::pair<int,int>> result;
  std::vector<std::pair<int,int>> temp;  

  //if the joined part is smaller
  if(j < m - j) {
    for(int i = m - j - 1 ; i >= j ; i--) {
      temp = permJoinReorderOne(i,i+j);
      copy (temp.begin(),temp.end(),back_inserter(result));
    }
  }
  //if the joined part is bigger
  else if(j > m - j) {
    for(int i = m - j - 1 ; i >= 0 ; i--) {
      temp = permJoinReorderOne(i,i + j - 1);
      copy (temp.begin(),temp.end(),back_inserter(result));
    }
  }
  return result;
}

std::vector<std::pair<int,int>> permJoin(int m, int j) {
  // | n-j | j | m-j |
  // ->
  // | n-j | m-j | j |
  std::vector<std::pair<int,int>> result; 
  std::vector<std::pair<int,int>> temp;
  
  //first we put the joined column on the right
  result = putJoinedPartOnRight(j,m);
  /*
  for(auto i = result.begin() ; i!=result.end() ; ++i)
    std::cout << i->first << "|" << i->second << std::endl;
  */
  //now we adapat if needed
  if (j!=m) {
    temp = permJoinReorder(j,m);
    copy (temp.begin(),temp.end(),back_inserter(result));
  }
  /*
  for(auto i = result.begin() ; i!=result.end() ; ++i)
    std::cout << i->first << "|" << i->second << std::endl;
  */
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
  CPRelVar joinResult, followAllResult, permT, permJoinResult, times;
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

    CPRelVar u(*this,GRelation::create_full(j),GRelation::create_full(j));
    times = CPRelVar(*this,GRelation(arityJoin),GRelation::create_full(arityJoin));
    join(*this,followAllResult,0,u,times);

    permJoinResult = CPRelVar(*this,GRelation(arityJoin),GRelation::create_full(arityJoin));
    permutation(*this,joinResult,permJoinResult,permJoin(m,j));

    permT = CPRelVar(*this,GRelation(k),GRelation::create_full(k));
    permutation(*this,t,permT,permJoin(m,j));
    
    intersect(*this,times,permJoinResult,permT);
    //branch(*this,t);
    //restrJoinAll(*this,r,j,s,t);
  }
  void print(std::ostream& os) const {
	
    os << "Print method called" << std::endl;
    os << "R: " << r << endl;
    os << "S: " << s << endl;
    os << "joinResult: " << joinResult << endl;
    os << "permJoinResult: " << permJoinResult << endl;
    os << "followAllResult: " << followAllResult.assigned() << endl;
    os << "times: " << times.assigned() << endl;
    os << "T: " << t << endl;
    os << "permT: " << permT << endl;
    
    
    os << "PermT: " << permT << endl;
    
    
    //os << "JoinResult: " << joinResult << endl;
    //os << "followAllResult: " << followAllResult << endl;
    

  }
  JoinTest(bool share, JoinTest& sp)
    : Gecode::Space(share,sp) {
    r.update(*this, share, sp.r);
    t.update(*this, share, sp.t);
    s.update(*this, share, sp.s);
    permT.update(*this, share, sp.permT);
    permJoinResult.update(*this, share, sp.permJoinResult);
    joinResult.update(*this, share, sp.joinResult);
    followAllResult.update(*this, share, sp.followAllResult);
    times.update(*this, share, sp.times);
    
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

