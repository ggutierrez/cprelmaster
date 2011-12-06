#include <iostream>
#include <vector>
#include <bdddomain/tuple.hh>
#include <bdddomain/rel-impl.hh>

using std::vector;
using std::pair;
using  MPG::VarImpl::RelationImpl;
/*
template <typename Functor>
void flat(const vector<vector<pair<int,int>>>& tuple, int i, vector<int>& val, Functor& fc) {
  if (static_cast<unsigned int>(i) >= val.size()) {
    fc(val);
    return;
  }
  
  for (const pair<int,int>& j : tuple.at(i)) {
    for (int e = j.first; e <= j.second; e++) {
      //int original = val.at(i);
      val[i] = e;
      flat(tuple,i+1,val,fc);
      //val[i] = original;
   }
    
  }
}

template <typename Functor>
void printAll(RelationImpl r, Functor& fc) {
  auto predicate = [=](const vector<vector<pair<int,int>>>& tuple) {
    vector<int> val(tuple.size(),-2);
    flat(tuple,0,val,fc);
  };
  r.visit(predicate);
}
*/
int main(void) {
  RelationImpl  ternary(3);
  
  ternary.add({0,0,1});
  ternary.add({0,1,1});
  ternary.add({0,0,2});
  ternary.add({1,2,3});
  /*
  vector<int> val(3,-2);
  vector<vector<pair<int,int>>> test = 
    {
      {{1,2},{5,6}},
      {{1,1}},
      {{2,3},{10,12}}
    };
  flat(test,0,val);
  */
  int tuples = 0;
  auto predicate = [&](const vector<int>& tuple) {
    for (unsigned int i = ternary.arity(); i--;)
      cout << tuple.at(i) << ",";
    cout << endl;
    tuples++;
  };
  ternary.visit(predicate);
  cout << "visited " << tuples << " tuples" << endl;
  return 0;
}
