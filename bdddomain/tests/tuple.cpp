#include <cassert>
#include <iostream>
#include <type_traits>
#include <bdddomain/manager.hh>

using namespace std;
using namespace MPG::VarImpl;

BDD encodeElement(int p, int a) {
  assert(Limits::checkSafeValue(p) &&
         "The manager was not configured to support this value");
  assert(Limits::checkSafeArity(a) &&
         "The manager was not configured to support this arity");

  BDD f = one();
  for (int i = Limits::bitsPerInteger; i--;) {
    BDD v = factory().bddVar((i << Limits::ba)+a);
    if (p & 1)
      f &= v;
    else
      f &= !v;
    p >>= 1;
  }
  return f;
}
  
/*
  The only valid elements in tuples are integers. This trait is used
  during the instantiation of the makeTuple function to check that
  property.
*/
template <typename T>
struct validTupleElement {
  static const bool value = false;
};

template <>
struct validTupleElement<int> {
  static const bool value = true;
};


//Since variadic templates are recursive, must have a base case
BDD makeTuple() {
  static_assert(true,"Cannot create an empty tuple");
  return MPG::VarImpl::zero();
}

template <typename First, typename ...Rest>
BDD makeTuple(First f, Rest ...rest) {
  static_assert(validTupleElement<First>::value,"Expecting only integers");
  //cout << sizeof...(rest) << endl;
  int col = sizeof...(rest);

  cout << f << ' ';
  if (sizeof...(rest)) {
    return  encodeElement(f,col) & makeTuple(rest...); 
  }
  else {
    return MPG::VarImpl::one();
  }
}
  
int main(void) {
  cout << "This is the main" << endl;
  BDD f = makeTuple(3,4,5,6);
  //cout << "Size of the tuple: " << size << endl;
  return 0;
}
