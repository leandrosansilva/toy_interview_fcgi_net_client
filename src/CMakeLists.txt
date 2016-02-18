# something here

add_subdirectory(dto)
add_subdirectory(backend)

add_executable(main main.cpp)
add_dependencies(main googletest dbus-cpp fcgicc)
target_link_libraries(main -ldbus-c++-1 -lfcgicc -lgmock -pthread)
