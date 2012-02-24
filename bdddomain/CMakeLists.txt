##########################################################################
# Find Cudd library
##########################################################################
include(ExternalProject)
ExternalProject_Add(cudd_distr
#  URL ftp://vlsi.colorado.edu/pub/cudd-2.5.0.tar.gz
  URL ftp://vlsi.colorado.edu/pub/cudd-2.4.2.tar.gz
  PATCH_COMMAND patch -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/cudd2.patch
  CONFIGURE_COMMAND echo "configure...There is no configure in cudd"
  BUILD_IN_SOURCE Yes
  BUILD_COMMAND make && cd obj && make 
  INSTALL_COMMAND echo "installing...fake"
  )


ExternalProject_Get_Property(cudd_distr source_dir)
message(STATUS "The source of cudd: ${source_dir}")

# Resource library for libcudd.a
add_library(cudd UNKNOWN IMPORTED)
set_property(TARGET cudd PROPERTY IMPORTED_LOCATION ${source_dir}/cudd/libcudd.a )
add_dependencies(cudd cudd_distr)

# Resource library for libmtr.a
add_library(mtr UNKNOWN IMPORTED)
set_property(TARGET mtr PROPERTY IMPORTED_LOCATION ${source_dir}/mtr/libmtr.a )
add_dependencies(mtr cudd_distr)

# Resource library for libst.a
add_library(st UNKNOWN IMPORTED)
set_property(TARGET st PROPERTY IMPORTED_LOCATION ${source_dir}/st/libst.a )
add_dependencies(st cudd_distr)

# Resource library for libepd.a
add_library(epd UNKNOWN IMPORTED)
set_property(TARGET epd PROPERTY IMPORTED_LOCATION ${source_dir}/epd/libepd.a )
add_dependencies(epd cudd_distr)

# Resource library for libutil.a
add_library(util UNKNOWN IMPORTED)
set_property(TARGET util PROPERTY IMPORTED_LOCATION ${source_dir}/util/libutil.a )
add_dependencies(util cudd_distr)

# Resource library for libobj.a
add_library(obj UNKNOWN IMPORTED)
set_property(TARGET obj PROPERTY IMPORTED_LOCATION ${source_dir}/obj/libobj.a )
add_dependencies(obj cudd_distr)

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
target_link_libraries(bdddomain cudd mtr epd st util obj)

##########################################################################
# BDDDomain Installation
##########################################################################
install(TARGETS bdddomain
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
install(DIRECTORY bdddomain/ DESTINATION include/bdddomain
  FILES_MATCHING PATTERN "*.hh" PATTERN "*.hpp")

add_subdirectory(tests)