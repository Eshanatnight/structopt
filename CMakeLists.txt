cmake_minimum_required(VERSION 3.12)

project(structopt VERSION 0.2)

set(CMAKE_CXX_STANDARD 17)

option(STRUCTOPT_TESTS "Build structopt tests + enable CTest")
option(STRUCTOPT_EXAMPLES "Build structopt examples")

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if(STRUCTOPT_EXAMPLES)
  add_subdirectory(examples)
endif()

if(STRUCTOPT_TESTS)
  add_subdirectory(tests)
endif()