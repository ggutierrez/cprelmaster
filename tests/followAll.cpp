#include <gecode/search.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using std::pair;
using std::make_pair;
using namespace MPG;
using namespace MPG::CPRel;

pair<GRelation,GRelation> domR(void) {
  GRelation lb(4);
  lb.add({
   {1,3,2,2},
   {1,3,2,4},
   {1,5,7,2}
  });

  GRelation ub(4);
  ub.add({
   {1,3,2,2},
   {1,3,2,4},
   {1,5,7,2}
  });

  return make_pair(lb,ub);
}

pair<GRelation,GRelation> domS(void) {
  GRelation lb(3);
  lb.add({
      {2,8,9},
      {2,8,10},
  });

  GRelation ub(3);
  ub.add({
      {2,8,9},
      {2,8,10},
  });
  return make_pair(lb,ub);
}

pair<GRelation,GRelation> domT(void) {
  GRelation lb(5);
  /* 
 lb.add({
      {1,2},
      {1,2},
  });
  */
  /*
 GRelation ub(4);
  ub.add({
      {1,3,2,2,2},
      {1,5,7,2,2},
  });
  */
  GRelation ub = GRelation::create_full(5);
  /*
  GRelation ub(2);
  ub.add({
      {1,2},
      {1,2},
  });
  */
  return make_pair(lb,ub);
}

class FollowTest : public Gecode::Space {
protected:
  CPRelVar r,s,t;
  CPRelVar complS, complT, resultF, resultFALL;
public:
  FollowTest(void)  {

   pair<GRelation,GRelation> dr = domR();
   r = CPRelVar(*this,dr.first,dr.second);

   pair<GRelation,GRelation> ds = domS();
   s = CPRelVar(*this,ds.first,ds.second);

   pair<GRelation,GRelation> dt = domT();
   t = CPRelVar(*this,dt.first,dt.second);

   // initialize the complements
   complS = CPRelVar(*this,GRelation(3), GRelation::create_full(3));
   complT = CPRelVar(*this,GRelation(5), GRelation::create_full(5));


   resultF = CPRelVar(*this,GRelation(5), GRelation::create_full(5));
   resultFALL = CPRelVar(*this,GRelation(5), GRelation::create_full(5));

   // the constraints
   follow(*this,r,1,s,resultF);
   intersect(*this,resultF,t,resultFALL);
   complement(*this,s,complS);
   complement(*this,t,complT);
   follow(*this,r,1,complS,complT);
  }

  void print(std::ostream& os) const {
    os << "R" << r << endl;
    os << "S" << s << endl;
    os << "Result " << resultFALL << endl;
 }
  FollowTest(bool share, FollowTest& sp)
    : Gecode::Space(share,sp) {
    r.update(*this, share, sp.r);
    t.update(*this, share, sp.t);
    s.update(*this, share, sp.s);
    complS.update(*this, share, sp.complS);
    complT.update(*this, share, sp.complT);
    resultF.update(*this, share, sp.resultF);
    resultFALL.update(*this, share, sp.resultFALL);
  }
  virtual Space* copy(bool share) {
    return new FollowTest(share,*this);
  }
};

int main(int, char**) {

  FollowTest* g = new FollowTest();
  DFS<FollowTest> e(g);

  std::cout << "Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<FollowTest*>(s)->print(std::cout);
    delete s;
  }

  delete g;
  return 0;
}
