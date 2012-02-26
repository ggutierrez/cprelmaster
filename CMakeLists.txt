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
find_package(Compiler)
##########################################################################
# System information
##########################################################################
find_package(System)
##########################################################################
# Boost
##########################################################################
set(BOOST_COMPONENTS)
if(UNIT_TEST)
  message(STATUS "Building unit tests")
  list(APPEND BOOST_COMPONENTS unit_test_framework)
endif()
list(APPEND BOOST_COMPONENTS system)
list(APPEND BOOST_COMPONENTS chrono)
find_package(BoostOrExternal)

#
# Dependency checking ends here
#

##########################################################################
# Domain representation using BDDs
##########################################################################
include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(bdddomain)
##########################################################################
# Gecode
##########################################################################
find_package(GecodeOrExternal)
##########################################################################
# Relation constraint system
##########################################################################
add_subdirectory(cprel)
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

