
set(EXAMPLES
  equality
  and
  projection
  permutation
  join
)

foreach(input ${EXAMPLES})
  add_executable(${input} ${input}.cpp)
  target_link_libraries(${input} gecodecprel ${Gecode_LIBRARIES})
endforeach()


# test for the equality propagator

set(OPERATIONS 
  follow
  proj
  swap
  join
  domain
  visitor
  grel
)

foreach(input ${OPERATIONS})
  add_executable(${input}-test ${input}-test.cpp)
  target_link_libraries(${input}-test gecodecprel ${Gecode_LIBRARIES})
endforeach()

add_executable(minimal minimal.cpp)
target_link_libraries(minimal gecodecprel ${Gecode_LIBRARIES} ${Boost_LIBRARIES})

add_executable(cardinality cardinality.cpp)
target_link_libraries(cardinality gecodecprel ${Gecode_LIBRARIES} ${Boost_LIBRARIES})

