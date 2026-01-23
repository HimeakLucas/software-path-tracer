cmake_minimum_required(VERSION 3.10)

project(path_tracer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

option(ENABLE_PERF "Build with flags useful for perf (ReleaseWithDebInfo + frame pointers)" OFF)

add_executable(path_tracer main.cpp renderer.cpp)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()


target_compile_options(path_tracer PRIVATE
	$<$<CONFIG:Debug>:-g>
	$<$<CONFIG:Debug>:-O0>
	$<$<CONFIG:Debug>:-fno-omit-frame-pointer>
	$<$<CONFIG:Debug>:-fno-inline>

	$<$<CONFIG:ReleaseWithDebInfo>:-g>
	$<$<CONFIG:ReleaseWithDebInfo>:-O2>
	$<$<CONFIG:ReleaseWithDebInfo>:-fno-omit-frame-pointer>
	$<$<CONFIG:ReleaseWithDebInfo>:-g3>

	$<$<CONFIG:Release>:-O3>

	$<$<CONFIG:Profile>:-g>
	$<$<CONFIG:Profile>:-O2>
	$<$<CONFIG:Profile>:-fno-omit-frame-pointer>
	$<$<CONFIG:Profile>:-fno-inline>
)

if(ENABLE_PERF)
	message(STATUS "ENABLE_PERF=ON: forcing ReleaseWithDebInfo and adding perf flags")
	set(CMAKE_BUILD_TYPE ReleaseWithDebInfo CACHE STRING "Build type" FORCE)
	target_compile_options(path_tracer PRIVATE	
		$<$<CONFIG:ReleaseWithDebInfo>:-march=native>
		$<$<CONFIG:ReleaseWithDebInfo>:-funroll-loops>
	)
endif()
