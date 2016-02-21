project(interview_fcgi_net_client)

include(ExternalProject)

cmake_minimum_required(VERSION 2.6)

add_definitions(-std=c++11 -Wall -Wextra -DDBUS_API_SUBJECT_TO_CHANGE)

set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR}/3rd)

enable_testing()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/3rd/cereal/include
  ${DEPS_INSTALL_DIR}/include/dbus-c++-1 
)

execute_process(COMMAND git submodule update --init --recursive)

ExternalProject_Add(
  googletest
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/googletest
  CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
)

ExternalProject_Add(
  fcgicc
  URL ${CMAKE_SOURCE_DIR}/3rd/fcgicc-0.1.3.tar.bz2
  CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=Debug
)

set(DBUS_CPP_LIBS -ldbus-c++-1 -ldbus-1)

ExternalProject_Add(
  dbus-cpp
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/dbus-cplusplus
  CONFIGURE_COMMAND 
    ./autogen.sh && ./configure --prefix=${DEPS_INSTALL_DIR} --disable-shared --enable-static
    --enable-debug
  BUILD_IN_SOURCE 1
)

add_custom_command(
  OUTPUT dbus-nm-stubs-generated.txt
  COMMAND 
    ${CMAKE_SOURCE_DIR}/tools/generate_dbus_stubs.sh 
    ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/3rd/network_manager_introspection_data/
    nm
  DEPENDS dbus-cpp
)

add_custom_target(dbus-nm-stubs-generated ALL DEPENDS dbus-nm-stubs-generated.txt)

include_directories(${DEPS_INSTALL_DIR}/include)
link_directories(${DEPS_INSTALL_DIR}/lib)

add_subdirectory(src)
add_subdirectory(tests)
