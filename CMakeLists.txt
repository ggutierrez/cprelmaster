cmake_minimum_required (VERSION 2.8)

project(CUDDCP)

include("${CMAKE_SOURCE_DIR}/cmake-support/util.cmake")

##########################################################################
# Compiler support
##########################################################################
#set(CMAKE_C_COMPILER "/Users/ggutierrez/Work/clang-build/bin/clang")
#set(CMAKE_CXX_COMPILER "/Users/ggutierrez/Work/clang-build/bin/clang++")

##########################################################################
# Version information
##########################################################################
set(CUDDCP_VERSION_MAJOR 1)
set(CUDDCP_VERSION_MINOR 0)
set(CUDDCP_VERSION "${CUDDCP_VERSION_MAJOR}.${CUDDCP_VERSION_MINOR}")
##########################################################################

##########################################################################
## user configurable options
##########################################################################
#option(BUILD_SHARED_LIB "Build as a shared library" ON)

##########################################################################
## C compiler definitions
##########################################################################
# NOTE: this new build system only takes into account the architecture
# i am able to test it on: Mac os x 64 bits
if (APPLE)
   # TODO: perform the check between 32 or 64 bits
   add_definitions( "-mtune=native -DHAVE_IEEE_754")
   add_definitions("-DBSD -DSIZEOF_VOID_P=8 -DSIZEOF_LONG=8" )
   ## prevent clang from generating warning messages when building cudd
   add_definitions("-Wno-unused-value")
else(APPLE)
  message(STATUS "Non tested on this architecture!")
   add_definitions( "-mtune=native -DHAVE_IEEE_754")
   add_definitions("-DBSD -DSIZEOF_VOID_P=8 -DSIZEOF_LONG=8" )
endif()
##########################################################################
# Mac OS X
##########################################################################
#if (APPLE)
#  message("-- Apple found, building for: x86_64")
#  set(CMAKE_OSX_ARCHITECTURES i386)
#endif(APPLE)

##########################################################################
# Main library
##########################################################################
# NOTE: cudd sources is split in several folders, this build system
# will respect that and will try to resemble the original one as close
# as possible.

# cudd
set(CUDDSRC
  cuddAPI.c cuddAddAbs.c cuddAddApply.c cuddAddFind.c cuddAddIte.c
  cuddAddInv.c cuddAddNeg.c cuddAddWalsh.c cuddAndAbs.c
  cuddAnneal.c cuddApa.c cuddApprox.c cuddBddAbs.c cuddBddCorr.c
  cuddBddIte.c cuddBridge.c cuddCache.c cuddCheck.c cuddClip.c
  cuddCof.c cuddCompose.c cuddDecomp.c cuddEssent.c
  cuddExact.c cuddExport.c cuddGenCof.c cuddGenetic.c
  cuddGroup.c cuddHarwell.c cuddInit.c cuddInteract.c
  cuddLCache.c cuddLevelQ.c
  cuddLinear.c cuddLiteral.c cuddMatMult.c cuddPriority.c
  cuddRead.c cuddRef.c cuddReorder.c cuddSat.c cuddSign.c
  cuddSolve.c cuddSplit.c cuddSubsetHB.c cuddSubsetSP.c cuddSymmetry.c
  cuddTable.c cuddUtil.c cuddWindow.c cuddZddCount.c cuddZddFuncs.c
  cuddZddGroup.c cuddZddIsop.c cuddZddLin.c cuddZddMisc.c
  cuddZddPort.c cuddZddReord.c cuddZddSetop.c cuddZddSymm.c
  cuddZddUtil.c
  cuddCdd.c
)

set(CUDDHDR cudd.h cuddInt.h cuddCdd.h)

# mtr
set(MTRSRC mtrBasic.c mtrGroup.c)
set(MTRHDR mtr.h)

# st
set(STSRC st.c)
set(STHDR st.h)

# epd
set(EPDSRC epd.c)
set(EPDHDR epd.h)

# util
set(UTILSRC
  cpu_time.c cpu_stats.c getopt.c safe_mem.c strsav.c texpand.c
  ptime.c prtime.c pipefork.c pathsearch.c stub.c
  tmpfile.c datalimit.c)

set(UTILHDR util.h)

# headers
include_directories("${CMAKE_SOURCE_DIR}/mtr")
include_directories("${CMAKE_SOURCE_DIR}/epd")
include_directories("${CMAKE_SOURCE_DIR}/st")
include_directories("${CMAKE_SOURCE_DIR}/util")
include_directories("${CMAKE_SOURCE_DIR}/cudd")

set(LIB_TYPE STATIC)
#set(LIB_TYPE SHARED)

# libraries: util
ADDPREFIX(FILES ${UTILSRC} PREFIX "util/" RESULT _UTILSRC)
ADDPREFIX(FILES ${UTILHDR} PREFIX "util/" RESULT _UTILHDR)
CREATE_LIB(util ${LIB_TYPE} ${_UTILSRC}  HEADERS ${_UTILHDR}
    OUTPUT_NAME "util" COMPONENT devel)

# libraries: st
ADDPREFIX(FILES ${STSRC} PREFIX "st/" RESULT _STSRC)
ADDPREFIX(FILES ${STHDR} PREFIX "st/" RESULT _STHDR)
CREATE_LIB(st ${LIB_TYPE} ${_STSRC}  HEADERS ${_STHDR}
    OUTPUT_NAME "st" COMPONENT devel)

# libraries: cudd
ADDPREFIX(FILES ${CUDDSRC} PREFIX "cudd/" RESULT _CUDDSRC)
ADDPREFIX(FILES ${CUDDHDR} PREFIX "cudd/" RESULT _CUDDHDR)
CREATE_LIB(cudd ${LIB_TYPE} ${_CUDDSRC}  HEADERS ${_CUDDHDR}
    OUTPUT_NAME "cudd" COMPONENT devel)

# libraries: mtr
ADDPREFIX(FILES ${MTRSRC} PREFIX "mtr/" RESULT _MTRSRC)
ADDPREFIX(FILES ${MTRHDR} PREFIX "mtr/" RESULT _MTRHDR)
CREATE_LIB(mtr ${LIB_TYPE} ${_MTRSRC}  HEADERS ${_MTRHDR}
    OUTPUT_NAME "mtr" COMPONENT devel)

# libraries: epd
ADDPREFIX(FILES ${EPDSRC} PREFIX "epd/" RESULT _EPDSRC)
ADDPREFIX(FILES ${EPDHDR} PREFIX "epd/" RESULT _EPDHDR)
CREATE_LIB(epd ${LIB_TYPE} ${_EPDSRC}  HEADERS ${_EPDHDR}
    OUTPUT_NAME "epd" COMPONENT devel)

##########################################################################
# C++ interface for cdds
##########################################################################
include_directories("${CMAKE_SOURCE_DIR}/cdd-obj")
set(CDDCPPSRCS cdd.cc)
set(CDDCPPHDRS cdd.hh)
ADDPREFIX(FILES ${CDDCPPSRCS} PREFIX "cdd-obj/" RESULT _CDDCPPSRC)
ADDPREFIX(FILES ${CDDCPPHDRS} PREFIX "cdd-obj/" RESULT _CDDCPPHDR)
CREATE_LIB(cddcpp ${LIB_TYPE} ${_CDDCPPSRC}  HEADERS ${_CDDCPPHDR}
    OUTPUT_NAME "cddcpp" COMPONENT devel)


##########################################################################
# Cdds examples
##########################################################################
set(EXLIBS cudd-static epd-static mtr-static st-static util-static cddcpp-static)

add_executable(testcdd tests/testcdd.c)
target_link_libraries(testcdd ${EXLIBS})

add_executable(testobj tests/testcdd.cpp)
target_link_libraries(testobj ${EXLIBS})

##########################################################################
# Test
##########################################################################
add_executable(testcudd cudd/testcudd.c)
target_link_libraries(testcudd cudd-static epd-static mtr-static
st-static util-static)

##########################################################################
# Cprel
##########################################################################
add_subdirectory(cprel)

#if(BUILD_TEST)
#  add_subdirectory(tests)
#endif(BUILD_TEST)

##########################################################################
# Uninstall
##########################################################################
#CONFIGURE_FILE(
#  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
#  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#  IMMEDIATE @ONLY)

#ADD_CUSTOM_TARGET(uninstall
#  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

##########################################################################
# Binary packages
##########################################################################
set(CPACK_PACKAGE_NAME "CuddCP")
set(CPACK_PACKAGE_VENDOR "info.ucl.ac.be")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CuddCP ${CPBINREL_VERSION}")

set(CPACK_PACKAGE_VERSION "${CUDDCP_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${CUDDCP_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${CUDDCP_VERSION_MINOR}")

#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
#set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/BUILD")

if (APPLE)
  set(CPACK_GENERATOR "PackageMaker")
endif (APPLE)

if (UNIX)
  list(APPEND CPACK_GENERATOR  "DEB")
endif (UNIX)

set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "cuddcp-${CUDDCP_VERSION}-src")
set(CPACK_PACKAGE_FILE_NAME "cuddcp-${CUDDCP_VERSION}")

#set(CPACK_COMPONENTS_ALL devel)

#set(CPACK_COMPONENT_DEVEL_DISPLAY_NAME "Library and header files")


set(CPACK_PACKAGE_CONTACT "Gustavo Gutierrez <gutierrez.gustavo@uclouvain.be>")

include(CPack)
