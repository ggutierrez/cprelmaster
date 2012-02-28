#include <iostream>
#include <bdddomain/tuple.hh>

int main(void) {
  cout << "Different ways to create a tuple" << endl;
  // using a container with random access iterator
  std::vector<int> contents;
  contents.push_back(1);
  contents.push_back(2);
  MPG::Tuple c(contents);
  c.output(cout,2);
  cout << endl;
  /// using a constructor of any length
  MPG::Tuple t(1,2,3,4);
  t.output(cout, 4);
  cout << endl;
  /// using an initializer list
  MPG::Tuple u({5,6,7});
  u.output(cout,3);
  cout << endl;
  /// A unary tuple
  MPG::Tuple unary(2);
  unary.output(cout,1);
  cout << endl;
  return 0;
}