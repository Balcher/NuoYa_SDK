cmake_minimum_required(VERSION 3.10)
project(nuoya_sdk)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(main main.cpp)
target_link_libraries(main ${CMAKE_CURRENT_SOURCE_DIR}/libnuoya_sdk.so)
