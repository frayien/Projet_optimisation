cmake_minimum_required(VERSION 3.0.0)
project(Project_opti VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")

include_directories(/mingw64/include)
include_directories(include)

file(
    GLOB_RECURSE 
    SRCS 
    src/*
)

file(
    GLOB_RECURSE 
    HEADERS 
    include/*
)

add_executable(Project_opti ${HEADERS} ${SRCS})

target_link_libraries(
    Project_opti
    glpk
)