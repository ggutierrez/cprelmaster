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