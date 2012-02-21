set(KNOWN_TO_WORK No)
# Only versions >4.6 of have the C++11 features that the package uses.
if(CMAKE_COMPILER_IS_GNUCC)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
    OUTPUT_VARIABLE GCC_VERSION)
  if (GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6)
    set(KNOWN_TO_WORK Yes)
  endif()
endif()

if (NOT KNOWN_TO_WORK)
  message(WARNING "The compiler being used has not been tested.")
endif()

# the compiler should implement part of the C++11 standard
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-std=c++0x" C0X_SUPPORT)
if(C0X_SUPPORT)
#  message(STATUS "CXX has c++0x support")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(FATAL_ERROR "c++0x capable compiler is needed to build this project at this time")
endif()

check_cxx_compiler_flag(-Wall FLAG_WALL)
if(FLAG_WALL)
  add_definitions(-Wall)
endif()

check_cxx_compiler_flag(-Wextra FLAG_WEXTRA)
if(FLAG_WEXTRA)
  add_definitions(-Wextra)
endif()

check_cxx_compiler_flag(-fdiagnostics-show-option FLAG_DIAGNOSTIC)
if(FLAG_DIAGNOSTIC)
  add_definitions(-fdiagnostics-show-option)
endif()
