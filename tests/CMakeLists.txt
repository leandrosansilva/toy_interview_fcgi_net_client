# tests here

add_executable(tests tests.cpp)
add_dependencies(tests googletest)
target_link_libraries(tests -lgtest -lgmock -pthread)
