set(IS_64BITS No)

#message(STATUS "Void pointer size ${CMAKE_SIZEOF_VOID_P}")

include(CheckTypeSize)
check_type_size("void*" SIZEOF_VOID_P)
if(SIZEOF_VOID_P STREQUAL "")
  message(SEND_ERROR "Failed to determine sizeof(void*)")
endif()

#message(STATUS "Size of pointer to void (compiler): ${SIZEOF_VOID_P}")
if(SIZEOF_VOID_P MATCHES "8")
  set(IS_64BITS Yes)
endif()


if (IS_64BITS)
  message(STATUS "Building with 64 bits support")
else()
  message(STATUS "Building with 32 bits support")
endif()