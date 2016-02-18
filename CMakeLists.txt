project(interview_fcgi_net_client)

include(ExternalProject)

cmake_minimum_required(VERSION 2.6)

add_definitions(-std=c++11 -Wall -Wextra -DDBUS_API_SUBJECT_TO_CHANGE)

set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR}/3rd)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/3rd/cereal/include
  ${DEPS_INSTALL_DIR}/lib/include/dbus-c++-1 
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
  URL http://althenia.net/files/fcgicc-0.1.3.tar.bz2
  CMAKE_ARGS 
    -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR}
)

ExternalProject_Add(
  dbus-cpp
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/dbus-cplusplus
  CONFIGURE_COMMAND ./autogen.sh && ./configure --prefix=${DEPS_INSTALL_DIR} --disable-shared --enable-shared
  BUILD_IN_SOURCE 1
)

include_directories(${DEPS_INSTALL_DIR}/include)
link_directories(${DEPS_INSTALL_DIR}/lib)

add_subdirectory(src)
add_subdirectory(tests)
