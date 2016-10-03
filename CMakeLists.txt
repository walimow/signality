cmake_minimum_required(VERSION 2.6)
project(signality)

set (CMAKE_BUILD_TYPE Debug)

add_executable(signality main.cpp)

add_definitions ( -Wall  -std=c++14 -lstdc++ )

add_subdirectory(test)

install(TARGETS signality RUNTIME DESTINATION bin)
