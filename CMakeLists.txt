cmake_minimum_required(VERSION 3.4)
project(PointsToAnalysis VERSION 0.0.1 LANGUAGES CXX)

# we need LLVM_HOME in order not automatically set LLVM_DIR
if(NOT DEFINED ENV{LLVM_HOME})
    message(FATAL_ERROR "$LLVM_HOME is not defined")
else ()
    set(ENV{LLVM_DIR} $ENV{LLVM_HOME}/lib/cmake/llvm)
endif()

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS} include)
link_directories(${LLVM_LIBRARY_DIRS})

include_directories("/usr/local/include/spatial")

add_library(Spatial SHARED IMPORTED)
set_target_properties(Spatial PROPERTIES IMPORTED_LOCATION "/usr/local/lib/libSpatial.so")

add_subdirectory(lib)
add_subdirectory(src)
