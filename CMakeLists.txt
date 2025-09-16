#---------------------------------------------------------------------------------------------------
# CMake Build Configuration for the Ray Tracing Weekend Series
# A modified version of the CMakeLists.txt provided by the author of the series
# https://github.com/RayTracing/raytracing.github.io/blob/release/CMakeLists.txt
#---------------------------------------------------------------------------------------------------

cmake_minimum_required ( VERSION 3.1.0...4.0.0 )

project(RayTracingOneWeekend)

# Set to C++11
set ( CMAKE_CXX_STANDARD          11 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

# To be extended as more source files are added
set ( SOURCE_ONE_WEEKEND
  src/InOneWeekend/main.cpp
  src/vec3.cpp
  src/color.cpp
  src/ray.cpp
  src/sphere.cpp
  src/hittable.cpp
  src/hittable_list.cpp
  src/interval.cpp
  src/camera.cpp
)

# Specific compiler flags below. We're not going to add options for all possible compilers, but if
# you're new to CMake (like we are), the following may be a helpful example if you're using a
# different compiler or want to set different compiler options.

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # This way we could set compiler flags
  #set(CMAKE_CXX_FLAGS_RELEASE "/Ob0 /Od")
  add_compile_options("/W4")      # Enable level-4 warnings
  add_compile_options("/WX")      # Treat warning as error
  add_compile_options("/we 4265") # Class has virtual functions, but its non-trivial destructor is not virtual
  add_compile_options("/we 5204") # Class has virtual functions, but its trivial destructor is not virtual
  add_compile_options("/wd 4100") # Unreferenced formal parameter
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(-Wnon-virtual-dtor)    # Class has virtual functions, but its destructor is not virtual
  add_compile_options(-Wreorder)             # Data member will be initialized after [other] data member
  add_compile_options(-Wmaybe-uninitialized) # Variable improperly initialized
  add_compile_options(-Wunused-variable)     # Variable is defined but unused
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wnon-virtual-dtor)        # Class has virtual functions, but its destructor is not virtual
  add_compile_options(-Wreorder)                 # Data member will be initialized after [other] data member
  add_compile_options(-Wsometimes-uninitialized) # Variable improperly initialized
  add_compile_options(-Wunused-variable)         # Variable is defined but unused
endif()

message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message (STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message (STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})

# Executable
add_executable(inOneWeekend ${SOURCE_ONE_WEEKEND})

# Include
target_include_directories(inOneWeekend PRIVATE include)