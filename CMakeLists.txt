project(interview_fcgi_net_client)

include(ExternalProject)

cmake_minimum_required(VERSION 2.6)

add_definitions(-std=c++11 -Wall -Wextra)

set(DEPS_INSTALL_DIR ${CMAKE_BINARY_DIR}/3rd)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}/3rd/cereal/include
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
  dbus-cxx
  SVN_REPOSITORY svn://svn.code.sf.net/p/dbus-cxx/code/trunk/dbus-cxx
  CONFIGURE_COMMAND ./autogen.sh && ./configure --prefix=${DEPS_INSTALL_DIR}
  BUILD_COMMAND test -e stamp || make && echo > stamp
  UPDATE_DISCONNECTED 1
  BUILD_IN_SOURCE 1
)

include_directories(${DEPS_INSTALL_DIR}/include)
link_directories(${DEPS_INSTALL_DIR}/lib)

add_subdirectory(src)
add_subdirectory(tests)
