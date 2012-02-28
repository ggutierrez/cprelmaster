set(CPREL_SRCS
  complement-view.hh
  const-view.hh
  cprel.hh
  var.hh
  varimp.cpp
  varimp.hh
  view.hh

# documentation
#  doc/main.hh

# propagators	
  prop/and.cpp
  prop/and.hh
  prop/and.hpp
  prop/equality.cpp
  prop/equality.hh
  prop/permutation.cpp
  prop/permutation.hh
  prop/join.hh
  prop/join.cpp
  prop/projection.cpp
  prop/projection.hh
#  prop/channel.cpp
#  prop/channel.hh
#  prop/tuple.cpp
#  prop/tuple.hh
  prop/follow.hh
  prop/follow.cpp
  prop/dom.cpp
# branchers
  branch/branch.cpp
)
add_library(gecodecprel ${CPREL_SRCS})
target_link_libraries(gecodecprel bdddomain ${Gecode_LIBRARIES})

#########################################################################
# Installation                                                           
#########################################################################
# The library is installed in the same place where gecode libraries were found
install(TARGETS gecodecprel
  LIBRARY DESTINATION ${GECODE_LIBS_HOME}
  ARCHIVE DESTINATION ${GECODE_LIBS_HOME})
install(DIRECTORY . DESTINATION ${GECODE_INCLUDE_DIR}/cprel
        FILES_MATCHING PATTERN "*.hh" PATTERN "*.hpp")
