#ifndef __CPREL_BDDDOMAIN_EXCEPTION_HH__
#define __CPREL_BDDDOMAIN_EXCEPTION_HH__

#include <exception>

// The implementation of this file was taken from the gecode sources.

namespace MPG {
  class Exception : public std::exception {
  private:
    static const int li_max = 127;
    char li[li_max+1];
  public:
    /// Initialize with location \a l and information \a i
    Exception(const char* l, const char* i) throw ();
    /// Return information
    virtual const char* what(void) const throw();
  };
}
#endif
