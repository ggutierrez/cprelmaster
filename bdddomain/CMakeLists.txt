
set(BDDDOMAIN_SRCS
  encoding.cpp encoding.hh abstract.cpp replace.cpp 
  exception.hh manager.cpp manager.hh
  bdd.hh bdd-print.cpp bdd-visitor.hpp
  perm-descriptor.hh rel-impl.cpp rel-impl.hh 
  tuple.cpp tuple.hh
  grelation.cpp grelation.hh 
)

add_library(bdddomain ${BDDDOMAIN_SRCS})
target_link_libraries(bdddomain ${CUDD_LIBS})

#if(UNIT_TEST)
#  list(APPEND TEST_SOURCES bdddomain/tests/bddrel.cpp)
#  list(APPEND TEST_LIBRARIES bdddomain)
#endif()
##########################################################################
# BDDDomain Installation
##########################################################################
install(TARGETS bdddomain
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
install(DIRECTORY bdddomain/ DESTINATION include/bdddomain
  FILES_MATCHING PATTERN "*.hh" PATTERN "*.hpp")
install(DIRECTORY rel/ DESTINATION include/rel
  FILES_MATCHING PATTERN "*.hh" PATTERN "*.hpp")
