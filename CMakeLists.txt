# https://cmake.org/cmake-tutorial/

cmake_minimum_required (VERSION 2.6)
project (single-control)
# The version number.
set (single-control_VERSION_MAJOR 0)
set (single-control_VERSION_MINOR 1)

add_executable(single-control main.cpp server.cpp logging.cpp)
