cmake_minimum_required(VERSION 3.10)
project(nuoya_sdk)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../rs232)

set(SRC main.cpp nuoyaSDK.c)

if (WIN32)
    list(APPEND SRC ../rs232/rs232-win.c)
else()
    list(APPEND SRC ../rs232/rs232-linux.c)
endif()

add_library(nuoya_sdk STATIC ${SRC})
# add_library(nuoya_sdk SHARED ${SRC})
add_executable(main ${SRC})
