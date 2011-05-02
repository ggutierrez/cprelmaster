%module gelisp



%ignore iv;
%ignore bv;
%ignore sv;

%typemap(cout) bool ":boolean";

//renaming must be done for the method with optional argument first or the renaming is not well done.

/****************************************Renaming of the gelisp.hh part ******************/

//CLASS GLSPACE


/* %rename(newIntVar_minmax) newIntVar(int lb, int ub); */
/* %rename(newIntVar_fromset) newIntVar(const Gecode::IntSet &d);//does not rename? */

/* %rename(newSetVar_home) newSetVar(); */

/* %rename(newSetVar_minmax_minmax_2option) newSetVar(int glbMin,int glbMax,int lubMin,int lubMax,  */
/*     unsigned int cardMin = 0, unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(newSetVar_minmax_minmax_1option) newSetVar(int glbMin,int glbMax,int lubMin,int lubMax,  */
/*     unsigned int cardMin = 0); */
/* %rename(newSetVar_minmax_minmax_default) newSetVar(int glbMin,int glbMax,int lubMin,int lubMax); */

/* %rename(newSetVar_set_minmax_2option) newSetVar(const Gecode::IntSet& glbD,int lubMin,int lubMax, */
/*            								unsigned int cardMin = 0, */
/* 							           unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(newSetVar_set_minmax_1option) newSetVar(const Gecode::IntSet& glbD,int lubMin,int lubMax, */
/*            								unsigned int cardMin = 0);        */
/* %rename(newSetVar_set_minmax_default) newSetVar(const Gecode::IntSet& glbD,int lubMin,int lubMax);        */
							           
/* %rename(newSetVar_minmax_set_2option) newSetVar(int glbMin,int glbMax,const Gecode::IntSet& lubD, */
/*            unsigned int cardMin = 0, */
/*            unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(newSetVar_minmax_set_1option) newSetVar(int glbMin,int glbMax,const Gecode::IntSet& lubD, */
/*            unsigned int cardMin = 0); */
/* %rename(newSetVar_minmax_set_default) newSetVar(int glbMin,int glbMax,const Gecode::IntSet& lubD); */

/* %rename(newSetVar_set_set_2option) newSetVar(const Gecode::IntSet& glbD,const Gecode::IntSet& lubD, */
/*            unsigned int cardMin = 0, */
/*            unsigned int cardMax = Gecode::Set::Limits::card); */
           
/* %rename(newSetVar_set_set_1option) newSetVar(const Gecode::IntSet& glbD,const Gecode::IntSet& lubD, */
/*            unsigned int cardMin = 0); */
/* %rename(newSetVar_set_set_default) newSetVar(const Gecode::IntSet& glbD,const Gecode::IntSet& lubD); */

//%rename(newGraphVar_int) newGraphVar(int n);

//%rename(newGraphVar_EdgesDesc) newGraphVar(const Gecode::EdgesDesc& glb, const Gecode::EdgesDesc& lub);

//CLASS GLINTVAR

/* %rename(GlIntVar_default) GlIntVar(void); //SHOULDN'T BE USED FROM LISPWORKS */
/* %rename(GlIntVar_minmax) GlIntVar(GlSpace& home, int lb, int ub); */
/* %rename(GlIntVar_set) GlIntVar(GlSpace& home, const Gecode::IntSet &d); */


//CLASS GLBOOLVAR
/* %rename(GlBoolVar_default) GlBoolVar(void); //SHOULDN'T BE USED FROM LISPWORKS */
/* %rename(GlBoolVar_minmax) GlBoolVar(GlSpace& home, int lb, int ub); */

/* //CLASS GLSETVAR */
/* %rename(GlSetVar_default) GlSetVar(void); //SHOULDN'T BE USED FROM LISPWORKS */

/* %rename(GlSetVar_home) GlSetVar(GlSpace& home); */

/* %rename(GlSetVar_minmax_minmax_2option) GlSetVar(GlSpace& home, int glbMin,int glbMax,int lubMin,int lubMax, unsigned int cardMin = 0, unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(GlSetVar_minmax_minmax_1option) GlSetVar(GlSpace& home, int glbMin,int glbMax,int lubMin,int lubMax, unsigned int cardMin = 0); */
/* %rename(GlSetVar_minmax_minmax_default) GlSetVar(GlSpace& home, int glbMin,int glbMax,int lubMin,int lubMax); */

/* %rename(GlSetVar_set_minmax_2option) GlSetVar(GlSpace& home, const Gecode::IntSet& glbD,int lubMin,int lubMax, unsigned int cardMin = 0, */
/* 		unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(GlSetVar_set_minmax_1option) GlSetVar(GlSpace& home, const Gecode::IntSet& glbD,int lubMin,int lubMax, unsigned int cardMin = 0); */
/* %rename(GlSetVar_set_minmax_default) GlSetVar(GlSpace& home, const Gecode::IntSet& glbD,int lubMin,int lubMax); */

/* %rename(GlSetVar_minmax_set_2option) GlSetVar(GlSpace& home, int glbMin,int glbMax,const Gecode::IntSet& lubD, unsigned int cardMin = 0,  */
/* 		unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(GlSetVar_minmax_set_1option) GlSetVar(GlSpace& home, int glbMin,int glbMax,const Gecode::IntSet& lubD, unsigned int cardMin = 0); */
/* %rename(GlSetVar_minmax_set_default) GlSetVar(GlSpace& home, int glbMin,int glbMax,const Gecode::IntSet& lubD); */

/* %rename(GlSetVar_set_set_2option) GlSetVar(GlSpace& home, const Gecode::IntSet& glbD,const Gecode::IntSet& lubD, unsigned int cardMin = 0, */
/*         unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(GlSetVar_set_set_1option) GlSetVar(GlSpace& home, const Gecode::IntSet& glbD,const Gecode::IntSet& lubD, unsigned int cardMin = 0); */
/* %rename(GlSetVar_set_set_default) GlSetVar(GlSpace& home, const Gecode::IntSet& glbD,const Gecode::IntSet& lubD); */

/* //CLASS GLGRAPHVAR */
/* //%rename(GlGraphVar_default) GlGraphVar(void); //SHOULDN'T BE USED FROM LISPWORKS */

/* //%rename(GlGraphVar_int) GlGraphVar(GlSpace& home, int n); */

/* //%rename(GlGraphVar_EdgesDesc) GlGraphVar(GlSpace& home, const Gecode::EdgesDesc& glb, const Gecode::EdgesDesc& lub); */

/* //%rename(GlGraphVar_EdgeSet) GlGraphVar(GlSpace& home, const Gecode::Graph::EdgeSet& glb, const Gecode::Graph::EdgeSet& lub); */

/* //CLASS INTVARLIST */

/* //%rename(newIntVar_empty) IntVarList(void); */
/* %rename(IntVarList_minmax) IntVarList(GlSpace& home, int n, int min, int max); */
/* %rename(IntVarList_set) IntVarList(GlSpace& home, int n, const Gecode::IntSet &d); */

/* //CLASS BOOLVARLIST */

/* %rename(BoolVarList_minmax) BoolVarList(GlSpace& home, int n, int min, int max); */

/* //CLASS SETVARLIST */

/* %rename(SetVarList_home) SetVarList(GlSpace& home, int n); */

/* %rename(SetVarList_minmax_minmax_2option) SetVarList(GlSpace& home, int n, int glbMin,int glbMax,int lubMin,int lubMax,  */
/*     unsigned int cardMin = 0, unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(SetVarList_minmax_minmax_1option) SetVarList(GlSpace& home, int n, int glbMin,int glbMax,int lubMin,int lubMax,  */
/*     unsigned int cardMin = 0); */
/* %rename(SetVarList_minmax_minmax_default) SetVarList(GlSpace& home, int n, int glbMin,int glbMax,int lubMin,int lubMax); */

/* %rename(SetVarList_set_minmax_2option) SetVarList(GlSpace& home, int n, const Gecode::IntSet& glbD,int lubMin,int lubMax, */
/*            								unsigned int cardMin = 0, */
/* 							           unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(SetVarList_set_minmax_1option) SetVarList(GlSpace& home, int n,const Gecode::IntSet& glbD,int lubMin,int lubMax, */
/*            								unsigned int cardMin = 0);        */
/* %rename(SetVarList_set_minmax_default) SetVarList(GlSpace& home, int n, const Gecode::IntSet& glbD,int lubMin,int lubMax);        */
							           
/* %rename(SetVarList_minmax_set_2option) SetVarList(GlSpace& home, int n, int glbMin,int glbMax,const Gecode::IntSet& lubD, */
/*            unsigned int cardMin = 0, */
/*            unsigned int cardMax = Gecode::Set::Limits::card); */
/* %rename(SetVarList_minmax_set_1option) SetVarList(GlSpace& home, int n, int glbMin,int glbMax,const Gecode::IntSet& lubD, */
/*            unsigned int cardMin = 0); */
/* %rename(SetVarList_minmax_set_default) SetVarList(GlSpace& home, int n, int glbMin,int glbMax,const Gecode::IntSet& lubD); */

/* %rename(SetVarList_set_set_2option) SetVarList(GlSpace& home, int n, const Gecode::IntSet& glbD,const Gecode::IntSet& lubD, */
/*            unsigned int cardMin = 0, */
/*            unsigned int cardMax = Gecode::Set::Limits::card); */
           
/* %rename(SetVarList_set_set_1option) SetVarList(GlSpace& home, int n, const Gecode::IntSet& glbD,const Gecode::IntSet& lubD, */
/*            unsigned int cardMin = 0); */
/* %rename(SetVarList_set_set_default) SetVarList(GlSpace& home, int n, const Gecode::IntSet& glbD,const Gecode::IntSet& lubD); */

/* //CLASS INTLIST */

/* %rename(IntList_size) IntList(int n); */
/* %rename(IntList_sizeAndDefaultValue) IntList(int n, int defaultValue); */
/* %rename(IntList_sizeAndStartAndInc) IntList(int n, int start, int inc); */

/* //CLASS INTSET */

/* %rename(IntSet_minmax) IntSet(int n, int m);; */
/* %rename(IntSet_n_element_of_array) IntSet(const int r[],   int n); */
/* %rename(IntSet_n_element_of_array_minmax) IntSet(const int r[][2], int n); */

/* %rename(min_index) min(int i) const; */
/* %rename(max_index) max(int i) const; */
/* %rename(width_index) width(int i) const; */


/* /\****************************************Renaming of the int.hh part ******************\/ */


/* //DOM CONSTRAINTS */

/* %rename(dom_onIntVar_option) dom(Space& home, IntVar x, int n, IntConLevel icl=ICL_DEF); */
/* %rename(dom_onIntVar_default) dom(Space& home, IntVar x, int n); */

/* %rename(dom_onIVA_option) dom(Space& home, const IntVarArgs& x, int n, IntConLevel icl=ICL_DEF); */
/* %rename(dom_onIVA_default) dom(Space& home, const IntVarArgs& x, int n); */

/* %rename(dom_onIntVarWithBounds_option) dom(Space& home, IntVar x, int l, int m, IntConLevel icl=ICL_DEF); */
/* %rename(dom_onIntVarWithBounds_default) dom(Space& home, IntVar x, int l, int m); */

/* %rename(dom_onIVAWithBounds_option) dom(Space& home, const IntVarArgs& x, int l, int m, IntConLevel icl=ICL_DEF); */
/* %rename(dom_onIVAWithBounds_default) dom(Space& home, const IntVarArgs& x, int l, int m); */

/* %rename(dom_onIntVarAndIntSet_option) dom(Space& home, IntVar x, const IntSet& s, IntConLevel icl=ICL_DEF); */
/* %rename(dom_onIntVarAndIntSet_default) dom(Space& home, IntVar x, const IntSet& s); */

/* %rename(dom_onIVAAndIntSet_option) dom(Space& home, const IntVarArgs& x, const IntSet& s, IntConLevel icl=ICL_DEF); */
/* %rename(dom_onIVAAndIntSet_default) dom(Space& home, const IntVarArgs& x, const IntSet& s); */

/* %rename(dom_onIntVarAndIntAndBoolVar_option) dom(Space& home, IntVar x, int n, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(dom_onIntVarAndIntAndBoolVar_default) dom(Space& home, IntVar x, int n, BoolVar b); */

/* %rename(dom_onIntVarAndBoolVarWithBounds_option) dom(Space& home, IntVar x, int l, int m, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(dom_onIntVarAndBoolVarWithBounds_default) dom(Space& home, IntVar x, int l, int m, BoolVar b); */

/* %rename(dom_onIntVarAndIntSetAndBoolVarWithBounds_option) dom(Space& home, IntVar x, const IntSet& s, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(dom_onIntVarAndIntSetAndBoolVarWithBounds_default) dom(Space& home, IntVar x, const IntSet& s, BoolVar b); */

/* //REL CONSTRAINTS */

/* %rename(rel_onIntVar_option) rel(Space& home, IntVar x0, IntRelType r, IntVar x1, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onIntVar_default) rel(Space& home, IntVar x0, IntRelType r, IntVar x1); */

/* %rename(rel_onIntVarAndIVA_option) rel(Space& home, const IntVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onIntVarAndIVA_default) rel(Space& home, const IntVarArgs& x, IntRelType r, IntVar y); */

/* %rename(rel_onIntVarAndInt_option) rel(Space& home, IntVar x, IntRelType r, int c, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onIntVarAndInt_default) rel(Space& home, IntVar x, IntRelType r, int c); */

/* %rename(rel_onIVAAndInt_option) rel(Space& home, const IntVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onIVAAndInt_default) rel(Space& home, const IntVarArgs& x, IntRelType r, int c); */

/* %rename(rel_on2IntVarAndBoolVar_option) rel(Space& home, IntVar x0, IntRelType r, IntVar x1, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(rel_on2IntVarAndBoolVar_default) rel(Space& home, IntVar x0, IntRelType r, IntVar x1, BoolVar b); */

/* %rename(rel_onIntVarAndIntAndBoolVar_option) rel(Space& home, IntVar x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onIntVarAndIntAndBoolVar_default) rel(Space& home, IntVar x, IntRelType r, int c, BoolVar b); */

/* %rename(rel_onIVA_option) rel(Space& home, const IntVarArgs& x, IntRelType r, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onIVA_default) rel(Space& home, const IntVarArgs& x, IntRelType r); */

/* %rename(rel_onIVAAndIVA_option) rel(Space& home, const IntVarArgs& x, IntRelType r, const IntVarArgs& y, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onIVAAndIVA_default) rel(Space& home, const IntVarArgs& x, IntRelType r, const IntVarArgs& y); */

/* %rename(rel_on2BoolVar_option) rel(Space& home, BoolVar x0, IntRelType r, BoolVar x1, IntConLevel icl=ICL_DEF); */
/* %rename(rel_on2BoolVar_default) rel(Space& home, BoolVar x0, IntRelType r, BoolVar x1); */

/* %rename(rel_on3BoolVar_option) rel(Space& home, BoolVar x0, IntRelType r, BoolVar x1, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(rel_on3BoolVar_default) rel(Space& home, BoolVar x0, IntRelType r, BoolVar x1, BoolVar b); */

/* %rename(rel_onBVAAndBoolVar_option) rel(Space& home, const BoolVarArgs& x, IntRelType r, BoolVar y, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onBVAAndBoolVar_default) rel(Space& home, const BoolVarArgs& x, IntRelType r, BoolVar y); */

/* %rename(rel_onBoolVarAndInt_option) rel(Space& home, BoolVar x, IntRelType r, int n, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onBoolVarAndInt_default) rel(Space& home, BoolVar x, IntRelType r, int n); */

/* %rename(rel_on2BoolVarAndInt_option) rel(Space& home, BoolVar x, IntRelType r, int n, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(rel_on2BoolVarAndInt_default) rel(Space& home, BoolVar x, IntRelType r, int n, BoolVar b); */

/* %rename(rel_onBVAAndInt_option) rel(Space& home, const BoolVarArgs& x, IntRelType r, int n, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onBVAAndInt_default) rel(Space& home, const BoolVarArgs& x, IntRelType r, int n); */

/* %rename(rel_on2BVA_option) rel(Space& home, const BoolVarArgs& x, IntRelType r, const BoolVarArgs& y, IntConLevel icl=ICL_DEF); */
/* %rename(rel_on2BVA_default) rel(Space& home, const BoolVarArgs& x, IntRelType r, const BoolVarArgs& y); */

/* %rename(rel_onBVA_option) rel(Space& home, const BoolVarArgs& x, IntRelType r, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onBVA_default) rel(Space& home, const BoolVarArgs& x, IntRelType r); */

/* %rename(rel_on3BoolVarWithBoolOpType_option) rel(Space& home, BoolVar x0, BoolOpType o, BoolVar x1, BoolVar x2, IntConLevel icl=ICL_DEF); */
/* %rename(rel_on3BoolVarWithBoolOpType_default) rel(Space& home, BoolVar x0, BoolOpType o, BoolVar x1, BoolVar x2); */

/* %rename(rel_on2BoolVarAndIntWithBoolOpType_option) rel(Space& home, BoolVar x0, BoolOpType o, BoolVar x1, int n, IntConLevel icl=ICL_DEF); */
/* %rename(rel_on2BoolVarAndIntWithBoolOpType_default) rel(Space& home, BoolVar x0, BoolOpType o, BoolVar x1, int n); */

/* %rename(rel_onBVAAndBoolVarWithBoolOpType_option) rel(Space& home, BoolOpType o, const BoolVarArgs& x, BoolVar y, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onBVAAndBoolVarWithBoolOpType_default) rel(Space& home, BoolOpType o, const BoolVarArgs& x, BoolVar y); */

/* %rename(rel_onBVAAndIntWithBoolOpType_option) rel(Space& home, BoolOpType o, const BoolVarArgs& x, int n, IntConLevel icl=ICL_DEF); */
/* %rename(rel_onBVAAndIntWithBoolOpType_default) rel(Space& home, BoolOpType o, const BoolVarArgs& x, int n); */

/* //CLAUSE CONSTRAINTS */

/* %rename(clause_onBoolVar_option) clause(Space& home, BoolOpType o, const BoolVarArgs& x, const BoolVarArgs& y, BoolVar z, IntConLevel icl=ICL_DEF); */
/* %rename(clause_onBoolVar_default) clause(Space& home, BoolOpType o, const BoolVarArgs& x, const BoolVarArgs& y, BoolVar z); */

/* %rename(clause_onInt_option) clause(Space& home, BoolOpType o, const BoolVarArgs& x, const BoolVarArgs& y, int n, IntConLevel icl=ICL_DEF); */
/* %rename(clause_onInt_default) clause(Space& home, BoolOpType o, const BoolVarArgs& x, const BoolVarArgs& y, int n); */

/* //ELEMENT CONSTRAINTS */

/* %rename(element_onIVAAnd2IntVar_option) element(Space& home, const IntVarArgs& x, IntVar y0, IntVar y1, IntConLevel icl=ICL_DEF); */
/* %rename(element_onIVAAnd2IntVar_default) element(Space& home, const IntVarArgs& x, IntVar y0, IntVar y1); */

/* %rename(element_onIVAAndIntVarAndInt_option) element(Space& home, const IntVarArgs& x, IntVar y0, int y1, IntConLevel icl=ICL_DEF); */
/* %rename(element_onIVAAndIntVarAndInt_default) element(Space& home, const IntVarArgs& x, IntVar y0, int y1); */

/* %rename(element_onIVAAnd3IntVarAnd2Int_option) element(Space& home, const IntVarArgs& a, IntVar x, int w, IntVar y, int h, IntVar z, IntConLevel icl=ICL_DEF); */
/* %rename(element_onIVAAnd3IntVarAnd2Int_default) element(Space& home, const IntVarArgs& a, IntVar x, int w, IntVar y, int h, IntVar z); */

/* %rename(element_onBVAAndIntVarAndBoolVar_option) element(Space& home, const BoolVarArgs& x, IntVar y0, BoolVar y1, IntConLevel icl=ICL_DEF); */
/* %rename(element_onBVAAndIntVarAndBoolVar_default) element(Space& home, const BoolVarArgs& x, IntVar y0, BoolVar y1); */

/* %rename(element_onBVAAndIntVarAndInt_option) element(Space& home, const BoolVarArgs& x, IntVar y0, int y1, IntConLevel icl=ICL_DEF); */
/* %rename(element_onBVAAndIntVarAndInt_default) element(Space& home, const BoolVarArgs& x, IntVar y0, int y1); */

/* %rename(element_onBVAAnd2IntVarAnd2IntAndBoolVar_option) element(Space& home, const BoolVarArgs& a, IntVar x, int w, IntVar y, int h, BoolVar z, IntConLevel icl=ICL_DEF); */
/* %rename(element_onBVAAnd2IntVarAnd2IntAndBoolVar_default) element(Space& home, const BoolVarArgs& a, IntVar x, int w, IntVar y, int h, BoolVar z); */

/* //DISTINCT CONSTRAINTS */

/* %rename(distinct_option) distinct(Space& home, const IntVarArgs& x, IntConLevel icl=ICL_DEF); */
/* %rename(distinct_default) distinct(Space& home, const IntVarArgs& x); */

/* %rename(distinct_constant_added_to_var_option) distinct(Space& home, const IntArgs& n, const IntVarArgs& x, IntConLevel icl=ICL_DEF); */
/* %rename(distinct_constant_added_to_var_default) distinct(Space& home, const IntArgs& n, const IntVarArgs& x); */

/* //CHANNEL CONSTRAINTS */

/* %rename(channel_onIVAAndIVA_option) channel(Space& home, const IntVarArgs& x, const IntVarArgs& y, IntConLevel icl=ICL_DEF); */
/* %rename(channel_onIVAAndIVA_default) channel(Space& home, const IntVarArgs& x, const IntVarArgs& y); */

/* %rename(channel_on2IVAAnd2IntVar_option) channel(Space& home, const IntVarArgs& x, int xoff, const IntVarArgs& y, int yoff, IntConLevel icl=ICL_DEF); */
/* %rename(channel_on2IVAAnd2IntVar_default) channel(Space& home, const IntVarArgs& x, int xoff, const IntVarArgs& y, int yoff); */

/* %rename(channel_onBoolVarAndIntVar_option) channel(Space& home, BoolVar x0, IntVar x1, IntConLevel icl=ICL_DEF); */
/* %rename(channel_onBoolVarAndIntVar_default) channel(Space& home, BoolVar x0, IntVar x1); */

/* %rename(channel_onIntVarAndBoolVar_option) channel(Space& home, IntVar x0, BoolVar x1, IntConLevel icl=ICL_DEF); */
/* %rename(channel_onIntVarAndBoolVar_default) channel(Space& home, IntVar x0, BoolVar x1); */

/* %rename(channel_onBVAAndIntVar_2option) channel(Space& home, const BoolVarArgs& x, IntVar y, int o=0, IntConLevel icl=ICL_DEF); */
/* %rename(channel_onBVAAndIntVar_1option) channel(Space& home, const BoolVarArgs& x, IntVar y, int o=0); */
/* %rename(channel_onBVAAndIntVar_default) channel(Space& home, const BoolVarArgs& x, IntVar y); */

/* //SORTED CONSTRAINTS */

/* %rename(sorted_onIVAAndIVA_option) sorted(Space& home, const IntVarArgs& x, const IntVarArgs& y, IntConLevel icl=ICL_DEF); */
/* %rename(sorted_onIVAAndIVA_default) sorted(Space& home, const IntVarArgs& x, const IntVarArgs& y); */

/* %rename(sorted_on3IVA_option) sorted(Space& home, const IntVarArgs& x, const IntVarArgs& y, const IntVarArgs& z, IntConLevel icl=ICL_DEF); */
/* %rename(sorted_on3IVA_default) sorted(Space& home, const IntVarArgs& x, const IntVarArgs& y, const IntVarArgs& z); */

/* //COUNT CONSTRAINTS */

/* %rename(count_onIVAAnd2Int_option) count(Space& home, const IntVarArgs& x, int n, IntRelType r, int m, IntConLevel icl=ICL_DEF); */
/* %rename(count_onIVAAnd2Int_default) count(Space& home, const IntVarArgs& x, int n, IntRelType r, int m); */

/* %rename(count_onIVAAndIntVarAndInt_option) count(Space& home, const IntVarArgs& x, IntVar y, IntRelType r, int m, IntConLevel icl=ICL_DEF); */
/* %rename(count_onIVAAndIntVarAndInt_default) count(Space& home, const IntVarArgs& x, IntVar y, IntRelType r, int m); */

/* %rename(count_onIVAAndIntArgsAndInt_option) count(Space& home, const IntVarArgs& x, const IntArgs& y, IntRelType r, int m, IntConLevel icl=ICL_DEF); */
/* %rename(count_onIVAAndIntArgsAndInt_default) count(Space& home, const IntVarArgs& x, const IntArgs& y, IntRelType r, int m); */

/* %rename(count_onIVAAndIntAndIntVar_option) count(Space& home, const IntVarArgs& x, int n, IntRelType r, IntVar z, IntConLevel icl=ICL_DEF); */
/* %rename(count_onIVAAndIntAndIntVar_default) count(Space& home, const IntVarArgs& x, int n, IntRelType r, IntVar z); */

/* %rename(count_onIVAAnd2IntVar_option) count(Space& home, const IntVarArgs& x, IntVar y, IntRelType r, IntVar z, IntConLevel icl=ICL_DEF); */
/* %rename(count_onIVAAnd2IntVar_default) count(Space& home, const IntVarArgs& x, IntVar y, IntRelType r, IntVar z); */

/* %rename(count_onIVAAndIntArgsAndIntVar_option) count(Space& home, const IntVarArgs& x, const IntArgs& y, IntRelType r, IntVar z, IntConLevel icl=ICL_DEF); */
/* %rename(count_onIVAAndIntArgsAndIntVar_default) count(Space& home, const IntVarArgs& x, const IntArgs& y, IntRelType r, IntVar z); */

/* %rename(count_on2IVA_option) count(Space& home, const IntVarArgs& x, const IntVarArgs& c, IntConLevel icl=ICL_DEF); */
/* %rename(count_on2IVA_default) count(Space& home, const IntVarArgs& x, const IntVarArgs& c); */

/* %rename(count_onIVAAndISA_option) count(Space& home, const IntVarArgs& x, const IntSetArgs& c, IntConLevel icl=ICL_DEF); */
/* %rename(count_onIVAAndISA_default) count(Space& home, const IntVarArgs& x, const IntSetArgs& c); */

/* %rename(count_on2IVAAndIntArgs_option) count(Space& home, const IntVarArgs& x, const IntVarArgs& c, const IntArgs& v, IntConLevel icl=ICL_DEF); */
/* %rename(count_on2IVAAndIntArgs_default) count(Space& home, const IntVarArgs& x, const IntVarArgs& c, const IntArgs& v); */

/* %rename(count_onIVAAndISAIntArgs_option) count(Space& home, const IntVarArgs& x, const IntSetArgs& c, const IntArgs& v, IntConLevel icl=ICL_DEF); */
/* %rename(count_onIVAAndISAIntArgs_default) count(Space& home, const IntVarArgs& x, const IntSetArgs& c, const IntArgs& v); */

/* %rename(count_onIVAAndIntSetIntArgs_option) count(Space& home, const IntVarArgs& x, const IntSet& c, const IntArgs& v, IntConLevel icl=ICL_DEF); */
/* %rename(count_onIVAAndIntSetIntArgs_default) count(Space& home, const IntVarArgs& x, const IntSet& c, const IntArgs& v); */

/* //SEQUENCE CONSTRAINTS */

/* %rename(sequence_IVA_option) sequence(Space& home, const IntVarArgs& x, const IntSet& s, int q, int l, int u, IntConLevel icl=ICL_DEF); */
/* %rename(sequence_IVA_default) sequence(Space& home, const IntVarArgs& x, const IntSet& s, int q, int l, int u); */

/* %rename(sequence_BVA_option) sequence(Space& home, const BoolVarArgs& x, const IntSet& s, int q, int l, int u, IntConLevel icl=ICL_DEF); */
/* %rename(sequence_BVA_default) sequence(Space& home, const BoolVarArgs& x, const IntSet& s, int q, int l, int u); */

/* //MIN CONSTRAINTS */

/* %rename(min_on3IntVar_option) min(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF); */
/* %rename(min_on3IntVar_default) min(Space& home, IntVar x0, IntVar x1, IntVar x2); */

/* %rename(min_onIVAANDIntVar_option) min(Space& home, const IntVarArgs& x, IntVar y, IntConLevel icl=ICL_DEF); */
/* %rename(min_onIVAANDIntVar_default) min(Space& home, const IntVarArgs& x, IntVar y); */

/* //MAX CONSTRAINTS */

/* %rename(max_on3IntVar_option) max(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF); */
/* %rename(max_on3IntVar_default) max(Space& home, IntVar x0, IntVar x1, IntVar x2); */

/* %rename(max_onIVAANDIntVar_option) max(Space& home, const IntVarArgs& x, IntVar y, IntConLevel icl=ICL_DEF); */
/* %rename(max_onIVAANDIntVar_default) max(Space& home, const IntVarArgs& x, IntVar y); */

/* //ABS CONSTRAINTS */

/* %rename(abs_option) abs(Space& home, IntVar x0, IntVar x1, IntConLevel icl=ICL_DEF); */
/* %rename(abs_default) abs(Space& home, IntVar x0, IntVar x1); */

/* //MULT CONSTRAINTS */

/* %rename(mult_option) mult(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF); */
/* %rename(mult_default) mult(Space& home, IntVar x0, IntVar x1, IntVar x2); */

/* //SQR CONSTRAINTS */

/* %rename(sqr_option) sqr(Space& home, IntVar x0, IntVar x1, IntConLevel icl=ICL_DEF); */
/* %rename(sqr_default) sqr(Space& home, IntVar x0, IntVar x1); */

/* //SQRT CONSTRAINTS */

/* %rename(sqrt_option) sqrt(Space& home, IntVar x0, IntVar x1, IntConLevel icl=ICL_DEF); */
/* %rename(sqrt_default) sqrt(Space& home, IntVar x0, IntVar x1); */

/* //DIVMOD CONSTRAINTS */

/* %rename(divmod_option) divmod(Space& home, IntVar x0, IntVar x1, IntVar x2, IntVar x3, IntConLevel icl=ICL_DEF); */
/* %rename(divmod_default) divmod(Space& home, IntVar x0, IntVar x1, IntVar x2, IntVar x3); */

/* //DIV CONSTRAINSTS */

/* %rename(div_option) div(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF); */
/* %rename(div_default) div(Space& home, IntVar x0, IntVar x1, IntVar x2); */

/* //MOD CONSTRAINTS */

/* %rename(mod_option) mod(Space& home, IntVar x0, IntVar x1, IntVar x2, IntConLevel icl=ICL_DEF); */
/* %rename(mod_default) mod(Space& home, IntVar x0, IntVar x1, IntVar x2); */

/* //LINEAR CONSTRAINTS */

/* %rename(linear_onIVAAndInt_option) linear(Space& home, const IntVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIVAAndInt_default) linear(Space& home, const IntVarArgs& x, IntRelType r, int c); */

/* %rename(linear_onIVAAndIntVar_option) linear(Space& home, const IntVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIVAAndIntVar_default) linear(Space& home, const IntVarArgs& x, IntRelType r, IntVar y); */

/* %rename(linear_onIVAAndIntAndBoolVar_option) linear(Space& home, const IntVarArgs& x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIVAAndIntAndBoolVar_default) linear(Space& home, const IntVarArgs& x, IntRelType r, int c,  BoolVar b ); */

/* %rename(linear_onIVAAndIntVarAndBoolVar_option) linear(Space& home, const IntVarArgs& x, IntRelType r, IntVar y, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIVAAndIntVarAndBoolVar_default) linear(Space& home, const IntVarArgs& x, IntRelType r, IntVar y, BoolVar b); */

/* %rename(linear_onIntArgsAndIVAAndInt_option) linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIntArgsAndIVAAndInt_default) linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, int c); */

/* %rename(linear_onIntArgsAndIVAAndIntVar_option) linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIntArgsAndIVAAndIntVar_default) linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, IntVar y); */

/* %rename(linear_onIntArgsAndIVAAndIntAndBoolVar_option) linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIntArgsAndIVAAndIntAndBoolVar_default) linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, int c , BoolVar b); */

/* %rename(linear_onIntArgsAndIVAAndIntVarAndBoolVar_option) linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, IntVar y, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIntArgsAndIVAAndIntVarAndBoolVar_default) linear(Space& home, const IntArgs& a, const IntVarArgs& x, IntRelType r, IntVar y , BoolVar b); */

/* %rename(linear_onBVAAndInt_option) linear(Space& home, const BoolVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onBVAAndInt_default) linear(Space& home, const BoolVarArgs& x, IntRelType r, int c); */

/* %rename(linear_onBVAAndBoolVar_option) linear(Space& home, const BoolVarArgs& x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onBVAAndBoolVar_default) linear(Space& home, const BoolVarArgs& x, IntRelType r, int c, BoolVar b); */

/* %rename(linear_onBVAAndIntVar_option) linear(Space& home, const BoolVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onBVAAndIntVar_default) linear(Space& home, const BoolVarArgs& x, IntRelType r, IntVar y); */

/* %rename(linear_onBVAAndIntVarAndBoolVar_option) linear(Space& home, const BoolVarArgs& x, IntRelType r, IntVar y, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onBVAAndIntVarAndBoolVar_default) linear(Space& home, const BoolVarArgs& x, IntRelType r, IntVar y, BoolVar b); */

/* %rename(linear_onIntArgsAndBVAAndInt_option) linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, int c, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIntArgsAndBVAAndInt_default) linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, int c); */

/* %rename(linear_onIntArgsAndBVAAndIntAndBoolVar_option) linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, int c, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIntArgsAndBVAAndIntAndBoolVar_default) linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, int c, BoolVar b); */

/* %rename(linear_onIntArgsAndBVAAndIntVar_option) linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, IntVar y, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIntArgsAndBVAAndIntVar_default) linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, IntVar y); */

/* %rename(linear_onIntArgsAndBVAAndIntVarAndBoolVar_option) linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, IntVar y, BoolVar b, IntConLevel icl=ICL_DEF); */
/* %rename(linear_onIntArgsAndBVAAndIntVarAndBoolVar_default) linear(Space& home, const IntArgs& a, const BoolVarArgs& x, IntRelType r, IntVar y, BoolVar b); */

/* //BRANCHING */

/* %rename(branch_onIVA_2option) branch(Space& home, const IntVarArgs& x, */
/*          IntVarBranch vars, IntValBranch vals, */
/*          const VarBranchOptions& o_vars = VarBranchOptions::def, */
/*          const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(branch_onIVA_1option) branch(Space& home, const IntVarArgs& x, */
/*          IntVarBranch vars, IntValBranch vals, */
/*          const VarBranchOptions& o_vars = VarBranchOptions::def); */
/* %rename(branch_onIVA_default) branch(Space& home, const IntVarArgs& x, */
/*          IntVarBranch vars, IntValBranch vals); */
         
         
/* %rename(branch_onIntVar_option) branch(Space& home, IntVar x, IntValBranch vals, */
/*          const ValBranchOptions& o_vals = ValBranchOptions::def);       */
/* %rename(branch_onIntVar_default) branch(Space& home, IntVar x, IntValBranch vals);  */

/* %rename(branch_onBVA_2option) branch(Space& home, const BoolVarArgs& x, */
/*          IntVarBranch vars, IntValBranch vals, */
/*          const VarBranchOptions& o_vars = VarBranchOptions::def, */
/*          const ValBranchOptions& o_vals = ValBranchOptions::def); */

/* %rename(branch_onBVA_1option) branch(Space& home, const BoolVarArgs& x, */
/*          IntVarBranch vars, IntValBranch vals, */
/*          const VarBranchOptions& o_vars = VarBranchOptions::def); */
/* %rename(branch_onBVA_default) branch(Space& home, const BoolVarArgs& x, */
/*          IntVarBranch vars, IntValBranch vals); */

/* %rename(branch_onBoolVar_option) branch(Space& home, BoolVar x, IntValBranch vals, */
/*          const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(branch_onBoolVar_default) branch(Space& home, BoolVar x, IntValBranch vals); */
         

/* //ASSIGNMENT */

/* %rename(assign_IntVarArgs_option) assign(Space& home, const IntVarArgs& x, IntAssign vals, const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(assign_IntVarArgs_default) assign(Space& home, const IntVarArgs& x, IntAssign vals); */

/* %rename(assign_IntVar_option) assign(Space& home, IntVar x, IntAssign vals, const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(assign_IntVar_default) assign(Space& home, IntVar x, IntAssign vals); */

/* %rename(assign_BoolVarArgs_option) assign(Space& home, const BoolVarArgs& x, IntAssign vals, const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(assign_BoolVarArgs_default) assign(Space& home, const BoolVarArgs& x, IntAssign vals); */

/* %rename(assign_BoolVar_option) assign(Space& home, BoolVar x, IntAssign vals, const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(assign_BoolVar_default) assign(Space& home, BoolVar x, IntAssign vals); */


/* /\****************************************Renaming of the set.hh part ******************\/ */



/* //DOM CONSTRAINTS */

/* %rename(dom_onSetVarAndInt) dom(Space& home, SetVar x, SetRelType r, int i); */
/* %rename(dom_onSetVarAnd2Int) dom(Space& home, SetVar x, SetRelType r, int i, int j); */
/* %rename(dom_onSetVarAndIntSet) dom(Space& home, SetVar x, SetRelType r, const IntSet& s); */
/* %rename(dom_onSetVarAndIntAndBoolVar) dom(Space& home, SetVar x, SetRelType r, int i, BoolVar b); */
/* %rename(dom_onSetVarAnd2IntAndBoolVar) dom(Space& home, SetVar x, SetRelType r, int i, int j, BoolVar b); */
/* %rename(dom_onSetVarAndIntSetAndBoolVar) dom(Space& home, SetVar x, SetRelType r, const IntSet& s, BoolVar b); */

/* //REL CONSTRAINTS */

/* %rename(rel_on2SetVar) rel(Space& home, SetVar x, SetRelType r, SetVar y); */
/* %rename(rel_on2SetVarAndBoolVar) rel(Space& home, SetVar x, SetRelType r, SetVar y, BoolVar b); */
/* %rename(rel_onSetVarAndIntVar) rel(Space& home, SetVar s, SetRelType r, IntVar x); */
/* %rename(rel_onIntVarAndSetVar) rel(Space& home, IntVar x, SetRelType r, SetVar s); */
/* %rename(rel_onSetVarAndIntVarAndBoolVar) rel(Space& home, SetVar s, SetRelType r, IntVar x, BoolVar b); */
/* %rename(rel_onIntVarAndSetVarAndBoolVar) rel(Space& home, IntVar x, SetRelType r, SetVar s, BoolVar b); */
/* %rename(rel_onSetVarAndIntVarForAll) rel(Space& home, SetVar s, IntRelType r, IntVar x); */
/* %rename(rel_onIntVarAndSetVarForAll) rel(Space& home, IntVar x, IntRelType r, SetVar s); */

/* %rename(rel_on3SetVar) rel(Space& home, SetVar x, SetOpType op, SetVar y, SetRelType r, SetVar z); */
/* %rename(rel_onSVAAndSetVar) rel(Space& home, SetOpType op, const SetVarArgs& x, SetVar y); */
/* %rename(rel_onSVAAndIntSetAndSetVar) rel(Space& home, SetOpType op, const SetVarArgs& x, const IntSet& z, SetVar y); */
/* %rename(rel_onIVAAndIntSetAndSetVar) rel(Space& home, SetOpType op, const IntVarArgs& x, const IntSet& z, SetVar y); */
/* %rename(rel_onIVAAndSetVar) rel(Space& home, SetOpType op, const IntVarArgs& x, SetVar y); */
/* %rename(rel_onIntSetAnd2SetVar) rel(Space& home, const IntSet& x, SetOpType op, SetVar y, SetRelType r, SetVar z); */
/* %rename(rel_onSetVarAndIntSetAndSetVar) rel(Space& home, SetVar x, SetOpType op, const IntSet& y, SetRelType r, SetVar z); */
/* %rename(rel_on2SetVarAndIntSet) rel(Space& home, SetVar x, SetOpType op, SetVar y, SetRelType r, const IntSet& z); */
/* %rename(rel_onIntSetAndSetVarAndIntSet) rel(Space& home, const IntSet& x, SetOpType op, SetVar y, SetRelType r, const IntSet& z); */
/* %rename(rel_onSetVarAndIntSetAndIntSet) rel(Space& home, SetVar x, SetOpType op, const IntSet& y, SetRelType r, const IntSet& z); */

/* //CONVEX CONSTRAINTS */
/* %rename(convex_onSetVar) convex(Space& home, SetVar x); */
/* %rename(convex_on2SetVar) convex(Space& home, SetVar x, SetVar y); */


/* //SEQUENCE CONSTRAINTS */

/* %rename(sequence_onSVA) sequence(Space& home, const SetVarArgs& x); */
/* %rename(sequence_onSVAAndSetVar) sequence(Space& home, const SetVarArgs& y, SetVar x); */

/* //MIN CONSTRAINTS */

/* %rename(min_onSetVarAndIntVar) min(Space& home, SetVar s, IntVar x); */
/* %rename(min_onSetVarAndIntVarAndBoolVar) min(Space& home, SetVar s, IntVar x, BoolVar b); */

/* //MAX CONSTRAINTS */
/* %rename(max_onSetVarAndIntVar) max(Space& home, SetVar s, IntVar x); */
/* %rename(max_onSetVarAndIntVarAndBoolVar) max(Space& home, SetVar s, IntVar x, BoolVar b); */

/* //CHANNEL CONSTRAINTS */
/* %rename(channel_onIVAAndSetVar) channel(Space& home, const IntVarArgs& x, SetVar y); */
/* %rename(channel_onIVAAndSVA) channel(Space& home, const IntVarArgs& x,const SetVarArgs& y); */
/* %rename(channel_onBVAAndSetVar) channel(Space& home, const BoolVarArgs& x, SetVar y); */

/* //CARDINALITY CONSTRAINTS */
/* %rename(cardinality_onSetVarAnd2Int) cardinality(Space& home, SetVar x, unsigned int i, unsigned int j); */
/* %rename(cardinality_onSetVarAndIntVar) cardinality(Space& home, SetVar s, IntVar x); */

/* //ELEMENTS CONSTRAINTS */
/* %rename(element_onSVAAnd2SetVar_option) element(Space& home, SetOpType op, const SetVarArgs& x, SetVar y, SetVar z, */
/*     const IntSet& u = IntSet(Set::Limits::min,Set::Limits::max)); */
/* %rename(element_onSVAAnd2SetVar_default) element(Space& home, SetOpType op, const SetVarArgs& x, SetVar y, SetVar z); */

/* %rename(element_onISAAnd2SetVar_option) element(Space& home, SetOpType op, const IntSetArgs& x, SetVar y, SetVar z, */
/*     const IntSet& u = IntSet(Set::Limits::min,Set::Limits::max)); */
/* %rename(element_onISAAnd2SetVar_default) element(Space& home, SetOpType op, const IntSetArgs& x, SetVar y, SetVar z); */

/* %rename(element_onSVAAndIntVarAndSetVar) element(Space& home, const SetVarArgs& x, IntVar y, SetVar z); */

/* %rename(element_onISAAndIntVarAndSetVar) element(Space& home, const IntSetArgs& s, IntVar y, SetVar z); */

/* %rename(element_onISAAnd2IntVarAnd2IntAndSetVar) element(Space& home, const IntSetArgs& a, IntVar x, int w, IntVar y, int h, SetVar z); */

/* %rename(element_onSVAAnd2IntVarAnd2IntAndSetVar) element(Space& home, const SetVarArgs& a, IntVar x, int w, IntVar y, int h, SetVar z); */
    

/* //BRANCHING */

/* %rename(branch_onSVA_2option) branch(Space& home, const SetVarArgs& x, */
/*          SetVarBranch vars, SetValBranch vals, */
/*          const VarBranchOptions& o_vars = VarBranchOptions::def, */
/*          const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(branch_onSVA_1option) branch(Space& home, const SetVarArgs& x, */
/*          SetVarBranch vars, SetValBranch vals, */
/*          const VarBranchOptions& o_vars = VarBranchOptions::def);      */
/* %rename(branch_onSVA_default) branch(Space& home, const SetVarArgs& x, */
/*          SetVarBranch vars, SetValBranch vals); */

/* %rename(branch_onSetVar_option) branch(Space& home, SetVar x, SetValBranch vals, */
/*          const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(branch_onSetVar_default) branch(Space& home, SetVar x, SetValBranch vals); */


/* //ASSIGNMENT */

/* %rename(assign_onSVA_option) assign(Space& home, const SetVarArgs& x, SetAssign vals, */
/*          const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(assign_onSVA_default) assign(Space& home, const SetVarArgs& x, SetAssign vals); */

/* %rename(assign_onSetVar_option) assign(Space& home, SetVar x, SetAssign vals, */
/*          const ValBranchOptions& o_vals = ValBranchOptions::def); */
/* %rename(assign_onSetVar_default) assign(Space& home, SetVar x, SetAssign vals); */





/****************************************************************************************************************/
/*																												*/
/*																												*/
/*											RENAMING OF BINARY RELATIONS PART									*/
/*																												*/
/*																												*/
/****************************************************************************************************************/

/******************************************** graph.hh ********************************************************/

/******************************************** edges-desc.hpp ********************************************************/

/* %rename(EdgesDesc_empty) EdgesDesc(void); */
/* %rename(EdgesDesc_n_Edges) EdgesDesc(const Edge r[], int n); */


%{
#include <gecode/kernel.hh>
#include <gecode/int.hh>
#include <gecode/set.hh>

  //#include <binrel/graph.hh>
  //#include <binrel/branch.hh>
  //#include <binrel/and.hh>
//#include <binrel/and.hpp>
//#include <binrel/follow.hh>

//#include <gecode/minimodel.hh> //D'ABORD LE LOW LEVEL	
#include <gelisp/gelisp.hpp>

  using namespace Gecode;
%}


%exception {
try {
  $action
    } catch(Gecode::Exception e) {
  std::cerr << e.what() << std::endl;
 } catch(Gecode::Exception& e) {
  std::cerr << e.what() << std::endl;
 } catch(Gecode::Exception *e) {
  std::cerr << e->what() << std::endl;
 } catch (...) {
  std::cerr << "unhandled exception." << std::endl;	
 }
 }


%include <gelisp/gelisp.hpp>
 //%include <gelisp/gecheader.hpp>












//PLUS UTILISE POUR LE MOMENT
//renaming for linear expressions
/// Default constructor
//%rename(new_empty_LinExpr) LinExpr(void); //TODO : IT DOES NOT RENAME ?!?
/// Create expression for constant \a c
//%rename(const_LinExpr) LinExpr(double c);
/// Create expression
//%rename(var_LinExpr) LinExpr(const IntVar& x);

//%rename(add_lin_expr) operator+(const LinExpr&, const LinExpr&);
