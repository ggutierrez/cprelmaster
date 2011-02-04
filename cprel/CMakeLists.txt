include_directories(${CUDDCP_SOURCE_DIR}/cprel)
link_directories(${CUDDCP_BINARY_DIR}/cprel)

## Turn on the unused value warning in clang
add_definitions("-Wunused-value")

##########################################################################
## Gecode detection (only works with source installation no frameworks)
##########################################################################
find_library(GKERNEL gecodekernel)
find_library(GSUPPORT gecodesupport)
find_library(GDRIVER gecodedriver)
find_library(GINT gecodeint)
find_library(GSET gecodeset)
find_library(GSEARCH gecodesearch)
find_library(GMINM gecodeminimodel)
find_path(GKERNELHH gecode/kernel.hh)

set(GECLIBS)
if (GKERNEL AND GKERNELHH AND GSUPPORT AND GDRIVER AND GINT AND GSET AND
    GSEARCH AND GMINM)
  include_directories(${GKERNELHH})
  set(GECLIBS ${GKERNEL} ${GSUPPORT} ${GDRIVER} ${GINT} ${GSET} ${GSEARCH}
      ${GMINM})
else()
  message(FATAL_ERROR "--gecode kernel: ${GKERNEL}")
  message(FATAL_ERROR "---------support: ${GSUPPORT}")
  message(FATAL_ERROR "---------int: ${GINT}")
  message(FATAL_ERROR "---------set: ${GSET}")
  message(FATAL_ERROR "---------search: ${GSEARCH}")
  message(FATAL_ERROR "---------driver: ${GDRIVER}")
  message(FATAL_ERROR "---------minimodel: ${GMINM}")
  message(FATAL_ERROR "---------headers: ${GKERNELHH}")
endif()

message(STATUS "Building CPREL")
add_executable(queens queens.cpp)
target_link_libraries(queens ${GECLIBS})
