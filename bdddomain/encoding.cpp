#include <bdddomain/encoding.hh>
#include <bdddomain/unique.h>

namespace MPG { namespace CPRel { namespace VarImpl {

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
  vector<DdNode*> orig, perm;
  int descSize = static_cast<int>(swapDesc.size());
  orig.reserve(descSize * Limits::bitsPerInteger);
  perm.reserve(descSize * Limits::bitsPerInteger);

  typedef vector<pair<int,int> >::const_iterator It;
  vector<DdNode*> tempOrig, tempPerm;
  tempOrig.reserve(Limits::bitsPerInteger);
  tempPerm.reserve(Limits::bitsPerInteger);
  for (DescIterator i(swapDesc); i(); ++i) {
    tempOrig = bddVars(i.val().first);
    tempPerm = bddVars(i.val().second);
    std::copy(tempOrig.begin(), tempOrig.end(), back_inserter(orig));
    std::copy(tempPerm.begin(), tempPerm.end(), back_inserter(perm));
    tempOrig.clear();
    tempPerm.clear();
  }

  assert(static_cast<int>(orig.size()) == descSize * Limits::bitsPerInteger &&
         static_cast<int>(perm.size()) == descSize * Limits::bitsPerInteger &&
         "size of the bddVar vector is not correct");

  return Cudd_bddSwapVariables(dd(),r,&orig[0],&perm[0],orig.size());
}

DdNode* shiftLeft(DdNode *r, int arity, const int n) {
  const int numberOfVars = Limits::bitsPerInteger * Limits::arity;
  //std::cout << "Number of variables in the bdd " << numberOfVars << std::endl;
  assert( (n + arity) <= numberOfVars && "The shift cannot be carried out");

  // This array is used to store the definition of the permutation
  int perm[numberOfVars];
  // A vector used for the quantification
  //std::vector<int> quant;
  //quant.reserve(n*Limits::bitsPerInteger);

  // Move the variables of the relation itself
  for (int i = 0; i < Limits::arity; i++) {
    //std::cout << "Column " << i << ": ";
    for (int j = i; j < numberOfVars;) {
      //std::cout << " " << j;
      if (i < arity) {
        // The current j is part of the representation of column i in the relation
        perm[j] = j + n;
        //std::cout << "->" << perm[j] << " ";
      } else if (i >= arity && i < arity+n) {
        perm[j] = j - n;
        //std::cout << "->" << perm[j] << " ";
        //quant.push_back(j-n);
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

DdNode* exists(int c, DdNode* r) {
  std::vector<int> indices = bddIndices(c);
  DdNode *cube = Cudd_IndicesToCube(dd(),&indices[0],indices.size());
  Cudd_Ref(cube);
  DdNode *q = Cudd_bddExistAbstract(dd(),r,cube);
  Cudd_RecursiveDeref(dd(),cube);
  return q;
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


}}}

