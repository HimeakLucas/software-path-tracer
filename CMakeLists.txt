cmake_minimum_required(VERSION 3.10)
project(path_tracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_PERF "Build with flags useful for perf (RelWithDebInfo + frame pointers)" OFF)

add_executable(path_tracer main.cpp renderer.cpp)

# default build type if not provided
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# basic flags per configuration
target_compile_options(path_tracer PRIVATE
  $<$<CONFIG:Debug>:-g -O0 -fno-omit-frame-pointer -fno-inline>
  $<$<CONFIG:RelWithDebInfo>:-g -O2 -fno-omit-frame-pointer -g3>
  $<$<CONFIG:Release>:-O3>
)

# extra flags para perf quando a opção estiver ativa
if(ENABLE_PERF)
  message(STATUS "ENABLE_PERF=ON: enabling perf-friendly flags and forcing RelWithDebInfo")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "" FORCE)
  target_compile_options(path_tracer PRIVATE $<$<CONFIG:RelWithDebInfo>:-funroll-loops -march=native>)
endif()

