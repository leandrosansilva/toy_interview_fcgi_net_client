# something here

add_subdirectory(dto)
add_subdirectory(backend)

add_executable(main main.cpp)
add_dependencies(main googletest dbus-cpp fcgicc dbus-nm-stubs-generated)
target_link_libraries(main ${DBUS_CPP_LIBS} -lfcgicc -lgmock -pthread)
