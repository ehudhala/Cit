cmake_minimum_required(VERSION 3.4)
project(cit)

set(CMAKE_CXX_STANDARD 14)

include(GoogleTest)
include(CTest)

add_subdirectory(src)
