#include <gecode/search.hh>
#include <cprel/cprel.hh>
#include <vector>
#include <algorithm>

using namespace Gecode;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;


template< class T >
std::vector<std::pair<int,int>> reorder(vector<T> &v, vector<size_t> const &order )  {   
  std::vector<std::pair<int,int>> swaps;
  for ( int s = 1, d; s < order.size(); ++ s ) {
    for ( d = order[s]; d < s; d = order[d] ) ;
    if ( d == s ) 
      while ( d = order[d], d != s ) {
        std::swap( v[s], v[d]);
        swaps.push_back(std::make_pair(s,d));
      }
  }
  return swaps;
}

std::vector<std::pair<int,int>> pJ(int n, int m, int j, int k) {
  // prepare a vector with the original columns
  std::vector<int> vA(k,-1);
  for (int i = 0; i < vA.size(); i++)
    vA[i] = i;
  
  std::vector<size_t> xPrime, J, yPrime;
  size_t s = 0;
  for (size_t i = 0; i < n-j; i++) {
    xPrime.push_back(s);
    s++;
  }
  for (size_t i = 0; i < j; i++) {
    J.push_back(s);
    s++;
  }
  for (size_t i = 0; i < m-j; i++) {
    yPrime.push_back(s);
    s++;
  }

  cout << "xprime: ";
  for (auto& i : xPrime)
    cout << " " << i;
  cout << endl;


  cout << "yprime: ";
  for (auto& i : yPrime)
    cout << " " << i;
  cout << endl;

  cout << "j: ";
  for (auto& i : J)
    cout << " " << i;
  cout << endl;
      
  std::vector<size_t> order;
  std::copy(xPrime.begin(), xPrime.end(), std::back_inserter(order));
  std::copy(yPrime.begin(), yPrime.end(), std::back_inserter(order));
  std::copy(J.begin(), J.end(), std::back_inserter(order));

  cout << "The vector" << endl;
  for (const auto& i : vA)
    cout << " " << i;
  cout << endl;
  
  cout << "The order" << endl;
  for (const auto& i : order)
    cout << " " << i;
  cout << endl;
  
  // find the permutations
  auto p = reorder(vA, order);

  // the swaps
  for (const auto& a : p) {
    cout << a.first << " -=- " << a.second << endl;
  }

  // convert into right index based
  for (auto& a : p) {
    a.first = vA.size() - 1 - a.first;
    a.second = vA.size() - 1 - a.second;
  }
  return p;
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

    CPRelVar permJoinResult(*this,GRelation(arityJoin),GRelation::create_full(arityJoin));
    permutation(*this,joinResult,permJoinResult,pJ(n,m,j,k));

    permT = CPRelVar(*this,GRelation(k),GRelation::create_full(k));
    auto permDescT = pJ(n,m,j,k);
    for (auto& a : permDescT)
      cout << a.first << " <--> " << a.second  << endl;
    
    //permutation(*this,t,permT,permC(n,m,j,k));
    //permutation(*this,t,permT,permDescT);
    permutation(*this,t,permT,{{0,2},{0,1}});
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
  /*
  char   A[]     = { '1', '2', '3', '7' };
  size_t ORDER[] = { 0, 2, 3, 1 };

  vector<char>   vA(A, A + sizeof(A) / sizeof(*A));
  vector<size_t> vOrder(ORDER, ORDER + sizeof(ORDER) / sizeof(*ORDER));
  */
  int   A[]     = { 0, 1, 2, 3 };
  size_t ORDER[] = { 0, 2, 3, 1 };

  vector<int>   vA(A, A + sizeof(A) / sizeof(*A));
  vector<size_t> vOrder(ORDER, ORDER + sizeof(ORDER) / sizeof(*ORDER));

  auto p = reorder(vA, vOrder);
  /*
  // convert into right index based
  for (auto& a : p) {
    a.first = vA.size() - 1 - a.first;
    a.second = vA.size() - 1 - a.second;
  }
  */
  
  for (auto& a : p)
    cout << a.first << " <> " << a.second  << endl;


  auto jo = pJ(3,3,2,4);
  /*  
  for (auto& a : jo)
    cout << a.first << " <--> " << a.second  << endl;
  */
  /*  
  JoinTest* g = new JoinTest();
  DFS<JoinTest> e(g);

  std::cout << "Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<JoinTest*>(s)->print(std::cout);
    delete s;
  }
  delete g;
  */
  return 0;
}
