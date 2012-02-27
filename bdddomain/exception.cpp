#include <bdddomain/exception.hh>

namespace MPG {
  /*
   * Base class for exceptions
   *
   */
  Exception::Exception(const char* l, const char* i) throw() {
    int j = 0;
    while ((*l != 0) && (j < li_max))
      li[j++] = *(l++);
    if (j < li_max)
      li[j++] = ':';
    if (j < li_max)
      li[j++] = ' ';
    while ((*i != 0) && (j < li_max))
      li[j++] = *(i++);
    li[j] = 0;
  }
  const char*
  Exception::what(void) const throw() {
    return &li[0];
  }
}