cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-support")

set(PROJECT_NAME "CP(Rel+Tuple)")
set(PROJECT_VERSION "0.9.0")
project(${PROJECT_NAME})
##########################################################################
# Command line options
##########################################################################
option(UNIT_TEST "Build unit tests (requires boost unit_test)" NO)
option(BUILD_EXAMPLES "Build examples " NO)
##########################################################################
# Compiler information
##########################################################################
if(CMAKE_COMPILER_IS_GNUCC)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
    OUTPUT_VARIABLE GCC_VERSION)
  if (GCC_VERSION VERSION_GREATER 4.6 OR GCC_VERSION VERSION_EQUAL 4.6)
    message(STATUS "GCC version ${GCC_VERSION}")
  else()
    message(FATAL_ERROR "GCC version 4.6 or greater is required")
  endif()
else()
  message(WARNING "Compiler: ${CMAKE_CXX_COMPILER_ID}")
  message(WARNING "${CMAKE_COMPILER_IS_CLANGCXX}")
endif()
##########################################################################
# System information
##########################################################################
message(STATUS "Building for architecture: ${CMAKE_SYSTEM}")
if(CMAKE_SYSTEM MATCHES Darwin-10.6.0)
  message(STATUS "  Not fully tested platform")
endif()
message(STATUS "Void pointer size ${CMAKE_SIZEOF_VOID_P}")

include(CheckTypeSize)
check_type_size("void*" SIZEOF_VOID_P)
if(SIZEOF_VOID_P STREQUAL "")
  message(SEND_ERROR "Failed to determine sizeof(void*)")
endif()
message(STATUS "Size of pointer to void (compiler): ${SIZEOF_VOID_P}")
if(SIZEOF_VOID_P MATCHES "8")
  add_definitions(-fPIC)
endif()
##########################################################################
# Additional compiler flags
##########################################################################
include(CheckCXXCompilerFlag)

check_cxx_compiler_flag("-std=c++0x" C0X_SUPPORT)
if(C0X_SUPPORT)
  message(STATUS "CXX has c++0x support")
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++0x")
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
##########################################################################
# Cudd library
##########################################################################
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cudd-2.4.2/mtr")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cudd-2.4.2/epd")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cudd-2.4.2/st")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cudd-2.4.2/util")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cudd-2.4.2/cudd")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cudd-2.4.2/obj")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cudd-2.4.2")

add_subdirectory(cudd-2.4.2)
##########################################################################
# Boost
##########################################################################
set(BOOST_COMPONENTS)
if(UNIT_TEST)
  message(STATUS "Building unit tests")
  list(APPEND BOOST_COMPONENTS unit_test_framework)
endif()
#add_definitions("-DBOOST_CHRONO_HEADER_ONLY")
list(APPEND BOOST_COMPONENTS system)
list(APPEND BOOST_COMPONENTS chrono)

find_package(Boost 1.47.0 COMPONENTS ${BOOST_COMPONENTS})
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  if(UNIT_TEST)
    set(TEST_SOURCES tests/test-main.cpp)
    set(TEST_LIBRARIES ${Boost_LIBRARIES})
  endif()
else()
  message(STATUS "Boost is needed to build this application")
  message(STATUS "No compatible version of Boost was found.")
  message(STATUS "  To build this library you need at least version ${MIN_BOOST_REQUIRED}")
  message(STATUS "  please execute make BOOST_INSTALL.")
  message(STATUS "  and reconfigure with option -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}")
  include(ExternalProject)
  ## configure options used to build boost
  set(BOOST_CONFIGURE_OPTIONS)
  list(APPEND BOOST_CONFIGURE_OPTIONS "--prefix=${CMAKE_INSTALL_PREFIX}")
  ExternalProject_Add(BOOST_INSTALL
    SVN_REPOSITORY http://svn.boost.org/svn/boost/tags/release/Boost_1_47_0
    SVN_USERNAME anonymous
    SVN_PASSWORD user@gmail.com
    CONFIGURE_COMMAND  ./bootstrap.sh ${BOOST_CONFIGURE_OPTIONS}
    BUILD_IN_SOURCE Yes
    BUILD_COMMAND ./b2 -j${JOBS} INSTALL_COMMAND ./b2 -j${JOBS} install
  )
endif()
##########################################################################
# Domain representation using BDDs
##########################################################################
include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(bdddomain)
##########################################################################
# JINC
##########################################################################
if (BUILD_JINC)
  include(ExternalProject)
  ExternalProject_Add(JINC_INSTALL
    URL http://www.jossowski.de/projects/jinc/download/assets/jinc_2010-05-29_114104.tgz
    BUILD_IN_SOURCE Yes
    PATCH_COMMAND patch < ${CMAKE_SOURCE_DIR}/jinc.patch
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make lib
    INSTALL_COMMAND ""
    )
endif()
##########################################################################
# Gecode
##########################################################################
set(GECODE_BREL_SUPPORT NO)
find_package(Gecode)
if(GECODE_FOUND)
  message(STATUS "Found Gecode version: ${GECODE_VERSION}")
  message(STATUS "Using gecode version: ${GECODE_VERSION}")
  set(MIN_GECODE_REQUIRED 3.6.0)
  if((${GECODE_VERSION} VERSION_EQUAL ${MIN_GECODE_REQUIRED}) OR
      (${GECODE_VERSION} VERSION_GREATER ${MIN_GECODE_REQUIRED}))
    message(STATUS "  Gecode library version is OK")
  endif()
  if(GECODE_CPREL_SUPPORT)
    message(STATUS  "  Gecode was built with support for the CPRel type of variable.")
  endif()
  if(GECODE_CPTUPLE_SUPPORT)
    message(STATUS  "  Gecode was built with support for the CPTuple type of variable.")
  endif()
endif()

if(NOT GECODE_CPREL_SUPPORT OR NOT GECODE_CPTUPLE_SUPPORT)
  message(STATUS "No compatible version of gecode was found.")
  message(STATUS "  To build the gecode you need please execute make GECODE_INSTALL.")
  message(STATUS "  and reconfigure with option -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}")
  include(ExternalProject)
  ## configure options used to build gecode
  set(CONFIGURE_OPTIONS)
  list(APPEND CONFIGURE_OPTIONS "CXXFLAGS=-DGECODE_HAS_VAR_DISPOSE")
  list(APPEND CONFIGURE_OPTIONS "CFLAGS=-DGECODE_HAS_VAR_DISPOSE")

  set(VIS_INPUTS "${CMAKE_SOURCE_DIR}/cprel/cprel.vis")
  set(VIS_INPUTS "${VIS_INPUTS} ${CMAKE_SOURCE_DIR}/cptuple/cptuple.vis")

  list(APPEND VIS_INPUTS ${EXTRA_VIS})
  #list(APPEND CONFIGURE_OPTIONS "--with-vis=${CMAKE_SOURCE_DIR}/cprel/cprel.vis")
  message(STATUS "VIS sources: ${VIS_INPUTS}")
  list(APPEND CONFIGURE_OPTIONS  "--with-vis=${VIS_INPUTS}")

  list(APPEND CONFIGURE_OPTIONS "--with-architectures=${CMAKE_OSX_ARCHITECTURES}")
  list(APPEND CONFIGURE_OPTIONS "--disable-examples")
  list(APPEND CONFIGURE_OPTIONS "--prefix=${CMAKE_INSTALL_PREFIX}")
  ExternalProject_Add(GECODE_INSTALL
    SVN_REPOSITORY https://svn.gecode.org/svn/gecode/tags/release-3.6.0
    SVN_USERNAME anonymous
    SVN_PASSWORD user@gmail.com
    CONFIGURE_COMMAND ./configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ${CONFIGURE_OPTIONS}
    BUILD_IN_SOURCE Yes
    BUILD_COMMAND make -j${JOBS} INSTALL_COMMAND make install
  )
endif()
##########################################################################
# Gecode includes
##########################################################################
include_directories(${Gecode_INCLUDE_DIRS})
##########################################################################
# Relation constraint system
##########################################################################
set(CPREL_SRCS
  cprel/complement-view.hh
  cprel/const-view.hh
  cprel/cprel.hh
  cprel/var.hh
  cprel/varimp.cpp
  cprel/varimp.hh
  cprel/view.hh

# documentation
  doc/main.hh

# propagators	
  cprel/prop/and.cpp
  cprel/prop/and.hh
  cprel/prop/and.hpp
  cprel/prop/equality.cpp
  cprel/prop/equality.hh
  cprel/prop/permutation.cpp
  cprel/prop/permutation.hh
  cprel/prop/join.hh
  cprel/prop/join.cpp
  cprel/prop/projection.cpp
  cprel/prop/projection.hh
#  cprel/prop/channel.cpp
#  cprel/prop/channel.hh
#  cprel/prop/tuple.cpp
#  cprel/prop/tuple.hh
  cprel/prop/follow.hh
  cprel/prop/follow.cpp
  cprel/prop/dom.cpp
# branchers
  cprel/branch/branch.cpp
)
add_library(gecodecprel ${CPREL_SRCS})
target_link_libraries(gecodecprel bdddomain ${Gecode_LIBRARIES})

if(UNIT_TEST)
  set(PROPAGATOR_TESTS
    cprel/prop/tests/and.cpp)
  list(APPEND TEST_SOURCES ${PROPAGATOR_TESTS})
  list(APPEND TEST_LIBRARIES gecodecprel)
endif()
##########################################################################
# Tuple constraint system
##########################################################################
set(CPTUPLE_SRCS
  cptuple/cptuple.hh
  cptuple/var.hh
  cptuple/varimp.cpp
  cptuple/varimp.hh
  cptuple/view.hh

# branchers
  cptuple/branch/branch.cpp
)
#add_library(gecodecptuple ${CPTUPLE_SRCS})
#target_link_libraries(gecodecptuple bdddomain ${Gecode_LIBRARIES})
#########################################################################
# Installation                                                           #
##########################################################################
install(TARGETS gecodecprel
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
install(DIRECTORY cprel/ DESTINATION include/cprel
        FILES_MATCHING PATTERN "*.hh" PATTERN "*.hpp")
#install(TARGETS gecodecptuple
#  LIBRARY DESTINATION lib
#  ARCHIVE DESTINATION lib)
#install(DIRECTORY cptuple/ DESTINATION include/cptuple
#        FILES_MATCHING PATTERN "*.hh" PATTERN "*.hpp")
##########################################################################
# Uninstall
##########################################################################
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
##########################################################################
# Documentation
##########################################################################
find_package(Doxygen)
if(DOXYGEN_FOUND)
  message(STATUS "Doxygen was found, to build the documentation run:
  make html")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  set(DOC_SOURCE_CODE doc/declground-ex.cpp)
  file(COPY ${DOC_SOURCE_CODE} DESTINATION  ${CMAKE_BINARY_DIR})
  add_custom_target(html
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
endif()
##########################################################################
# Unit test
##########################################################################
if(UNIT_TEST)
  add_executable(unit-test ${TEST_SOURCES})
  target_link_libraries(unit-test ${TEST_LIBRARIES})
  message(STATUS "Test libraries ${TEST_LIBRARIES}")
endif()
##########################################################################
# Source distribution
##########################################################################
MACRO(TODAY RESULT)
 if(WIN32)
   execute_process(COMMAND "date" "/T" OUTPUT_VARIABLE ${RESULT})
   string(REGEX REPLACE "(..)/(..)/..(..).*" "\\3\\2\\1" ${RESULT} ${${RESULT}})
 elseif(UNIX)
   execute_process(COMMAND "date" "+%d/%m/%Y" OUTPUT_VARIABLE ${RESULT})
   string(REGEX REPLACE "(..)/(..)/..(..).*" "\\3\\2\\1" ${RESULT} ${${RESULT}})
 else()
   message(SEND_ERROR "date not implemented")
   set(${RESULT} 000000)
 endif()
ENDMACRO()
 
TODAY(TIME_SUFFIX)
set(FILE_SUFFIX "${PROJECT_VERSION}-${TIME_SUFFIX}")
set(ARCHIVE_NAME ${CMAKE_PROJECT_NAME}-${FILE_SUFFIX})
add_custom_target(dist
  git archive --format zip --output ${CMAKE_BINARY_DIR}/${ARCHIVE_NAME}.zip --prefix=${ARCHIVE_NAME}/ master
  COMMAND tar zcvf "${CMAKE_BINARY_DIR}/doc-${FILE_SUFFIX}.tgz" ${CMAKE_BINARY_DIR}/html
  DEPENDS html
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

##########################################################################
# Executables
##########################################################################
if(BUILD_EXAMPLES)
  add_subdirectory(tests EXCLUDE_FROM_ALL)
endif()

