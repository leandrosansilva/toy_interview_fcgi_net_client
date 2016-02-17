# something here

add_subdirectory(dto)
add_subdirectory(backend)

add_executable(main main.cpp)
add_dependencies(main googletest dbus-cxx fcgicc)
target_link_libraries(main libdbus-cxx.a -lfcgicc -lgmock -pthread)
