cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-support")

project(cuddcp)

##########################################################################
# Command line options
##########################################################################

##########################################################################
# System information
##########################################################################
message(STATUS "Building for architecture: ${CMAKE_SYSTEM_PROCESSOR}")
##########################################################################
# Cuddy flags
##########################################################################
if (CMAKE_SYSTEM_PROCESSOR MATCHES x86_64)
  add_definitions("-mtune=native -DHAVE_IEEE_754")
  add_definitions("-DBSD -DSIZEOF_VOID_P=8 -DSIZEOF_LONG=8")
else()
  message(STATUS "This has not been built in any other architecture than x86_64")
endif()
##########################################################################
# Additional compiler flags
##########################################################################
add_definitions("-Wall -Wextra")
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
  cudd/cuddCdd.c
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

set(CUDD_LIBS cudd util st mpr epd)
##########################################################################
# Cdds examples
##########################################################################
include_directories("${CMAKE_SOURCE_DIR}")
add_executable(testcdd tests/testcdd.c)
target_link_libraries(testcdd m ${CUDD_LIBS})

add_executable(testobj
  tests/testcdd.cpp
  cdd-obj/cdd.cpp
  cdd-obj/cdd.hh
)
target_link_libraries(testobj ${CUDD_LIBS})
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
  ExternalProject_Add(GECODE_INSTALL
    SVN_REPOSITORY https://svn.gecode.org/svn/gecode/tags/release-3.5.0
    SVN_USERNAME anonymous
    SVN_PASSWORD user@gmail.com
    CONFIGURE_COMMAND ./configure --with-vis=${CMAKE_SOURCE_DIR}/cprel/cprel.vis --disable-examples --prefix=${CMAKE_INSTALL_PREFIX}
    BUILD_IN_SOURCE Yes
    BUILD_COMMAND make -j3
    INSTALL_COMMAND make install
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
  cprel/cprel.hh
  cprel/main.cpp
)
add_library(rel ${CPREL_SRCS})
target_link_libraries(rel ${Gecode_LIBRARIES})
##########################################################################
# Executables
##########################################################################

add_executable(minimal tests/minimal.cpp)
target_link_libraries(minimal rel ${Gecode_LIBRARIES})


add_executable(bdd tests/bdd.cpp)
target_link_libraries(bdd ${CUDD_LIBS})
