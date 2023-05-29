cmake_minimum_required(VERSION 3.0.0)
project(Zilla VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(Zilla main.cpp)

set_property(TARGET Zilla PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
