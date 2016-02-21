# tests here

add_executable(tests tests.cpp)
add_dependencies(tests googletest)
target_link_libraries(tests -lgtest -lgmock -pthread)

add_executable(nm_test nm_test.cpp)
add_dependencies(nm_test dbus-cpp)
target_link_libraries(nm_test ${DBUS_CPP_LIBS} -pthread)

add_test(RequestsTests tests)
