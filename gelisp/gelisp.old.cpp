#include <iostream>
#include <sstream>
#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <gelisp/gelisp.hpp>

using std::cout;
using std::cerr;
using std::endl;

namespace GeLisp {
  
  using namespace Gecode;
  
  GlSpace::GlSpace(void) 
    :iv(0)
    ,bv(0)
    ,sv(0)
     //,gv(0) 
  {
    //freopen ("/tmp/mystdout.txt","w",stdout);
    //freopen ("/tmp/mystderr.txt","w",stderr);
    //printf ("This sentence is redirected to a file.");
    std::cout << "This should be in the same file" << std::endl;
    std::cerr << "This should be an error" << std::endl;
  }
  
  GlSpace::~GlSpace(void)
  {
    //fclose(stdout);
    //fclose(stderr);
  }
  
  GlSpace::GlSpace(bool share, GlSpace& s) 
    : Space(share,s), iv(s.iv), bv(s.bv), sv(s.sv) {
    for (int i = 0; i<iv.size(); i++) 
      {
	iv[i].update(*this,share,s.iv[i]);
	iv[i].setIndexInVector(s.iv[i].getIndexInVector());
      }
    for (int i = 0; i<bv.size(); i++) 
      {
	bv[i].update(*this,share,s.bv[i]);
	bv[i].setIndexInVector(s.bv[i].getIndexInVector());
      }
	  
    for (int i = 0; i<sv.size(); i++) 
      {
	sv[i].update(*this,share,s.sv[i]);
	sv[i].setIndexInVector(s.sv[i].getIndexInVector());
      }
    /*
      for (int i = 0; i<gv.size(); i++) 
      {
      gv[i].update(*this,share,s.gv[i]);
      gv[i].setIndexInVector(s.gv[i].getIndexInVector());
      }
    */
  }
  
  Space*
  GlSpace::copy(bool share) {
    return new GlSpace(share,*this);
  }
  

  GlIntVar&
  GlSpace::newIntVar(int lb, int ub) {
    //std::cout << "size : " << iv.size() << std::endl;
    iv.push_back(GeLisp::GlIntVar(*this,lb,ub));
    iv.back().setIndexInVector(iv.size() - 1);
    return iv.back();
  }
  
  GlIntVar&
  GlSpace::newIntVar(const IntSet &d){
    iv.push_back(GeLisp::GlIntVar(*this,d));
    iv.back().setIndexInVector(iv.size() - 1);
    return iv.back();
  }
	
	
  GlBoolVar&
  GlSpace::newBoolVar(int lb, int ub){
    bv.push_back(GeLisp::GlBoolVar(*this, lb, ub));
    bv.back().setIndexInVector(bv.size() - 1);
    return bv.back();
  }
  	
  GlSetVar& 
  GlSpace::newSetVar(){
    sv.push_back(GeLisp::GlSetVar(*this));
    sv.back().setIndexInVector(sv.size() - 1);
    return sv.back();
  }
		
    
  GlSetVar&
  GlSpace::newSetVar(int glbMin,int glbMax,int lubMin,int lubMax,
		     unsigned int cardMin,
		     unsigned int cardMax){
    sv.push_back(GeLisp::GlSetVar(*this,glbMin,glbMax,lubMin,lubMax, cardMin,cardMax));
    sv.back().setIndexInVector(sv.size() - 1);
    return sv.back();
  }
           
  GlSetVar&
  GlSpace::newSetVar(const IntSet& glbD,int lubMin,int lubMax,
		     unsigned int cardMin,
		     unsigned int cardMax){
    sv.push_back(GeLisp::GlSetVar(*this,glbD,lubMin,lubMax, cardMin,cardMax));
    sv.back().setIndexInVector(sv.size() - 1);
    return sv.back();
  }       
    
  GlSetVar&
  GlSpace::newSetVar(int glbMin,int glbMax,const IntSet& lubD,
		     unsigned int cardMin,
		     unsigned int cardMax){
    sv.push_back(GeLisp::GlSetVar(*this,glbMin,glbMax,lubD, cardMin,cardMax));
    sv.back().setIndexInVector(sv.size() - 1);
    return sv.back();
  }
           
  GlSetVar&
  GlSpace::newSetVar(const IntSet& glbD,const IntSet& lubD,
		     unsigned int cardMin,
		     unsigned int cardMax){
    sv.push_back(GeLisp::GlSetVar(*this,glbD,lubD, cardMin,cardMax));
    sv.back().setIndexInVector(sv.size() - 1);
    return sv.back();
  }
  /*
    GlGraphVar&
    GlSpace::newGraphVar(int n)
    {
    gv.push_back(GeLisp::GlGraphVar(*this, n));
    gv.back().setIndexInVector(sv.size() - 1);
    return gv.back();
    }
    
    GlGraphVar&
    GlSpace::newGraphVar(const Gecode::EdgesDesc& glb, const Gecode::EdgesDesc& lub)
    {
    gv.push_back(GeLisp::GlGraphVar(*this, glb, lub));
    gv.back().setIndexInVector(sv.size() - 1);
    return gv.back();
    }
  */
    
	
  const char*
  GlSpace::status(void) {
    Space& sp = static_cast<Space&>(*this);
    switch (sp.status()) {
    case SS_FAILED:
      return "SS_FAILED";
    case SS_SOLVED:
      return "SS_SOLVED";
    case SS_BRANCH:
      return "SS_BRANCH";
    }
    return "UNKNOWN";
  }

  void
  GlSpace::print(std::ostream& os) const {
    os << "Integer variables:" << std::endl;
    for (int i = 0; i < iv.size(); i++)
      os << "\tiv[" << i << "]: " << iv[i] << std::endl;
    os << "Bool variables:" << std::endl;
    for (int i = 0; i < bv.size(); i++)
      os << "\tbv[" << i << "]: " << bv[i] << std::endl;
    os << "Set variables:" << std::endl;
    for (int i = 0; i < sv.size(); i++)
      os << "\tsv[" << i << "]: " << sv[i] << std::endl;
    os << "Graph variables:" << std::endl;
    /*
      for (int i = 0; i < gv.size(); i++)
      os << "\tgv[" << i << "]: " << gv[i] << std::endl;
    */
  }
  
  const char*
  GlSpace::debug(void) const {
    std::stringstream os;
    print(os);
    return os.str().c_str();
  }
  // BRANCHING
  void
  GlSpace::branch(void) {
    std::cout << "will branch...." << std::endl;
    if (iv.size() > 0) {
      IntVarArgs ivArgs;
      for (int i =0; i < iv.size(); i++) {
	ivArgs<<iv[i];
      }
      Gecode::branch(*this,ivArgs,INT_VAR_SIZE_MIN, INT_VAL_MIN);
    }
    
    if (bv.size() > 0) {
      BoolVarArgs bvArgs;
      for (int i =0; i < bv.size(); i++) {
	bvArgs<<bv[i];
      }
      Gecode::branch(*this,bvArgs,INT_VAR_SIZE_MIN, INT_VAL_MIN);
    }

    if (sv.size() > 0) {
      SetVarArgs svArgs;
      for (int i =0; i < sv.size(); i++) {
	svArgs<<sv[i];
      }
      Gecode::branch(*this,svArgs,SET_VAR_NONE, SET_VAL_MIN_INC);
    }
    /*	
	if (gv.size() > 0) {
	for (int i =0; i < gv.size(); i++) {
	Gecode::branch(*this, gv[i], GRAPH_VAL_INC);
	std::cout << "A graph has been branched !!!" << std::endl;
	}
	}*/

  }
  
  template<template<class> class Engine>
  unsigned int
  GlSpace::runEngine(std::ostream& out, int nbSolutions, std::vector<GlSpace*>& solutionsVector) {
    std::cout << "will search...." << std::endl;
    using namespace std;
    Search::Options o;
    o.threads = 1;
    Engine<GlSpace> se(this,o);
    int noOfSolutions = nbSolutions;
    int findSol = noOfSolutions;
    GlSpace* sol = NULL;
    while (GlSpace* next_sol = se.next()) {
      std::cout << "A new solution found !!!" << std::endl;
      //delete sol;
      sol = next_sol;
      //std::cout << solutionsVector.size() << std::endl;
      solutionsVector.push_back(sol);
      //std::cout << solutionsVector.size() << std::endl;
      sol->print(out);
      out << "----------" << std::endl;
      if (--findSol==0)
        goto stopped;
    }
    //delete sol;
  stopped:
    Gecode::Search::Statistics stat = se.statistics();
    out << endl
        << "%%  solutions:     " 
        << std::abs(noOfSolutions - findSol) << endl
        << endl;
    out << std::endl << "No more solutions" << std::endl;
    std::cout << "search finished..." << std::endl;
    //std::cout << "sdfgdsg " << solutionsVector.size() << std::endl;
    return std::abs(noOfSolutions - findSol);
  }
    
  const char* GlSpace::run(int nbSolutions, std::vector<GlSpace*>& solutionsVector) {
    std::stringstream os;
    runEngine<DFS>(os,nbSolutions,solutionsVector);
    return os.str().c_str();
  }
    
  GlIntVar& GlSpace::getIntVar(int indexInVector)
  {
    return iv[indexInVector];
  }

  GlBoolVar& GlSpace::getBoolVar(int indexInVector)
  {
    return bv[indexInVector];
  }
	
  GlSetVar& GlSpace::getSetVar(int indexInVector)
  {
    return sv[indexInVector];
  }
  /*	
	GlGraphVar& GlSpace::getGraphVar(int indexInVector)
	{
	return gv[indexInVector];
	}
  */		
  //create a vector of GlSpace and return a reference on it
  std::vector<GlSpace*> GlSpace::createGlSpaceVector(void)
  {
    std::vector<GlSpace*> vec;
    return vec;
  }
    
  //gives access to an element of a vector of GlSpace at a given index
  GlSpace* GlSpace::getGlSpaceFromVector(std::vector<GlSpace*>& vec, int index)
  {
    if(index >= 0 && index < vec.size())
      return vec[index];
    else
      return NULL;
    		
  }
    
  //allow to check if a pointer to a GlSpace is NULL (1 if yes, 0 if not)
  /*int isNull(GlSpace* toCheck)
    {
    if(toCheck == NULL)
    return 1;
    else
    return 0;	
    }*/
    
    
    
  /***********************************************ADDITIONNAL FACILITIES*******************************************/
  /*
  std::vector<Gecode::Edge> createEdgeVector(void)
  {
    std::vector<Gecode::Edge> vec;
    return vec;
  }
    
  void addEdge(Gecode::Edge e, std::vector<Gecode::Edge> &vec)
  {
    vec.push_back(e);
  }
   
  Gecode::Edge* vecToArray(std::vector<Gecode::Edge> vec)
  {	
    Gecode::Edge* newArray = new Edge[vec.size()];
    for (int i =0; i < vec.size(); i++) 
      {
	newArray[i] = vec[i];
      }
    return newArray;
  }
  */
    
    
}
