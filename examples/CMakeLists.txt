cmake_minimum_required(VERSION 3.12)

project(structopt VERSION 0.2)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_executable(demo demo.cpp)
target_include_directories(demo PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(positional_arguments positional_arguments.cpp)
target_include_directories(positional_arguments PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(optional_arguments optional_arguments.cpp)
target_include_directories(optional_arguments PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(flag_arguments flag_arguments.cpp)
target_include_directories(flag_arguments PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(positional_and_optional_arguments positional_and_optional_arguments.cpp)
target_include_directories(positional_and_optional_arguments PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(enum_class_argument enum_class_argument.cpp)
target_include_directories(enum_class_argument PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(remaining_arguments remaining_arguments.cpp)
target_include_directories(remaining_arguments PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(compound_arguments compound_arguments.cpp)
target_include_directories(compound_arguments PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(positional_and_compound_arguments positional_and_compound_arguments.cpp)
target_include_directories(positional_and_compound_arguments PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(integer_literals integer_literals.cpp)
target_include_directories(integer_literals PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(float_literals float_literals.cpp)
target_include_directories(float_literals PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(double_dash_argument double_dash_argument.cpp)
target_include_directories(double_dash_argument PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(pair_argument pair_argument.cpp)
target_include_directories(pair_argument PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(tuple_arguments tuple_arguments.cpp)
target_include_directories(tuple_arguments PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(nested_structures nested_structures.cpp)
target_include_directories(nested_structures PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(nested_structures_2 nested_structures_2.cpp)
target_include_directories(nested_structures_2 PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(printing_help printing_help.cpp)
target_include_directories(printing_help PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(printing_help_custom printing_help_custom.cpp)
target_include_directories(printing_help_custom PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(option_delimiters option_delimiters.cpp)
target_include_directories(option_delimiters PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(3d_array_posi 3d_array_positional.cpp)
target_include_directories(3d_array_posi PRIVATE ${CMAKE_SOURCE_DIR}/src)
