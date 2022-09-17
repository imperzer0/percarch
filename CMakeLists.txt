cmake_minimum_required(VERSION 3.23)
project(percarch)

set(CMAKE_CXX_STANDARD 20)

if (DEFINED PACKAGE_VERSION)
    add_compile_definitions(VERSION="${PACKAGE_VERSION}")
endif ()

if (DEFINED APPNAME)
    add_compile_definitions(APPNAME="${APPNAME}")
endif ()

if (NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    message("Disabled optimization flags for use in profiler.")
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
    message("Enabled optimization compiler flags.")
endif ()

message(" [ compiler ]  ${CMAKE_CXX_COMPILER}_${CMAKE_CXX_STANDARD} ${CMAKE_CXX_FLAGS}")

set(CMAKE_VERBOSE_MAKEFILE on)

add_executable(percarch main.cpp percarch.cpp layer.cpp matrix.cpp perceptron.cpp)
