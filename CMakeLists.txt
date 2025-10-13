#---------------------------------------------------------------------------------------------------
# CMake Build Configuration for the Ray Tracing Weekend Series
# A heavily modified version of the CMakeLists.txt provided by the author of the series
# Includes Catch2 testing framework
# https://github.com/RayTracing/raytracing.github.io/blob/release/CMakeLists.txt
#---------------------------------------------------------------------------------------------------

cmake_minimum_required ( VERSION 3.1.0...4.0.0 )

project(RayTracingOneWeekend LANGUAGES CXX)

# Set to C++23
set ( CMAKE_CXX_STANDARD          23 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

# === Build ===

# Make raytracer a library that will be linked to tests
add_library(raytracer_lib
  src/camera.cpp
  src/color.cpp
  src/hittable_list.cpp
  src/hittable.cpp
  src/material.cpp
  src/interval.cpp
  src/ray.cpp
  src/sphere.cpp
  src/timer.cpp
  src/vec3.cpp
)

# Include headers
target_include_directories(raytracer_lib PUBLIC include)

# Executable
add_executable(inOneWeekend
  src/InOneWeekend/main.cpp
)

# Link library to main
target_link_libraries(inOneWeekend PRIVATE raytracer_lib)

# Only enable compiler options for raytracer_lib
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # This way we could set compiler flags for different configs
  #set(CMAKE_CXX_FLAGS_RELEASE "/Ob0 /Od")
  target_compile_options(raytracer_lib PRIVATE
    "/W4"      # Enable level-4 warnings
    "/WX"      # Treat warning as error
    "/we 4265" # Class has virtual functions, but its non-trivial destructor is not virtual
    "/we 5204" # Class has virtual functions, but its trivial destructor is not virtual
    "/wd 4100" # Unreferenced formal parameter
  )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(raytracer_lib PRIVATE
    -Wnon-virtual-dtor    # Class has virtual functions, but its destructor is not virtual
    -Wreorder             # Data member will be initialized after [other] data member
    -Wmaybe-uninitialized # Variable improperly initialized
    -Wunused-variable     # Variable is defined but unused
  )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options(raytracer_lib PRIVATE
    -Wnon-virtual-dtor        # Class has virtual functions, but its destructor is not virtual
    -Wreorder                 # Data member will be initialized after [other] data member
    -Wsometimes-uninitialized # Variable improperly initialized
    -Wunused-variable         # Variable is defined but unused
  )
endif()

message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message (STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message (STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})



# === Tests ===

# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests
    tests/test.cpp
)

# Link raytracer_lib and Catch2 to tests
target_link_libraries(tests PRIVATE
  raytracer_lib
  Catch2::Catch2WithMain
)

# ! This is not needed atm as compiler options only for raytracer_lib !
# Suppress warnings for tests
# Catch2 triggers some warnings with compiler options above
# if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#   target_compile_options(tests PRIVATE "/w")
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   target_compile_options(tests PRIVATE -w)
# endif()

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)
