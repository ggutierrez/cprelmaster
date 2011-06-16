;;to test, load this file and execute (test) : 
;;(load "/Users/saschavancauwelaert/Documents/EPL/Memoire/gelisp/examples/test.lisp")
;;(test)

(load "/Users/saschavancauwelaert/Documents/EPL/Memoire/gelisp/gelisp_highlayer.lisp")

(defun test ()
	;; declare a space
	(setq sp (new_GlSpace))
	;; declare a single var
	;(defvar i (GlSpace_newIntVar_minmax sp 0 2))
	;(defvar j (GlSpace_newIntVar_minmax sp 0 4))
	;(setq k (GlSpace_newIntVar_minmax sp 0 4))
	;; declare an array of variables
	;(defvar a (new_IntVarList_minmax sp 4 0 10))
	;(defvar b (new_IntVarList_minmax sp 3 0 10))
	
	;(defvar boollist1 (new_BoolVarList_minmax sp 5 0 1))
	;(defvar boollist2 (new_BoolVarList_minmax sp 5 0 1))
	
	;(setq testbool (GlSpace_newBoolVar sp 0 1))
	
	;;WORKS !!!
	;(defvar test2 (GlSpace_newSetVar_home sp))
	;(setq is (new_IntSet_minmax 1 1) )
	;(setq is2 (new_IntSet_minmax 0 3) )
	
	;(setq test3 (GlSpace_newSetVar_set_set_default sp is is2))
	;(setq test4 (GlSpace_newSetVar_minmax_set_default sp 0 2 is2))
	
	(setq test3 (GlSpace_newSetVar_minmax_minmax_default sp 0 0 0 10))
	(setq test4 (GlSpace_newSetVar_minmax_minmax_default sp 4 7 4 7))
	
	;(setq test3 (GlSpace_newSetVar_home sp))
	;(setq test4 (GlSpace_newSetVar_home sp))
	
	;(defvar testsetvar (GlSpace_newSetVar_minmax_minmax_default sp 1 3 1 7) )
	;;(defvar test5 (GlSpace_newSetVar_set_minmax_default sp 9 15 ))
	
	;;WORKS!!!
	;(defvar testSetList (new_SetVarList_set_set_default sp 10 is is))
	
	;; show the space contents
 	;(GlSpace_debug sp)
	;; post all different constraint on a
	;;(g_distinct sp (IntVarList_get a))
	
	
	;;WORKS!!!
 	;;(dom_onIntVar_default sp i 3)
 	
 	
	
	;; post a[0] = 3 WORKS !!!
	;;(dom_onIntVar_default sp (IntVarList_getVar a 0) 3)
 	
 	;;WORKS !!!
 	;;(dom_onIntVar_option sp (IntVarList_getVar a 1) 3 :ICL_VAL)
 	
 	;; post a[i] = 3 for all i WORKS !!!
 	;;(dom_onIntVarArgs_default sp (IntVarList_get a) 3)
 	
 	;; post 1<= a[0] <= 3 WORKS !!!
 	;;(dom_leq_default sp (IntVarList_getVar a 0) 1 3)
 	
 	;;works!!
 	;;(dom_leq_onIntVarArgs_default sp (IntVarList_get a) 1 3)
 	
 	;;create an IntSet WORKS!
 	;;(defvar testintset (new_IntSet_minmax 1 2))
 	
 	;;WORKS!
 	;;(dom_set_onIntVar_default sp (IntVarList_getVar a 0) testintset)
 	
 	;;WORKS!
 	;;(dom_set_onIntVarArgs_default sp (IntVarList_get a) testintset)
 	
 	;;WORKS!
 	;;(rel_onIntVar_default sp i :IRT_LQ j) 
 	
 	;;WORKS!
 	;;(rel_IntVarAndIntVarArgs_default sp (IntVarList_get a) :IRT_LQ j)
 	
 	;;WORKS!
 	;(rel_IntVarAndInt_default sp j :IRT_LQ 3)
 	
 	;;WORKS!
 	;;(rel_IntVarArgsAndInt_default sp (IntVarList_get a) :IRT_LQ 2)
 	
 	;;WORKS!
 	;;(rel_onIntVarArgs_default sp (IntVarList_get a) :IRT_LQ)
 	
 	;;WORKS!
 	;;(rel_onIntVarArgsAndIntVarArgs_default sp (IntVarList_get a) :IRT_EQ (IntVarList_get b))
 	
 	;;WORKS!!
 	;;(element_onIntVarArgsAnd2IntVar_default sp (IntVarList_get a) i j)
 	
 	;;WORKS!!
 	;(element_onIntVarArgsAndIntVarAndInt_default sp (IntVarList_get a) i 2)
 	
 	;;WORKS!!
 	;(channel_onIntVarArgsAndIntVarArgs_default sp (IntVarList_get a) (IntVarList_get b))
 	
 	;;WORKS ONLY ON SMALL ARRAYS ?!? WHY ?!?
 	;(sorted_onIVAAndIVA_default sp (IntVarList_get a) (IntVarList_get b))
 	
 	;;WORKS!!!
 	;(count_onIVAAnd2Int_default sp (IntVarList_get a) 2 :IRT_EQ 3)
 	
 	;;WORKS!!
 	;;(min_on3IntVar_default sp i j k )
 	
 	;;WORKS!!!
 	;(linear_onIVAAndInt_default sp a :IRT_EQ 7)
 	
 	;;WORKS!!!
 	;(dom_onIntVarAndIntAndBoolVar_default sp i 3 testbool)
 	
 	;; WORKS but strange it didn't at first ...
 	;(clause_onBoolVar_default sp :BOT_AND (BoolVarList_get boollist1) (BoolVarList_get boollist2) testbool)
 	
 	;;WORKS!!!
 	;(assign_BoolVar_default sp testbool :INT_ASSIGN_MIN)
 	
 	;(defvar l (new_IntList 5 0 1 ) )
 	
 	;WORKS!!!
 	;(defvar isl (new_IntSetList 5 0 2))
 	
 	;WORKS!!!
 	;(IntSet_max (IntSetList_getElement isl 1))
 	
 	;WORKS!!!
 	;(count_onIVAAndISA_default sp (IntVarList_get a) (IntSetList_get isl))
 	
 	;;;WORKS!!!
 	;(sequence_IVA_default sp (IntVarList_get a) is 2 0 0)
 	
 	;(defvar is (new_IntSet_minmax 0 2) )
 	
 	;WORKS!!!
 	;(dom_onSetVarAndInt sp test3 :SRT_EQ 1)
 	
 	;;WORKS!!!
 	;(cardinality sp test3 1 2)
 	
 	
 	;WORKS !!!
 	;(rel_on2SetVar sp test3 :SRT_EQ test4)
 	
 	
 	;WORKS !!!
 	;(rel_on2SetVarAndBoolVar sp test4 :SRT_EQ test3 testbool)
 	
 	;WORKS !!!
 	;(convex_onSetVar sp test3)
 	
 	
 	;WORKS!!!
 	;(convex_on2SetVar sp test3 test4)
 	
 	;(setq sva (new_SetVarList_home sp 2))
 	(setq sva (new_SetVarList_minmax_minmax_default sp 2 4 7 4 7))
 	
 	
 	;WORKS!!!
 	;(sequence_onSVAAndSetVar sp (SetVarList_get sva) test4)
 	
 	;WORKS!!!
 	;(atmostOne sp (SetVarList_get sva) 5)
 	
 	;WORKS!!!
 	;(min_onSetVarAndIntVar sp (IntVarList_get test4) j)
 	
 	;WORKS!!!
	;(channel_onIVAAndSetVar sp (IntVarList_get a) test4)
	
	;WORKS!!!
	;(cardinality_onSetVarAndIntVar sp test4 j)
	
	;(setq w (new_IntList_sizeAndDefaultValue 5 1))
	;(setq e (new_IntList_sizeAndStartAndInc 5 4 1))
	
	;WORKS!!!
	;(weights sp e w test4 j)
	
	;WORKS!!!
	;(element_onSVAAnd2SetVar_default sp :SOT_UNION (SetVarList_get sva) test3 test4)
	
	
	
	
	
	
	
	
	
	(GlSpace_branch sp)
	
	;(GlSpace_run sp 1) DEPRECATED
	
	(setq v (GlSpace_createGlSpaceVector sp))
	(GlSpace_run sp 3 v)
	;(print (IntVar_val k ))
)

(defun test2 ()

	(setq sp (new_GlSpace))
	;(setq a (GlSpace_newIntVar_minmax sp 0 5))
	;(setq c (GlSpace_newIntVar_minmax sp 0 5))
	;(setq d (GlSpace_newIntVar_minmax sp 0 5))
	(defvar b (new_IntVarList_minmax sp 2 0 10))
	(dom_onIntVar_default sp (IntVarList_getVar b 0) 3)
	(GlSpace_branch sp)
	(setq v (GlSpace_createGlSpaceVector sp))
	(GlSpace_run sp 1 v)
	;(setq sol (GlSpace_getGlSpaceFromVector sp v 0))
	
	;WORKS enven with several intvars!!!
	;(IntVar_val (GlSpace_getIntVar sol (GlIntVar_getIndexInVector a)))
	
	
	
	
	;(GlIntVar_getIndexInVector (IntVarList_getVar b 0))
	
	;(GlIntVar_getIndexInVector d)
	;(IntVar_val (GlSpace_getIntVar sol ))
	
	(IntVarList_getIndexOfAVarInVector b 0)
	
	
)


(defun testGraph ()
	(setq sp (new_GlSpace))
	
	;WORKS !!!
	(setq h (GlSpace_newGraphVar_int sp 2))
	
	(setq f (GlSpace_newGraphVar_int sp 1))
	
	(setq g (GlSpace_newGraphVar_int sp 0))
	
	;WORKS but a problem when I print it ...!!!
	
	#|(setq e1 (new_Edge 0 0))
	 
	(setq vec1 (createEdgeVector))
	
	(addEdge e1 vec1)
	
	(setq ar1 (vecToArray vec1))
	
	(setq ed1 (new_EdgesDesc_n_Edges ar1 1))
	
	(setq e2 (new_Edge 0 0))
	(setq e3 (new_Edge 1 1))
	 
	(setq vec2 (createEdgeVector))
	
	(addEdge e2 vec2)
	(addEdge e3 vec2)
	
	(setq ar2 (vecToArray vec2))
	
	(setq ed2 (new_EdgesDesc_n_Edges ar2 2))
		
	(setq g (GlSpace_newGraphVar_EdgesDesc sp ed1 ed2))|#
	
	
	
	;(GlSpace_debug sp)
	
	
	;DON'T WORK !!!
	;(land sp f g h)
	;(lor sp f g h)
	
	;DOESN'T CRASH ...
	(transClousure sp g)
	
	(GlSpace_branch sp)
	
	(setq v (GlSpace_createGlSpaceVector sp))
	(GlSpace_run sp 1 v)
)