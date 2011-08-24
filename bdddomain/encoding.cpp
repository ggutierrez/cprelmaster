#include <bdddomain/encoding.hh>
#include <bdddomain/unique.h>
#include <set>

namespace MPG { namespace VarImpl {

using std::pair;
using std::vector;

vector<int> bddIndices(int c) {
  vector<int> vars;
  vars.reserve(Limits::bitsPerInteger);

  for (int i = (1 << Limits::bbv); i--;) {
    vars.push_back((i << Limits::ba) + c);
  }
  return vars;
}

vector<DdNode*> bddVars(int c) {
  vector<DdNode*> vars;
  vars.reserve(Limits::bitsPerInteger);

  for (int i = Limits::bitsPerInteger; i--;) {
    int var = (i << Limits::ba)+c;
    vars.push_back(Cudd_bddIthVar(dd(),var));
  }
  return vars;
}

std::vector<int> decodeCube(int* cube, int arity) {
  const int cubeSize = 1 << (Limits::bbv + Limits::ba);
  // The maximum arity of a tuple with the current setup is maxTupleSize
  const int maxTupleSize = 1 << Limits::ba;

  // The working vector:
  // Reserve space in the vector to store a complete tuple stored in the bdd.
  // at the end we will not use all the information but we will perform some
  // operations on it.
  std::vector<int> ret(maxTupleSize,0);

  for (int i = cubeSize - 1; i >= 0; i--) {
    // In every iteration of this loop we are concerned only with tuples of arity
    // _arity. To iterate only in the elements in the cube that contain useful
    // information for this arity:
    //
    // *) (maxTupleSize -1) in binary is equivalent to ones at every place that
    //    has to be considered for the maximum arity.
    // *) Anding that number with the current bit of the cube  and comparing it
    //    less than the actual tuple arity will allow us to filter the bits in
    //    the cube that are meaningful for this tuple.
    if ((i & (maxTupleSize - 1)) < arity) {
      // compute the column that is being affected by the current element in the
      // cube.
      int column = maxTupleSize - 1 - (i & (maxTupleSize - 1));
      //int column = (i & (maxTupleSize - 1));

      // compute the possition in the column for the current element of the cube
      int bitInColumn = (Limits::bitsPerInteger-1-(i>>Limits::ba));

      ret[column] &= ~(1 << bitInColumn);
      ret[column] |= cube[i] << bitInColumn;
    }
  }
  return std::vector<int>(ret.begin()+(maxTupleSize-arity), ret.end());
}

DdNode* swap_columns(DdNode *r, const PermDescriptor& swapDesc) {
  const int numberOfVars = Limits::bitsPerInteger * Limits::arity;
  std::set<int> permuted;
  // This array is used to store the definition of the permutation
  int perm[numberOfVars];

  for (DescIterator i(swapDesc); i(); ++i) {
    std::vector<int> idx_orig = bddIndices(i.val().first);
    std::vector<int> idx_dest = bddIndices(i.val().second);
    for (unsigned int i = 0; i < idx_orig.size(); i++) {
      perm[idx_orig[i]] = idx_dest[i];
      permuted.insert(idx_orig[i]);
    }
  }

  for (int i = 0; i < numberOfVars; i++)
    if (permuted.find(i) == permuted.end())
      perm[i] = i;

  DdNode *ret = Cudd_bddPermute(dd(),r,perm);
  assert(ret && "bddPerm returned invalid bdd");

  return ret;
}

DdNode* shiftLeft(DdNode *r, int arity, const int n) {
  const int numberOfVars = Limits::bitsPerInteger * Limits::arity;
  assert( (n + arity) <= numberOfVars && "The shift cannot be carried out");

  // This array is used to store the definition of the permutation
  int perm[numberOfVars];

  // Move the variables of the relation itself
  for (int i = 0; i < Limits::arity; i++) {
    for (int j = i; j < numberOfVars;) {
      if (i < arity) {
        // The current j is part of the representation of column i in the relation
        perm[j] = j + n;
      } else if (i >= arity && i < arity+n) {
        perm[j] = j - n;
      } else {
        perm[j] = j;
      }
      j += Limits::arity;
    }
  }

  // perform the permutation
  DdNode *p = Cudd_bddPermute(dd(),r,perm);
  assert(p && "Permutation resulted in invalid bdd");

  /// \todo Do I have to existentially quantify on the first n columns of p?
  return p;
}

DdNode* shiftRight(DdNode *r, int arity, const int n) {
  const int numberOfVars = Limits::bitsPerInteger * Limits::arity;
  assert( arity >= n && "The shift cannot be carried out");

  // This array is used to store the definition of the permutation
  int perm[numberOfVars];
  std::vector<int> quantIndices;
  quantIndices.reserve(n*Limits::bitsPerInteger);

  // Move the variables of the relation itself
  for (int i = 0; i < Limits::arity; i++) {
    //std::cout << "Column " << i << ": ";
    for (int j = i; j < numberOfVars;) {
      //std::cout << " " << j;
      if (i < n) {
        perm[j] = j + arity;
        quantIndices.push_back(j + arity);
        //std::cout << "->*" << perm[j] << " ";
      } else if (i < arity) {
        // The current j is part of the representation of column i in the relation
        perm[j] = j - n;
        //std::cout << "->" << perm[j] << " ";
      } else {
        perm[j] = j;
        //std::cout << "->" << perm[j] << " ";
      }
      j += Limits::arity;
    }
    //std::cout << std::endl;
  }

  // perform the permutation
  DdNode *p = Cudd_bddPermute(dd(),r,perm);
  assert(p && "Permutation resulted in invalid bdd");
  Cudd_Ref(p);

  // quantify on the variables that were "removed" from the relation
  DdNode *cube = Cudd_IndicesToCube(dd(),&quantIndices[0],quantIndices.size());
  Cudd_Ref(cube);

  DdNode *q = Cudd_bddExistAbstract(dd(),p,cube);
  Cudd_RecursiveDeref(dd(),cube);
  Cudd_RecursiveDeref(dd(),p);
  return q;
}

DdNode* discard(DdNode *r, const int arity, const int left, const int right) {
  const int numberOfVars = Limits::bitsPerInteger * Limits::arity;
  /// \todo Add code to handle the cases when left and right are mistaken
  //assert( arity >= left && arity >= right && "The shift cannot be carried out");

  // This array is used to store the definition of the permutation
  int perm[numberOfVars];
  std::vector<int> quantIndices;
  quantIndices.reserve((left - right + 1)*Limits::bitsPerInteger);

  // Move the variables of the relation itself
  for (int i = 0; i < Limits::arity; i++) {
    for (int j = i; j < numberOfVars;) {
      if (i >= right && i <= left) {
        // The current variable is used to represent one of the columns that must
        // be discarded
        perm[j] = j + arity;
        quantIndices.push_back(j + arity);
      } else if (i > left && i < arity) {
        // The current variable is after the range of columns that was moved.
        // it will be moved back to replace the discarded columns
        perm[j] = (j - left - right + 1);
      } else {
        perm[j] = j;
      }
      j += Limits::arity;
    }
  }

// perform the permutation
DdNode *p = Cudd_bddPermute(dd(),r,perm);
assert(p && "Permutation resulted in invalid bdd");
Cudd_Ref(p);

// quantify on the variables that were "removed" from the relation
DdNode *cube = Cudd_IndicesToCube(dd(),&quantIndices[0],quantIndices.size());
Cudd_Ref(cube);

DdNode *q = Cudd_bddExistAbstract(dd(),p,cube);
Cudd_RecursiveDeref(dd(),cube);
Cudd_RecursiveDeref(dd(),p);
return q;
}

DdNode * exists(std::vector<int>& indices, DdNode* r) {
  DdNode *cube = Cudd_IndicesToCube(dd(),&indices[0],indices.size());
  Cudd_Ref(cube);
  DdNode *q = Cudd_bddExistAbstract(dd(),r,cube);
  Cudd_RecursiveDeref(dd(),cube);
  return q;
}

DdNode* exists(int c, DdNode* r) {
  std::vector<int> indices = bddIndices(c);
  return exists(indices,r);
}

DdNode* exists(int first, int last, DdNode* r) {
  assert(first <= last && "emtpy column identifier set");
  std::vector<int> indices = bddIndices(first);

  for(int i = last; i > first; i--) {
    std::vector<int> idx = bddIndices(i);
    std::copy(idx.begin(),idx.end(),std::back_inserter(indices));
  }
  return exists(indices,r);
}

DdNode* unique(int c, DdNode* r) {
  std::vector<int> indices = bddIndices(c);
  DdNode *cube = Cudd_IndicesToCube(dd(),&indices[0],indices.size());
  Cudd_Ref(cube);
  DdNode *q = Cudd_bddUniqueAbstract(dd(),r,cube);
  Cudd_RecursiveDeref(dd(),cube);
  return q;
}

DdNode* unique(const std::vector<int>& c, DdNode* r) {
  std::vector<int> indices;
  indices.reserve(c.size() * Limits::bitsPerInteger);
  for (unsigned int i = 0; i < c.size(); i++) {
    std::vector<int> x = bddIndices(c.at(i));
    for (unsigned int j = 0; j < x.size(); j++)
      indices.push_back(x[j]);

  }

  DdNode *cube = Cudd_IndicesToCube(dd(),&indices[0],indices.size());
  Cudd_Ref(cube);
  DdNode *q = Cudd_bddUniqueAbstract(dd(),r,cube);
  Cudd_RecursiveDeref(dd(),cube);
  return q;
}

DdNode* forall(int c, DdNode* r) {
  std::vector<int> indices = bddIndices(c);
  DdNode *cube = Cudd_IndicesToCube(dd(),&indices[0],indices.size());
  Cudd_Ref(cube);
  DdNode *q = Cudd_bddUnivAbstract(dd(),r,cube);
  Cudd_RecursiveDeref(dd(),cube);
  return q;
}

#ifndef NDEBUG
void debug_bdd(DdNode* node, int a) {
    if (node == one()) {
      std::cerr << "Universe";
      return;
    }
    if (node == zero()) {
      std::cerr << "Empty";
      return;
    }

    DdGen* gen;
    int *cube = (int*) malloc (sizeof(int)*(1<<(Limits::bbv + Limits::ba)));
    int tuple[1<<Limits::ba];
    CUDD_VALUE_TYPE val;
    int done;
    int i,k,j;
    std::cerr << "#(" << Cudd_CountMinterm(dd(),node,a << Limits::bbv) << "){";
    //printf("Cardinality: %f\n", Cudd_CountMinterm(Cudd::dd,node,a<<BBV));
    for(k=0;k<1<<Limits::ba;k++)tuple[k]=0;
    Cudd_ForeachCube(dd(),node,gen,cube,val){
      done=0;
      while(!done){
        done=1;
        for(i=(1<<(Limits::bbv+Limits::ba))-1;i>=0;i--){
          if((i&((1<<Limits::ba)-1))<a){
            tuple[i&((1<<Limits::ba)-1)]&=~(1<<((1<<Limits::bbv)-1-(i>>Limits::ba)));
            tuple[i&((1<<Limits::ba)-1)]|=(cube[i]&1)<<((1<<Limits::bbv)-1-(i>>Limits::ba));
            if((cube[i]&2)&&done){
              done&=cube[i]&1;
              cube[i]^=1;
            }
          }
        }
        std::cerr << "<";
        for(j = 0; j < a; j++) {
          std::cerr << tuple[j] << ",";//printf("%d,",tuple[j]);
        }
        //printf(">\n");
        std::cerr << ">, ";
      }
    }
    std::cerr << "}";
}

#endif

}}

