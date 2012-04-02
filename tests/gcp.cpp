#include <gecode/search.hh>
#include <cprel/cprel.hh>

using namespace Gecode;
using namespace MPG;

class GCP : public Gecode::Space {
protected:
  GRelation graph;
  GRelation colors;
  CPRelVar colorLeft, colorRight, colorMapping;
  CPRelVar tmp;
public:
  GCP(void) : graph(2), colors(1) 
  {
    // graph 
    {
      graph.add(make_Tuple(1,2));
      graph.add(make_Tuple(2,3));
      graph.add(make_Tuple(3,4));
    }
    CPRelVar G(*this,graph,graph);
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
    CPRelVar colorsForb(*this,colorEq,colorEq);
    
    // initialize the color mapping to contain every possible combination of colors.
    colorLeft = CPRelVar(*this,GRelation(4),colors.times(graph.times(colors)));

    PermDescriptor p;
    p.permute(3,1);
    colorRight = CPRelVar(*this,GRelation(4),GRelation::create_full(4));
    permutation(*this,colorLeft,colorRight,p);
    
    tmp = CPRelVar(*this,GRelation(2),GRelation::create_full(2));
    projection(*this,2,colorRight,tmp);

    //subset(*this,G,colorLeft_{first two});
    disjoint(*this,tmp,colorsForb);
    branch(*this,colorLeft);
}
  void print(std::ostream& os) const {
    os << std::endl << std::endl 
      //<< "ColorMapping: " << std::endl
      // << tmp << std::endl
       << "ColorLeft:" << std::endl
       << colorLeft << std::endl
      //<< "ColorRight:" << std::endl
      // << colorRight 
       << std::endl;
  }
  GCP(bool share, GCP& s)
    : Gecode::Space(share,s), graph(s.graph), colors(s.colors) {
    tmp.update(*this,share,s.tmp);
    colorLeft.update(*this,share,s.colorLeft);
    colorRight.update(*this,share,s.colorRight);
  }
  virtual Space* copy(bool share) {
    return new GCP(share,*this);
  }
};


int main(int, char**) {
  GCP* g = new GCP;

  Gecode::DFS<GCP> e(g);
  delete g;
  std::cout << "Search will start" << std::endl;
  while (Gecode::Space* s = e.next()) {
    static_cast<GCP*>(s)->print(std::cout);
    delete s;
  }
  std::cout << "Finishing" << std::endl;
  return 0;
}
