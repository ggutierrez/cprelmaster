##########################################################################
# Find Cudd library
##########################################################################
include(ExternalProject)
ExternalProject_Add(cudd_distr
#  URL ftp://vlsi.colorado.edu/pub/cudd-2.5.0.tar.gz
  URL ftp://vlsi.colorado.edu/pub/cudd-2.4.2.tar.gz
  PATCH_COMMAND patch -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/cudd.patch
  CONFIGURE_COMMAND echo "configure...There is no configure in cudd"
  BUILD_IN_SOURCE Yes
  BUILD_COMMAND make
  INSTALL_COMMAND echo "installing...fake"
  )


ExternalProject_Get_Property(cudd_distr source_dir)
add_library(cudd UNKNOWN IMPORTED)
set_property(TARGET cudd PROPERTY IMPORTED_LOCATION ${source_dir}cudd/libcudd.a )
add_dependencies(cudd cudd_distr)

#include the directories of cudd that conatin header files
include_directories("${source_dir}")
include_directories("${source_dir}/cudd")
include_directories("${source_dir}/mtr")
include_directories("${source_dir}/epd")

set(BDDDOMAIN_SRCS
 manager.cpp manager.hh
 encoding.cpp encoding.hh
 abstract.cpp replace.cpp 
 exception.hh
 bdd.hh bdd-print.cpp bdd-visitor.hpp
 perm-descriptor.hh rel-impl.cpp rel-impl.hh 
 tuple.cpp tuple.hh
 grelation.cpp grelation.hh 
 )

add_library(bdddomain ${BDDDOMAIN_SRCS})
target_link_libraries(bdddomain cudd)

##########################################################################
# BDDDomain Installation
##########################################################################
install(TARGETS bdddomain
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
install(DIRECTORY bdddomain/ DESTINATION include/bdddomain
  FILES_MATCHING PATTERN "*.hh" PATTERN "*.hpp")

add_subdirectory(tests)