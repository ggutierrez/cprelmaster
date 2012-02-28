#Works under the assumption than when gecode is installed at least the kernel
#  component exists
# Exports:
# GECODE_FOUND : boolean indicating wether gecode was found or not.
# GECODE_VERSION : version of gecode that was found
# GECODE_CPREL_SUPPORT : boolean indicating if gecode has support for cprel
# Gecode_LIBRARIES : The gecode libraries
# GECODE_LIBS_HOME : Directory where gecode libraries were found
# GECODE_INCLUDE_DIR : Girectory where gecode headers were found



# Look for the header file
find_path(GECODE_INCLUDE_DIR NAMES gecode/kernel.hh)
find_file(GECODE_CONFIG gecode/support/config.hpp)
# Look for the library
find_library(GECODE_LIBRARY NAMES gecodekernel)
find_library(GECODE_SUPPORT_LIBRARY NAMES gecodesupport)

if(GECODE_CONFIG)
  file(STRINGS ${GECODE_CONFIG} GECODE_LINE_VERSION REGEX "^#define GECODE_VERSION .*")
  string(REGEX MATCH "[0-9].[0-9].[0-9]" GECODE_VERSION ${GECODE_LINE_VERSION})
endif()

if(GECODE_INCLUDE_DIR AND GECODE_LIBRARY AND GECODE_SUPPORT_LIBRARY)
  set(GECODE_FOUND Yes)
  get_filename_component(GECODE_LIBS_HOME ${GECODE_LIBRARY} PATH)
endif()

if(GECODE_FOUND)
  set(Gecode_LIBRARIES ${GECODE_LIBRARY} ${GECODE_SUPPORT_LIBRARY})
  set(Gecode_INCLUDE_DIRS ${GECODE_INCLUDE_DIR})
  find_library(GECODE_DRIVER_LIBRARY gecodedriver)
  if(GECODE_DRIVER_LIBRARY)
    list(APPEND Gecode_LIBRARIES ${GECODE_DRIVER_LIBRARY})
  endif()
  find_library(GECODE_FZ_LIBRARY gecodeflatzinc)
  if(GECODE_FZ_LIBRARY)
    list(APPEND Gecode_LIBRARIES ${GECODE_FZ_LIBRARY})
  endif()
  find_library(GECODE_GIST_LIBRARY gecodegist)
  if(GECODE_GIST_LIBRARY)
    list(APPEND Gecode_LIBRARIES ${GECODE_GIST_LIBRARY})
  endif()
  find_library(GECODE_GRAPH_LIBRARY gecodegraph)
  if(GECODE_GRAPH_LIBRARY)
    list(APPEND Gecode_LIBRARIES ${GECODE_GRAPH_LIBRARY})
  endif()
  find_library(GECODE_INT_LIBRARY gecodeint)
  if(GECODE_INT_LIBRARY)
    list(APPEND Gecode_LIBRARIES ${GECODE_INT_LIBRARY})
  endif()
  find_library(GECODE_MM_LIBRARY gecodeminimodel)
  if(GECODE_MM_LIBRARY)
    list(APPEND Gecode_LIBRARIES ${GECODE_MM_LIBRARY})
  endif()
  find_library(GECODE_SCHEDULING_LIBRARY gecodescheduling)
  if(GECODE_SCHEDULING_LIBRARY)
    list(APPEND Gecode_LIBRARIES ${GECODE_SCHEDULING_LIBRARY})
  endif()
  find_library(GECODE_SEARCH_LIBRARY gecodesearch)
  if(GECODE_SEARCH_LIBRARY)
    list(APPEND Gecode_LIBRARIES ${GECODE_SEARCH_LIBRARY})
  endif()
  find_library(GECODE_SET_LIBRARY gecodeset)
  if(GECODE_SET_LIBRARY)
    list(APPEND Gecode_LIBRARIES ${GECODE_SET_LIBRARY})
  endif()

  find_file(GECODE_VIMP gecode/kernel/var-imp.hpp)
  ## Detect if gecode has already support for CPRel. Sets variable GECODE_HAS_CPREL
  ## to Yes
  set(GECODE_CPREL_CLASS "CPRelVarImpBase")
  file(STRINGS ${GECODE_VIMP} GECODE_LINE_CPREL_CLASS
      REGEX "^.*class ${GECODE_CPREL_CLASS} .*")
  message(STATUS "Gecode class: ${GECODE_LINE_CPREL_CLASS}")
  list(LENGTH GECODE_LINE_CPREL_CLASS GECODE_CPREL_SUPPORT)
endif()


