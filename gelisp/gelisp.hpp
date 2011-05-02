#ifndef __GELISP_HPP__
#define __GELISP_HPP__


#include <iostream>
#include <vector>
#include <gecode/kernel.hh>
#include <gecode/int.hh>
#include <gecode/set.hh>

//#include <binrel/graph.hh>
//#include <binrel/branch.hh>
//#include <binrel/and.hh>
//#include <binrel/and.hpp>

namespace GeLisp {

  class GlIntVar;
  class GlBoolVar;
  class GlSetVar;
  //class GlGraphVar;
	
  class GlSpace : public Gecode::Space {
    GlSpace(bool share, GlSpace&);
  public:
    /// The integer variables
    std::vector<GlIntVar> iv;
    /// The Boolean variables
    std::vector<GlBoolVar> bv;
    /// The set variables
    std::vector<GlSetVar> sv;
    /// The graph variables
    //std::vector<GlGraphVar> gv;
    /// Construct an empty space
    GlSpace(void);
    /// Destructor
    ~GlSpace(void);
    

    /// Create a new integer variable with domain [lb,ub]
    GlIntVar& newIntVar(int lb, int ub);
    /// Create a new integer variable from IntSet is
    GlIntVar& newIntVar(const Gecode::IntSet &d);
    
    /// Create a new boolean variable with domain [lb,ub]
    GlBoolVar& newBoolVar(int lb, int ub);
    
    /// Create a new set variable from home
    GlSetVar& newSetVar();
    
    /// Create a new set variable with bounds and cardinality
    GlSetVar& newSetVar(int glbMin,int glbMax,int lubMin,int lubMax, 
			unsigned int cardMin = 0, unsigned int cardMax = Gecode::Set::Limits::card);
           
    /// Create a new set variable with bounds(lower bound is an IntSet) and cardinality
    GlSetVar& newSetVar(const Gecode::IntSet& glbD,int lubMin,int lubMax,
			unsigned int cardMin = 0,
			unsigned int cardMax = Gecode::Set::Limits::card);       
    
    /// Create a new set variable with bounds(upper bound is an IntSet) and cardinality
    GlSetVar&
    newSetVar(int glbMin,int glbMax,const Gecode::IntSet& lubD,
	      unsigned int cardMin = 0,
	      unsigned int cardMax = Gecode::Set::Limits::card);
           
    /// Create a new set variable with bounds(lower and upper bound is an IntSet) and cardinality       
    GlSetVar&
    newSetVar(const Gecode::IntSet& glbD,const Gecode::IntSet& lubD,
	      unsigned int cardMin = 0,
	      unsigned int cardMax = Gecode::Set::Limits::card);
	      
	/// Create a new graph variable with an empty lower bound and a full upper bound. n is the maximum value of a node.       
    //GlGraphVar&
    //newGraphVar(int n);
    /*
    /// Create a new graph variable from two edge descriptors
    GlGraphVar&
    newGraphVar(const Gecode::EdgesDesc& glb, const Gecode::EdgesDesc& lub);
    */
    //TODO : dernier constructeur de graphvar 
    //PAS OUBLIE DE RENOMMER LA DERNIERE FONCTION DE CONSTRUCTION DE GLGRAPH (si on en a besoin)
     
    
    /// Perform propagation
    const char* status(void);
    /// Print space information
    void print(std::ostream& os) const;
    /// Debug 
    const char* debug(void) const;
    /// Post the branchings
    void branch(void);
    /// Run the search for nbSolutions, returns the number of solutions. if nbSolutions = 0, search on all the space.
    template<template<class> class Engine>
    unsigned int runEngine(std::ostream& os, int nbSolutions, std::vector<GlSpace*>& solutionsVector);
    /// Returns the number of solutions to the problem
    const char* run(int nbSolutions, std::vector<GlSpace*>& solutionsVector);
    /// Implement optimization
    //virtual void constrain(const Space& s);
    /// Copy function
    virtual Gecode::Space* copy(bool share);
    
    //get the variable reference from the index of iv
    GlIntVar& getIntVar(int indexInVector);
    
    //get the variable reference from the index of bv
    GlBoolVar& getBoolVar(int indexInVector);
    
    //get the variable reference from the index of sv
    GlSetVar& getSetVar(int indexInVector);
    
    //get the variable reference from the index of gv
    //GlGraphVar& getGraphVar(int indexInVector);
    
    //create a vector of GlSpace and return a reference on it
    std::vector<GlSpace*> createGlSpaceVector(void);
    
    //gives access to an element of a vector of GlSpace at a given index
    GlSpace* getGlSpaceFromVector(std::vector<GlSpace*>& vec, int index);
    
    //allow to check if a pointer to a GlSpace is NULL (1 if yes, 0 if not)
    //int isNull(GlSpace* toCheck);
    
  };


  //classe utilisee a la place de IntVar car on a besoin de connaitre l'indice de l'objet dans le vecteur iv
  class GlIntVar: public Gecode::IntVar
  {
    int index; //index of the variable in iv vector (used to get the values solution after search)
  public:
    GlIntVar(void):Gecode::IntVar()
    {
      index = -1;
    }
		
    GlIntVar(GlSpace& home, int lb, int ub):Gecode::IntVar(home, lb, ub)
    {
      index = -1;
    }
    GlIntVar(GlSpace& home, const Gecode::IntSet &d):Gecode::IntVar(home, d)
    {
      index = -1;
    }
    int getIndexInVector(void)
    {
      return index;
    }
    void setIndexInVector(int newIndex)
    {
      index = newIndex;
    }
		
  };
	
	
  //classe utilisee a la place de BoolVar car on a besoin de connaitre l'indice de l'objet dans le vecteur bv
  class GlBoolVar: public Gecode::BoolVar
  {
    int index; //index of the variable in bv vector (used to get the values solution after search)
  public:
    GlBoolVar(void):Gecode::BoolVar()
    {
      index = -1;
    }
		
    GlBoolVar(GlSpace& home, int lb, int ub):Gecode::BoolVar(home, lb, ub)
    {
      index = -1;
    }
		
    int getIndexInVector(void)
    {
      return index;
    }
    void setIndexInVector(int newIndex)
    {
      index = newIndex;
    }
		
  };	
	

  //classe utilisee a la place de SetVar car on a besoin de connaitre l'indice de l'objet dans le vecteur sv
  class GlSetVar: public Gecode::SetVar
  {
    int index; //index of the variable in sv vector (used to get the values solution after search)
  public:
	
    GlSetVar(void):Gecode::SetVar()
    {
      index = -1;
    }
		
    GlSetVar(GlSpace& home):Gecode::SetVar(home)
    {
      index = -1;
    }
		
    GlSetVar(GlSpace& home, int glbMin,int glbMax,int lubMin,int lubMax, unsigned int cardMin = 0, unsigned int cardMax = Gecode::Set::Limits::card):Gecode::SetVar(home, glbMin, glbMax, lubMin, lubMax, cardMin, cardMax)
    {
      index = -1;
    }
		
    GlSetVar(GlSpace& home, const Gecode::IntSet& glbD,int lubMin,int lubMax, unsigned int cardMin = 0,
	     unsigned int cardMax = Gecode::Set::Limits::card):Gecode::SetVar(home, glbD, lubMin, lubMax, cardMin, cardMax)
    {
      index = -1;
    }
		
    GlSetVar(GlSpace& home, int glbMin,int glbMax,const Gecode::IntSet& lubD, unsigned int cardMin = 0, 
	     unsigned int cardMax = Gecode::Set::Limits::card):Gecode::SetVar(home, glbMin, glbMax, lubD, cardMin, cardMax)
    {
      index = -1	;
    }
		
    GlSetVar(GlSpace& home, const Gecode::IntSet& glbD,const Gecode::IntSet& lubD, unsigned int cardMin = 0,
	     unsigned int cardMax = Gecode::Set::Limits::card):Gecode::SetVar(home, glbD, lubD, cardMin, cardMax)
    {
      index = -1;
    }
		
    int getIndexInVector(void)
    {
      return index;
    }
    void setIndexInVector(int newIndex)
    {
      index = newIndex;
    }
		
  };	
  /*
  //classe utilisee a la place de GraphVar car on a besoin de connaitre l'indice de l'objet dans le vecteur gv
  class GlGraphVar: public Gecode::GraphVar
  {
    int index; //index of the variable in gv vector (used to get the values solution after search)
  public:
	GlGraphVar(void):Gecode::GraphVar()
    {
      index = -1;
    }
    GlGraphVar(GlSpace& home, int n): Gecode::GraphVar(home, n)
    {
      index = -1;
    }

    GlGraphVar(GlSpace& home, const Gecode::EdgesDesc& glb, const Gecode::EdgesDesc& lub): Gecode::GraphVar(home, glb, lub)
    {
      index = -1;	
    }
    
    GlGraphVar(GlSpace& home, const Gecode::Graph::EdgeSet& glb, const Gecode::Graph::EdgeSet& lub): GraphVar(home, glb, lub)
    {
      index = -1;	
    }
				
    int getIndexInVector(void)
    {
      return index;
    }
    void setIndexInVector(int newIndex)
    {
      index = newIndex;
    }
		
  };
  */
  class IntVarList {
    Gecode::IntVarArgs va;
    std::vector<int> vectorOfIndex; //will contain the indexes of the IntVar objects in the iv vector
  public:
    /*IntVarList(void)  
      {
    	
      }*/
    IntVarList(GlSpace& home, int n, int min, int max)  {
      //va = new Gecode::IntVarArgs(0);
      GlIntVar* tempVar;
      for (int i = 0; i < n; i++) {
	tempVar = &(home.newIntVar(min,max));
	va << *tempVar;
	vectorOfIndex.push_back(tempVar->getIndexInVector());
      }
    }
    IntVarList(GlSpace& home, int n, const Gecode::IntSet &d){
      for (int i = 0; i < n; i++) {
	va << home.newIntVar(d);
      }
    }  
    
    unsigned int size(void) {
      return va.size();
    } 
    void add(/*const*/ GlIntVar& x) {
      va << x;
      vectorOfIndex.push_back(x.getIndexInVector());
    }
    const Gecode::IntVarArgs& get(void) {
      return va;
    }

    const GlIntVar& getVar(int i) {
      assert(i >= 0 && i < va.size());
      GlIntVar& toReturn = ((GlIntVar&) va[i]) ;
      return toReturn;
    }
    
    //return the index in iv of an IntVar in va
    const int getIndexOfAVarInVector(int i)
    {
      return vectorOfIndex[i];
    }
  };
	
	
	
  class BoolVarList {
    Gecode::BoolVarArgs ba;
    std::vector<int> vectorOfIndex; //will contain the indexes of the BoolVar objects in the bv vector
  public:
    //BoolVarList(void)  {}
    BoolVarList(GlSpace& home, int n, int min, int max)  {    	
      GlBoolVar* tempVar;
      for (int i = 0; i < n; i++) {
	tempVar = &(home.newBoolVar(min,max));
	ba << *tempVar;
	vectorOfIndex.push_back(tempVar->getIndexInVector());
      }
    }
    unsigned int size(void) {
      return ba.size();
    } 
    void add(/*const*/ GlBoolVar& x) {
      ba << x;
      vectorOfIndex.push_back(x.getIndexInVector());
    }
    const Gecode::BoolVarArgs& get(void) {
      return ba;
    }

    const GlBoolVar& getVar(int i) {
      assert(i >= 0 && i < ba.size());
      GlBoolVar& toReturn = ((GlBoolVar&) ba[i]) ;
      return toReturn;

    }
    
    //return the index in bv of an IntVar in ba
    const int getIndexOfAVarInVector(int i)
    {
      return vectorOfIndex[i];
    }

  };

	
	
  class SetVarList {
    Gecode::SetVarArgs sa;
    std::vector<int> vectorOfIndex; //will contain the indexes of the SetVar objects in the sv vector
  public:
  
    //CONSTRUCTORS
  	
    SetVarList(GlSpace& home, int n) 
    {   	
      GlSetVar* tempVar;
      for (int i = 0; i < n; i++) 
    	{
	  tempVar = &(home.newSetVar());
	  sa << *tempVar;
	  vectorOfIndex.push_back(tempVar->getIndexInVector());
     	}
    }
    
    SetVarList(GlSpace& home, int n, int glbMin,int glbMax,int lubMin,int lubMax, 
	       unsigned int cardMin = 0, unsigned int cardMax = Gecode::Set::Limits::card)  
    {
      GlSetVar* tempVar;
      for (int i = 0; i < n; i++) 
    	{
	  tempVar = &(home.newSetVar(glbMin, glbMax, lubMin, lubMax, cardMin, cardMax));
	  sa << *tempVar;
	  vectorOfIndex.push_back(tempVar->getIndexInVector());
      	}
    }
    
    SetVarList(GlSpace& home, int n, const Gecode::IntSet& glbD, int lubMin,int lubMax, 
	       unsigned int cardMin = 0, unsigned int cardMax = Gecode::Set::Limits::card)  
    {
      GlSetVar* tempVar;
      for (int i = 0; i < n; i++) 
      	{
	  tempVar = &(home.newSetVar(glbD, lubMin, lubMax, cardMin, cardMax));
	  sa << *tempVar;
	  vectorOfIndex.push_back(tempVar->getIndexInVector());
      	}
    }
    
    SetVarList(GlSpace& home, int n, int glbMin,int glbMax, const Gecode::IntSet& lubD, 
	       unsigned int cardMin = 0, unsigned int cardMax = Gecode::Set::Limits::card)  
    {
      GlSetVar* tempVar;
      for (int i = 0; i < n; i++) 
      	{
	  tempVar = &(home.newSetVar(glbMin, glbMax, lubD, cardMin, cardMax));
	  sa << *tempVar;
	  vectorOfIndex.push_back(tempVar->getIndexInVector());
    	}
    }
    
    SetVarList(GlSpace& home, int n, const Gecode::IntSet& glbD, const Gecode::IntSet& lubD, 
	       unsigned int cardMin = 0, unsigned int cardMax = Gecode::Set::Limits::card)  
    {
      GlSetVar* tempVar;
      for (int i = 0; i < n; i++) 
      	{
	  tempVar = &(home.newSetVar(glbD, lubD, cardMin, cardMax));
	  sa << *tempVar;
	  vectorOfIndex.push_back(tempVar->getIndexInVector());
      	}
    }
    
    //METHODS
    unsigned int size(void) {
      return sa.size();
    } 
    void add(/*const*/ GlSetVar& x) {
      sa << x;
      vectorOfIndex.push_back(x.getIndexInVector());
    }
    const Gecode::SetVarArgs& get(void) {
      return sa;
    }

    const Gecode::SetVar& getVar(int i) {
      assert(i >= 0 && i < sa.size());
      GlSetVar& toReturn = ((GlSetVar&) sa[i]) ;
      return toReturn;
    }
    
    //return the index in sv of an IntVar in sa
    const int getIndexOfAVarInVector(int i)
    {
      return vectorOfIndex[i];
    }

  };
	
	
  class IntList {
		
  private:
    Gecode::IntArgs va;
  public:
	
    IntList(int n):va(n){}
		
    IntList(int n, int defaultValue):va(n)
    {
      for(int i = 0; i < n; i++)
	{
	  va[i] = defaultValue;
				
	}
			
    }
		
    IntList(int n,int start, int inc):va(n)
    {
      for(int i = 0; i < n; i++, start+=inc)
	{
	  va[i] = start;
				
	}
    }
    const Gecode::IntArgs& get(void) {
      return va;
    }
		
    unsigned int size(void) {
      return va.size();
    } 

    const int getVar(int i) {
      assert(i >= 0 && i < va.size());
      return va[i];
      	
    }
    const void setVar(int i, int value) {
      assert(i >= 0 && i < va.size());
      va[i] = value ;
    }
    	
    //doesn't work when wrapped to lisp
    const char* print(){
    		
      std::stringstream out;
      out << "[ " ;
      for(int i = 0; i < va.size() - 1; i++)
	{
	  out << va[i] << ", " ;
	}
      out << va[va.size() - 1] << "]" << std::endl;
      return out.str().c_str();
    }

	
  };
	
  class IntSetList {
		
  private:
    Gecode::IntSetArgs isa;
  public:
    IntSetList(int n,int min, int max):isa(n)
    {
      for(int i = 0; i < n; i++)
	{
	  isa[i] = Gecode::IntSet(min,max);
				
	}
    }
    const Gecode::IntSetArgs& get(void) {
      return isa;
    }
		
    const Gecode::IntSet& getElement(int i) {
      return isa[i];
    }
		
	
	
  };
	 
	 
	/***********************************************ADDITIONNAL FACILITIES*******************************************/
	
  			
  	//create a vector of edges and return a reference on it
  //std::vector<Gecode::Edge> createEdgeVector(void);
    
    //add an edge to a vector of edges
    //void addEdge(Gecode::Edge e, std::vector<Gecode::Edge> &vec);
    
    //create an array of edges from a vector of edges
  //Gecode::Edge* vecToArray(std::vector<Gecode::Edge> vec);
}
#endif
