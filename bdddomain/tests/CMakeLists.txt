add_executable(tuple tuple.cpp)
target_link_libraries(tuple bdddomain)

add_executable(tuple-variadic tuple-variadic.cpp)
target_link_libraries(tuple-variadic bdddomain)

add_executable(relation relation.cpp)
target_link_libraries(relation bdddomain)
