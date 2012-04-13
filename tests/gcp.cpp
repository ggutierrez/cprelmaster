#include <gecode/search.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using namespace MPG;

class GCP : public Gecode::Space {
protected:
  GRelation graph;
  GRelation colors;
  //CPRelVar possibles, colorCT, graphCT;
  CPRelVar r, r_prime;
  CPRelVar tmp, tmp2;
  CPRelVar tmp3;
  //, tmp2, G;
  CPRelVar G; // a constraint variable with the graph, fixed
public:
  GCP(void) : graph(2), colors(1) 
  {
    // graph 
    {
      graph.add(make_Tuple(1,2));
      graph.add(make_Tuple(2,3));
      //graph.add(make_Tuple(3,4));
    }
    G = CPRelVar(*this,graph,graph);
    //tmp = CPRelVar(*this,GRelation(2),graph);
    //subset(*this,G,tmp);
 
    // colors
    {
      colors.add(make_Tuple(10));
      colors.add(make_Tuple(11));
    }
    GRelation colorEq(2);
    {
      colorEq.add(make_Tuple(10,10));
      colorEq.add(make_Tuple(11,11));
    }
    GRelation possibleColors = colors.times(colors).difference(colorEq);
    tmp3 = CPRelVar(*this,possibleColors,possibleColors);
    
    GRelation r_max = graph.times(possibleColors);
    //cout << "R max" << endl << r_max << endl;
    r = CPRelVar(*this,GRelation(4),r_max);

    r_prime = CPRelVar(*this,GRelation(4),GRelation::create_full(4));
    std::vector<std::pair<int,int> > perm = {{1,3},{2,0}};
    permutation(*this,r,r_prime,perm);
    
    tmp = CPRelVar(*this,GRelation(2),graph);
    projection(*this,2,r_prime,tmp);
    subset(*this,G,tmp);

    tmp2 = CPRelVar(*this,GRelation(2),possibleColors);
    projection(*this,2,r,tmp2);
    subset(*this,tmp2,tmp3);
         
    branch(*this,r_prime);
}
  void print(std::ostream& os) const {
    os << std::endl << std::endl 
      //<< "ColorMapping: " << std::endl
      // << tmp << std::endl
      // << "Possibles:" << std::endl
      // << possibles << std::endl
      // << "ColorCT:" << std::endl
      // << colorCT
      // << "GraphCT:" << std::endl
      // << graphCT
       << "R:" << std::endl
       << r
       << "Tmp: " << std::endl
       << tmp
       << endl
       << "Tmp2: " << std::endl
       << tmp2
       << endl
       << "R':" << std::endl
       << r_prime
      
      //<< "Tmp3: " << tmp3 << endl
      
      //<< "G:" << endl
      // << G
       << std::endl;
  }
  GCP(bool share, GCP& s)
    : Gecode::Space(share,s), graph(s.graph), colors(s.colors) {
    G.update(*this,share,s.G);
    r.update(*this,share,s.r);
    r_prime.update(*this,share,s.r_prime);
    tmp.update(*this,share,s.tmp);    
    tmp2.update(*this,share,s.tmp2);    
    tmp3.update(*this,share,s.tmp3);    
    /*

    tmp2.update(*this,share,s.tmp2);
    
    possibles.update(*this,share,s.possibles);
    colorCT.update(*this,share,s.colorCT);
    graphCT.update(*this,share,s.graphCT);
    */
  }
  virtual Space* copy(bool share) {
    return new GCP(share,*this);
  }
};


int main(int, char**) {
  GCP* g = new GCP;

  int sols = 0;
  Gecode::DFS<GCP> e(g);
  delete g;
  std::cout << "Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    std::cout << "Next solution: " << std::endl;
    static_cast<GCP*>(s)->print(std::cout);
    delete s;
    sols++;
  }
  std::cout << "Finishing: " << sols << std::endl;
  return 0;
}
