#include <iostream>
#include <bdddomain/tuple.hh>

int main(void) {
  cout << "This is the main" << endl;

  std::vector<int> contents;
  contents.push_back(1);
  contents.push_back(2);
  //MPG::Tuple t(contents);
  MPG::Tuple t(1,2,3,4);
  t.output(cout, 4);
//  BDD f = makeTuple(3,4,5,6);
  //cout << "Size of the tuple: " << size << endl;
  return 0;
}