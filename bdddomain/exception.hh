#ifndef __CPREL_BDDDOMAIN_EXCEPTION_HH__
#define __CPREL_BDDDOMAIN_EXCEPTION_HH__

#include <exception>
#include <boost/exception/all.hpp>

namespace MPG {
/// Exception base class
struct ExceptionBase : virtual std::exception, virtual boost::exception {};
typedef boost::error_info<struct tag_errno_code,int> errno_code;
}
#endif
