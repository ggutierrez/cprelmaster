set(GECODE_BREL_SUPPORT NO)

# Try to find a gecode installation
find_package(Gecode)
if(GECODE_FOUND)
  message(STATUS "Found Gecode version: ${GECODE_VERSION}")
  message(STATUS "Using gecode version: ${GECODE_VERSION}")
  set(MIN_GECODE_REQUIRED 3.7.2)
  if((${GECODE_VERSION} VERSION_EQUAL ${MIN_GECODE_REQUIRED}) OR
      (${GECODE_VERSION} VERSION_GREATER ${MIN_GECODE_REQUIRED}))
    message(STATUS "  Gecode library version is OK")
  endif()
  if(GECODE_CPREL_SUPPORT)
    message(STATUS  "  Gecode was built with support for the CPRel type of variable.")
  endif()
endif()

# If we did not find Gecode or a compatible version installed then
# offer to install one
if(NOT GECODE_CPREL_SUPPORT)
  message(STATUS "No compatible version of gecode was found.")
  message(STATUS "  To build the gecode you need please execute make GECODE_INSTALL.")
  message(STATUS "  and reconfigure with option -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}")
  include(ExternalProject)
  ## configure options used to build gecode
  set(CONFIGURE_OPTIONS)
  list(APPEND CONFIGURE_OPTIONS "CXXFLAGS=-DGECODE_HAS_VAR_DISPOSE")
  list(APPEND CONFIGURE_OPTIONS "CFLAGS=-DGECODE_HAS_VAR_DISPOSE")

  set(VIS_INPUTS "${CMAKE_SOURCE_DIR}/cprel/cprel.vis")
#  set(VIS_INPUTS "${VIS_INPUTS} ${CMAKE_SOURCE_DIR}/cptuple/cptuple.vis")

  list(APPEND VIS_INPUTS ${EXTRA_VIS})
 
  message(STATUS "VIS sources: ${VIS_INPUTS}")
  list(APPEND CONFIGURE_OPTIONS  "--with-vis=${VIS_INPUTS}")

  list(APPEND CONFIGURE_OPTIONS "--with-architectures=${CMAKE_OSX_ARCHITECTURES}")
  list(APPEND CONFIGURE_OPTIONS "--disable-examples")
  list(APPEND CONFIGURE_OPTIONS "--prefix=${CMAKE_INSTALL_PREFIX}")
  ExternalProject_Add(GECODE_INSTALL
#    SVN_REPOSITORY https://svn.gecode.org/svn/gecode/tags/release-3.6.0
#    SVN_USERNAME anonymous
#    SVN_PASSWORD user@gmail.com
    URL http://www.gecode.org/download/gecode-3.7.2.tar.gz
    CONFIGURE_COMMAND ./configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ${CONFIGURE_OPTIONS}
    BUILD_IN_SOURCE Yes
    BUILD_COMMAND make
    INSTALL_COMMAND make install
  )
endif()

include_directories(${Gecode_INCLUDE_DIRS})