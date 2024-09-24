cmake_minimum_required(VERSION 3.20)

project(structopt)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
message(${SRC_DIR})

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${SRC_DIR})

