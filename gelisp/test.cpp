#include <gelisp/test.hh>
#include <iostream>

Test::Test(const char* name) 
  : name_(name) {}

void Test::out(void) const {
  std::cout << name_ << std::endl;
}
