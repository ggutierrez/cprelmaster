#include <cdd.hh>

namespace Cdd {
  using namespace std;
  using namespace tr1;
  /*
   * BDD
   */
  BDD::BDD(void) : node(NULL) {}
	
	BDD::BDD(DdNode *n) {
		node = n;
		if (node != NULL) 
			Cudd_Ref(node);
	}
  
	BDD::BDD(const BDD& from) {
		node = from.node;
		if (node != NULL)
			Cudd_Ref(node);
	}
	
  BDD::~BDD(void) {
		if (node) {
			Cudd_RecursiveDeref(Cudd::dd, node);
		}
	};
	
	BDD::BDD(int v, bool b) {
		if (b) {
			node = Cudd_bddIthVar(Cudd::dd,v);
		} else {
			node = Cudd_Not(Cudd_bddIthVar(Cudd::dd,v));
		}
		assert(node != NULL);
		Cudd_Ref(node);
	}
	
  BDD BDD::create(tuple<int,int> t) {
    return path(get<0>(t), 0) && path(get<1>(t), 1);
  }
  
	bool BDD::operator == (const BDD& b) const {
		return node == b.node;
	}
	
	bool BDD::operator != (const BDD& b) const {
		return !(*this == b);
	}
	
	BDD BDD::operator ! (void) const {
		if (node == CDD_UNK(Cudd::dd)) {
			return *this;
		}
		return BDD(Cudd_Not(node));
	}
	
	BDD BDD::operator ~ (void) const {
		return !(*this);
	}
	
	BDD BDD::operator = (const BDD& right) {
		if (this == &right) return *this;
		if (right.node != NULL) Cudd_Ref(right.node);
		if (node != NULL)
			Cudd_RecursiveDeref(Cudd::dd,node);
    node = right.node;
		return *this;
	}
	
	BDD BDD::operator &= (const BDD& other) {
		DdNode *r = Cudd_cddAnd(Cudd::dd, node, other.node);
		assert(r != NULL);
		Cudd_Ref(r);
		Cudd_RecursiveDeref(Cudd::dd, node);
		node = r;
		return *this;
	}

	BDD BDD::operator |= (const BDD& other) {
		DdNode *r = Cudd_cddOr(Cudd::dd, node, other.node);
		assert(r != NULL);
		Cudd_Ref(r);
		Cudd_RecursiveDeref(Cudd::dd, node);
		node = r;
		return *this;
	}

	BDD BDD::operator << (const BDD& other) const {
		
		DdNode *r = Cudd_cddMerge(Cudd::dd, node, other.node);
		if (r == NULL) {
			if (Cudd::dd->errorCode = CDD_FAIL)
				throw failedMerge();
			else {
				assert(r);
			}

		}

		return BDD(r);
	}
	
	BDD BDD::operator && (const BDD& other) const {
		
		DdNode *r = Cudd_cddAnd(Cudd::dd, node, other.node);
		assert(r);
		return BDD(r);
	}
  
  BDD BDD::status(const BDD& r) const {
    DdNode *s = Cudd_cddStatus(Cudd::dd, node, r.node);
		assert(s);
		return BDD(s);
  }
  
  BDD BDD::delta(void) const {
    DdNode *s = Cudd_cddDelta(Cudd::dd, node);
    assert(s);
    return BDD(s);
  }
	
	BDD BDD::operator || (const BDD& other) const {
		
		DdNode *r = Cudd_cddOr(Cudd::dd, node, other.node);
		assert(r);
		return BDD(r);
	}
	
  BDD BDD::operator ^ (const BDD& other) const {
    DdNode *r = Cudd_cddXor(Cudd::dd, node, other.node);
    assert(r);
    return BDD(r);
  }
  
	double BDD::minterms(int nvars) const {
		return Cudd_CountMinterm(Cudd::dd, node, nvars<<BBV);
	}
	
	int BDD::numNodes(void) const {
		return Cudd_DagSize(node);
	}

  
	void BDD::dot(const char* fname) const {
		FILE *out = fopen(fname,"w");
		DdNode *outputs[] = {node};
		char *names[] = {"f"};
		Cudd_DumpDot(Cudd::dd,1,outputs,NULL,names,out);
		fclose(out);
	}
  
  // encoding specific bdd operations
  BDD BDD::path(int p, int a) {
		BDD f = Cudd::one();
		for (int i = (1<<BBV); i--;) {
			f &= BDD((i<<BA)+a,p&1);
			p >>= 1;
		}
		return f;
	}
  
	void BDD::printTuples(int a, std::ostream& os) const {
    if (node ==CDD_ONE(Cudd::dd)) {
      os << "Universe";
      return;
    }
    if (node == CDD_ZERO(Cudd::dd)) {
      os << "Empty";
      return;
    }
    
		DdGen* gen;
		int *cube = (int*)malloc(sizeof(int)*(1<<(BBV+BA)));
		int tuple[1<<BA];
		CUDD_VALUE_TYPE val;
		int done;
		int i,k,j;
    os << "#(" << Cudd_CountMinterm(Cudd::dd,node,a<<BBV) << "){";
		//printf("Cardinality: %f\n", Cudd_CountMinterm(Cudd::dd,node,a<<BBV));
		for(k=0;k<1<<BA;k++)tuple[k]=0;
		Cudd_ForeachCube(Cudd::dd,node,gen,cube,val){
			done=0;
			while(!done){
				done=1;
				for(i=(1<<(BBV+BA))-1;i>=0;i--){
					if((i&((1<<BA)-1))<a){
						tuple[i&((1<<BA)-1)]&=~(1<<((1<<BBV)-1-(i>>BA)));
						tuple[i&((1<<BA)-1)]|=(cube[i]&1)<<((1<<BBV)-1-(i>>BA));
						if((cube[i]&2)&&done){
							done&=cube[i]&1;
							cube[i]^=1;
						}
					}
				}
				os << "<";
				for(j = 0; j < a; j++) {
					os << tuple[j] << ",";//printf("%d,",tuple[j]);
				}
				//printf(">\n");
        os << ">, ";
			}
		}		
    os << "}";
	}
	/*
	 * Cudd
	 */
	DdManager *Cudd::dd = Cudd_Init(0,0,CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS,0);
	
	Cudd::Cudd(void) {}
	
	BDD Cudd::one(void) {
		return BDD(CDD_ONE(Cudd::dd));
	}

	BDD Cudd::unk(void) {
		return BDD(CDD_UNK(Cudd::dd));
	}
	
	BDD Cudd::zero(void) {
		return BDD(CDD_ZERO(Cudd::dd));
	}
	
#ifndef NDEBUG 
	int Cudd::references(void) {
		return Cudd_CheckZeroRef(Cudd::dd);
	}
  
  void Cudd::stats(void) {
    Cudd_PrintInfo(Cudd::dd, stdout);
  }
#endif
	
  /*
   * Relation 
   */
	
  Relation::Relation(int a) : r(Cudd::zero()), arity(a) {}
	
  Relation::Relation(const BDD& lb, const BDD& ub, int a)
  : arity(a) {
    init(lb, ub);
  }
  
  Relation::Relation(const BDD& d, int a): r(d), arity(a) {}
  
	Relation::~Relation(void) {}
	
	void Relation::init(const BDD& lb, const BDD& ub) {
		BDD l = lb || Cudd::unk();
		BDD u = ub && Cudd::unk();
		try {
      r = (l << u);
    }
    catch (failedMerge& e) {
      std::cerr << "Failed merge operation" << std::endl;
      throw;
    }
	}
  
  void Relation::init(const BDD& d) {
    r = d;
  }
  
  void Relation::exclude(const BDD& g) {
    r = r << (!g && Cudd::unk()); 
  }

  Relation& Relation::operator >>= (const BDD& g) {
    (*this).exclude(g);
    return *this;
  }
  
  void Relation::include(const BDD& g) {
    r = r << (g || Cudd::unk()); 
  }

  Relation& Relation::operator <<= (const BDD& g) {
    (*this).include(g);
    return *this;    
  }
  
  const BDD Relation::glb(void) const {
    return r.status(Cudd::one());
  }

  const BDD Relation::oob(void) const {
    return r.status(Cudd::zero());
  }

  const BDD Relation::unk(void) const {
    return r.status(Cudd::unk());
  }

  const BDD Relation::dom(void) const {
    return r;
  }
  
  std::ostream& operator << (std::ostream& os, const Relation& r) {
    os << "<glb: ";
    r.glb().printTuples(2,os);
    os << std::endl << "unk: ";
    r.unk().printTuples(2,os);
    os << ">";
    return os;
  }
}
