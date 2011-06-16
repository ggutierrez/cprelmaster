cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-support")

project(cuddcp)
set(PROJECT_VERSION "0.9.0")
##########################################################################
# Command line options
##########################################################################
option(UNIT_TEST "Build unit tests (requires boost unit_test)" NO)

##########################################################################
# Compiler information
##########################################################################
if(CMAKE_COMPILER_IS_GNUCC)
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion
                    OUTPUT_VARIABLE GCC_VERSION)
  if (GCC_VERSION VERSION_GREATER 4.2 OR GCC_VERSION VERSION_EQUAL 4.2)
    message(STATUS "GCC version ${GCC_VERSION}")
  else()
    messge(FATAL_ERROR "GCC version 4.2 or greater is required")
  endif()
else()
  message(WARNING "This project has been only built with GCC, other compilers
                  might work: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_COMPILER_IS_CLANGCXX}")
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

## temporal flags for development
add_definitions(-O0 -g)
##########################################################################
# Cudd library
##########################################################################
include_directories("${CMAKE_SOURCE_DIR}/mtr")
include_directories("${CMAKE_SOURCE_DIR}/epd")
include_directories("${CMAKE_SOURCE_DIR}/st")
include_directories("${CMAKE_SOURCE_DIR}/util")
include_directories("${CMAKE_SOURCE_DIR}/cudd")

set(CUDD_SRCS
  cudd/cuddAddAbs.c
  cudd/cuddAddApply.c
  cudd/cuddAddFind.c
  cudd/cuddAddInv.c
  cudd/cuddAddIte.c
  cudd/cuddAddNeg.c
  cudd/cuddAddWalsh.c
  cudd/cuddAndAbs.c
  cudd/cuddAnneal.c
  cudd/cuddApa.c
  cudd/cuddAPI.c
  cudd/cuddApprox.c
  cudd/cuddBddAbs.c
  cudd/cuddBddCorr.c
  cudd/cuddBddIte.c
  cudd/cuddBridge.c
  cudd/cuddCache.c
#  cudd/cuddCdd.c
  cudd/cuddCheck.c
  cudd/cuddClip.c
  cudd/cuddCof.c
  cudd/cuddCompose.c
  cudd/cuddDecomp.c
  cudd/cuddEssent.c
  cudd/cuddExact.c
  cudd/cuddExport.c
  cudd/cuddGenCof.c
  cudd/cuddGenetic.c
  cudd/cuddGroup.c
  cudd/cuddHarwell.c
  cudd/cuddInit.c
  cudd/cuddInteract.c
  cudd/cuddLCache.c
  cudd/cuddLevelQ.c
  cudd/cuddLinear.c
  cudd/cuddLiteral.c
  cudd/cuddMatMult.c
  cudd/cuddPriority.c
  cudd/cuddRead.c
  cudd/cuddRef.c
  cudd/cuddReorder.c
  cudd/cuddSat.c
  cudd/cuddSign.c
  cudd/cuddSolve.c
  cudd/cuddSplit.c
  cudd/cuddSubsetHB.c
  cudd/cuddSubsetSP.c
  cudd/cuddSymmetry.c
  cudd/cuddTable.c
  cudd/cuddUtil.c
  cudd/cuddWindow.c
  cudd/cuddZddCount.c
  cudd/cuddZddFuncs.c
  cudd/cuddZddGroup.c
  cudd/cuddZddIsop.c
  cudd/cuddZddLin.c
  cudd/cuddZddMisc.c
  cudd/cuddZddPort.c
  cudd/cuddZddReord.c
  cudd/cuddZddSetop.c
  cudd/cuddZddSymm.c
  cudd/cuddZddUtil.c

  bdddomain/unique.c
  bdddomain/unique.h

#Headers:
  cudd/cuddCdd.h
  cudd/cudd.h
  cudd/cuddInt.h
)

set(MTR_SRCS
  mtr/mtrBasic.c
  mtr/mtrGroup.c
#Headers:
  mtr/mtr.h
)

set(ST_SRCS
  st/st.c
#Headers:
  st/st.h
)

set(EPD_SRCS
  epd/epd.c
#Headers:
  epd/epd.h
)

set(UTIL_SRCS
  util/cpu_time.c
  util/cpu_stats.c
  util/getopt.c
  util/safe_mem.c
  util/strsav.c
  util/texpand.c
  util/ptime.c
  util/prtime.c
  util/pipefork.c
  util/pathsearch.c
  util/stub.c
  util/tmpfile.c
  util/datalimit.c
#Headers
  util/util.h
)

add_library(cudd ${CUDD_SRCS})
add_library(util ${UTIL_SRCS})
add_library(st ${ST_SRCS})
add_library(mpr ${MTR_SRCS})
add_library(epd ${EPD_SRCS})

target_link_libraries(mpr util)
target_link_libraries(epd util)
target_link_libraries(st util)
target_link_libraries(cudd util st mpr epd)
## NOTE about warnings:
## When CUDF is compiled by gcc there are a lot of warnings signaling
## unused parameters. As they can hide other important warnings of this project
## they will be disabled during the building of CUDD specific targets.

include(CheckCCompilerFlag)
set(CUDD_SPECIFIC_FLAGS)

check_c_compiler_flag(-Wno-unused-parameter FLAG_DIS_UNUSED)
if(FLAG_DIS_UNUSED)
 set(CUDD_SPECIFIC_FLAGS "${CUDD_SPECIFIC_FLAGS} -Wno-unused-parameter")
endif()

check_c_compiler_flag(-Wno-uninitialized FLAG_DIS_UNINITIALIZED)
if(FLAG_DIS_UNINITIALIZED)
 set(CUDD_SPECIFIC_FLAGS "${CUDD_SPECIFIC_FLAGS} -Wno-uninitialized")
endif()

check_c_compiler_flag(-Wno-sign-compare FLAG_DIS_SIGN)
if(FLAG_DIS_SIGN)
  set(CUDD_SPECIFIC_FLAGS "${CUDD_SPECIFIC_FLAGS} -Wno-sign-compare")
endif()

## Building options extracted from original CUDD Makefile
if(SIZEOF_VOID_P MATCHES "8")
  set(CUDD_SPECIFIC_FLAGS "${CUDD_SPECIFIC_FLAGS} -DHAVE_IEEE_754 -DBSD -DSIZEOF_VOID_P=8 -DSIZEOF_LONG=8")

  check_c_compiler_flag("-mtune=native" FLAG_TUNE_NATIVE)
  if(FLAG_TUNE_NATIVE)
    set(CUDD_SPECIFIC_FLAGS "${CUDD_SPECIFIC_FLAGS} -mtune=native")
  else()
    message(WARNING "CUDD compilation option not supported!")
  endif()

elseif(SIZEOF_VOID_P MATCHES "4")
  set(CUDD_SPECIFIC_FLAGS "${CUDD_SPECIFIC_FLAGS} -DHAVE_IEEE_754 -DBSD")

  check_c_compiler_flag("-mtune=pentium4" FLAG_TUNE_PIV)
  if(FLAG_TUNE_PIV)
    set(CUDD_SPECIFIC_FLAGS "${CUDD_SPECIFIC_FLAGS} -mtune=pentium4")
  else()
    message(WARNING "CUDD compilation option not supported!")
  endif()

  check_c_compiler_flag(-malign-double FLAG_ALIGN_DOUBLE)
  if(FLAG_ALIGN_DOUBLE)
    set(CUDD_SPECIFIC_FLAGS "${CUDD_SPECIFIC_FLAGS} -malign-double")
  else()
    message(WARNING "CUDD compilation option not supported!")
  endif()
else()
  message(SEND_ERROR "Unable to determine the flags to build cudd")
endif()

set_source_files_properties(
    ${CUDD_SRCS} ${UTIL_SRCS} ${ST_SRCS} ${MTR_SRCS} ${EPD_SRCS}
    PROPERTIES COMPILE_FLAGS ${CUDD_SPECIFIC_FLAGS}
)
set(CUDD_LIBS cudd util st mpr epd)
##########################################################################
# Boost
##########################################################################
set(BOOST_COMPONENTS)
if(UNIT_TEST)
  message(STATUS "Building unit tests")
  list(APPEND BOOST_COMPONENTS unit_test_framework)
endif()
find_package(Boost 1.36.0 COMPONENTS ${BOOST_COMPONENTS})
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  if(UNIT_TEST)
    set(TEST_SOURCES tests/test-main.cpp)
    set(TEST_LIBRARIES ${Boost_LIBRARIES})
  endif()
else()
  message(FATAL_ERROR "Boost is needed to build this application")
endif()
##########################################################################
# Domain representation using BDDs
##########################################################################
include_directories(${CMAKE_SOURCE_DIR})
set(BDDDOMAIN_SRCS
  bdddomain/encoding.cpp
  bdddomain/encoding.hh
  bdddomain/exception.hh
  bdddomain/manager.cpp
  bdddomain/manager.hh
  bdddomain/perm-descriptor.hh
  bdddomain/rel-impl.cpp
  bdddomain/rel-impl.hh
  bdddomain/relations.cpp
  bdddomain/relations.hh
  bdddomain/tuple.cpp
  bdddomain/tuple.hh
)
add_library(bdddomain ${BDDDOMAIN_SRCS})
target_link_libraries(bdddomain ${CUDD_LIBS})
if(UNIT_TEST)
  list(APPEND TEST_SOURCES bdddomain/tests/bddrel.cpp)
  list(APPEND TEST_LIBRARIES bdddomain)
endif()
##########################################################################
# BDDDomain Installation
##########################################################################
install(TARGETS bdddomain ${CUDD_LIBS}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
install(DIRECTORY bdddomain/ DESTINATION include/bdddomain
  FILES_MATCHING PATTERN "*.hh" PATTERN "*.hpp")
##########################################################################
# Gecode
##########################################################################
find_package(Gecode)
if(GECODE_FOUND)
  message(STATUS "Found Gecode version: ${GECODE_VERSION}")
  message(STATUS "Using gecode version: ${GECODE_VERSION}")
  set(MIN_GECODE_REQUIRED 3.5.0)
  set(GECODE_KERNEL_CLASS "CPRelVarImpBase")
  if ((${GECODE_VERSION} VERSION_EQUAL ${MIN_GECODE_REQUIRED}) OR
      (${GECODE_VERSION} VERSION_GREATER ${MIN_GECODE_REQUIRED}))
    message(STATUS "  Gecode library version is OK")
    # Detect if gecode was build to support the CPRel variable type
    find_file(GECODE_VIMP gecode/kernel/var-imp.hpp)
    file(STRINGS ${GECODE_VIMP} GECODE_LINE_BREL_CLASS
      REGEX "^.*class ${GECODE_KERNEL_CLASS} .*")
    message(STATUS "Gecode class: ${GECODE_LINE_BREL_CLASS}")
    list(LENGTH GECODE_LINE_BREL_CLASS GECODE_BREL_SUPPORT)
    if(GECODE_BREL_SUPPORT)
      message(STATUS "  Gecode was built with support for the BRel type of variable.")
      set(WORKING_GECODE Yes)
    endif()
  endif()
else()
  message(STATUS "No compatible version of gecode was found.")
  message(STATUS "  To build this library Gecode you need at least version ${MIN_GECODE_REQUIRED}")
  message(STATUS "  and built with support for the relational constraint system.")
  message(STATUS "  To build the gecode you need please execute make GECODE_INSTALL.")
  message(STATUS "  and reconfigure with option -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}")
  include(ExternalProject)
  set(CONFIGURE_OPTIONS)
  list(APPEND CONFIGURE_OPTIONS "CXXFLAGS=-DGECODE_HAS_VAR_DISPOSE")
  list(APPEND CONFIGURE_OPTIONS "CFLAGS=-DGECODE_HAS_VAR_DISPOSE")
  list(APPEND CONFIGURE_OPTIONS "--with-vis=${CMAKE_SOURCE_DIR}/cprel/cprel.vis")

  #list(APPEND CONFIGURE_OPTIONS "CXXFLAGS=-DGECODE_HAS_GRAPH_VARS -DGECODE_HAS_VAR_DISPOSE")
  #list(APPEND CONFIGURE_OPTIONS "CFLAGS=-DGECODE_HAS_GRAPH_VARS  -DGECODE_HAS_VAR_DISPOSE")
  #list(APPEND CONFIGURE_OPTIONS "--with-vis=/home/gg1/Work/temp-build/graph-vars/graph.vis")

  #list(APPEND CONFIGURE_OPTIONS "--disable-gist")
  list(APPEND CONFIGURE_OPTIONS "--with-architectures=${CMAKE_OSX_ARCHITECTURES}")
  list(APPEND CONFIGURE_OPTIONS "--disable-examples")
  list(APPEND CONFIGURE_OPTIONS "--prefix=${CMAKE_INSTALL_PREFIX}")
  ExternalProject_Add(GECODE_INSTALL
    SVN_REPOSITORY https://svn.gecode.org/svn/gecode/tags/release-3.5.0
    SVN_USERNAME anonymous
    SVN_PASSWORD user@gmail.com
    CONFIGURE_COMMAND ./configure ${CONFIGURE_OPTIONS}
    BUILD_IN_SOURCE Yes
    BUILD_COMMAND make -j1 INSTALL_COMMAND make install
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
  cprel/permuted-view.hh
  cprel/const-view.hh
  cprel/cprel.hh
  cprel/grelation.cpp
  cprel/grelation.hh
  cprel/var.hh
  cprel/varimp.cpp
  cprel/varimp.hh
  cprel/view.hh

# builtin ground relations
  cprel/grel-builtin/arith.cpp

# documentation
  doc/main.hh

# propagators
  cprel/prop/and.cpp
  cprel/prop/and.hh
  cprel/prop/equality.cpp
  cprel/prop/equality.hh
  cprel/prop/join.hh
  cprel/prop/join.cpp
  cprel/prop/projection.cpp
  cprel/prop/projection.hh

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
# Installation                                                           #
##########################################################################
#install(FILES ${BINREL_HDRS} DESTINATION include/binrel COMPONENT devel)
install(TARGETS gecodecprel
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
install(DIRECTORY cprel/ DESTINATION include/cprel
        FILES_MATCHING PATTERN "*.hh" PATTERN "*.hpp")
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
# Swig
##########################################################################
find_package(SWIG 1.3.31)
set(BUILD_LISP_INTERFACE No)
if (SWIG_FOUND)
  set(BUILD_LISP_INTERFACE Yes)
  include(${SWIG_USE_FILE})
  message(STATUS "Swig information:")
  message(STATUS "   program: ${SWIG_EXECUTABLE}")
else()
  message(STATUS "Swig interface cannot be build")
endif()
##########################################################################
# Lisp
##########################################################################
#find_program(LISPINT sbcl)
#if (NOT LISPINT)
#  message(FATAL_ERROR "Lisp interpreter not found")
#else()
#  message(STATUS "Lisp interpreter: ${LISPINT}")
#endif()
##########################################################################
# Test
##########################################################################
if(BUILD_LISP_INTERFACE)
  ## The foreign library
  add_library(test gelisp/test.cpp gelisp/test.hh)
  ## The swig part
  set_source_files_properties(test.i PROPERTIES CPLUSPLUS ON)
  #set_source_files_properties(example.i PROPERTIES SWIG_FLAGS "-includeall")
  swig_add_module(lisptest cffi test.i)
  swig_link_libraries(lisptest test)
endif()
##########################################################################
# Gelisp
##########################################################################
if(BUILD_LISP_INTERFACE)
  ## The foreign library
  add_library(cxxgelisp gelisp/gelisp.cpp gelisp/gelisp.hh)
  target_link_libraries(cxxgelisp gecodecprel ${Gecode_LIBRARIES})

  ## The swig part
  set_source_files_properties(gelisp.i PROPERTIES CPLUSPLUS ON)
  #set_source_files_properties(example.i PROPERTIES SWIG_FLAGS "-includeall")
  swig_add_module(gelisp cffi gelisp.i)
  swig_link_libraries(gelisp cxxgelisp gecodecprel ${Gecode_LIBRARIES})


  add_executable(gelispcl gelisp/gelispcl.cpp)
  target_link_libraries(gelispcl cxxgelisp gecodecprel ${Gecode_LIBRARIES})
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
# test for the equality propagator
add_executable(eq tests/equality.cpp)
target_link_libraries(eq gecodecprel ${Gecode_LIBRARIES})

# test for the and propagator
add_executable(and tests/and.cpp)
target_link_libraries(and gecodecprel ${Gecode_LIBRARIES})

# test for the projection propagator
add_executable(projection tests/projection.cpp)
target_link_libraries(projection gecodecprel ${Gecode_LIBRARIES})

# test for the join propagator
add_executable(join tests/join.cpp)
target_link_libraries(join gecodecprel ${Gecode_LIBRARIES})

add_executable(minimal tests/minimal.cpp)
target_link_libraries(minimal gecodecprel ${Gecode_LIBRARIES})

add_executable(domain-test tests/domain-test.cc)
target_link_libraries(domain-test bdddomain)

add_executable(grel-test tests/grel-test.cpp)
target_link_libraries(grel-test gecodecprel)


add_executable(swap-test tests/swap-test.cpp)
  target_link_libraries(swap-test gecodecprel)

add_executable(join-test tests/join-test.cpp)
  target_link_libraries(join-test gecodecprel)

add_executable(proj-test tests/proj-test.cpp)
  target_link_libraries(proj-test gecodecprel)

add_executable(midi-4seasons-test tests/midi-four-seasons.cpp)
  target_link_libraries(midi-4seasons-test gecodecprel)