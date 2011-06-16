#ifndef __EXAMPLE_TEST_HH__
#define __EXAMPLE_TEST_HH__

#include <string>

using std::string;

class Test {
private:
  string name_;
public:
  /// Constructor
  Test(const char* name);
  /// Output to standard outpu
  void out(void) const;
};

#endif
