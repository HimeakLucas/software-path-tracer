cmake_minimum_required(VERSION 3.10)

project(path_tracer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# opção para habilitar flags úteis para perf (símbolos + otimizações)
option(ENABLE_PERF "Build with flags useful for perf (RelWithDebInfo + frame pointers)" OFF)

add_executable(path_tracer main.cpp)

if(ENABLE_PERF)
  # Força RelWithDebInfo (otimizações com símbolos)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
  # Flags por alvo (melhor prática)
  target_compile_options(path_tracer PRIVATE -g -O2 -fno-omit-frame-pointer)
  # normalmente não é necessário link flag especial para perf
else()
  # Debug útil durante desenvolvimento (símbolos, sem otimizações agressivas)
  target_compile_options(path_tracer PRIVATE -g -O0)
endif()
