cmake_minimum_required(VERSION 3.14)
project(mesi_cachesim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 0.8.0)
FetchContent_GetProperties(yaml-cpp)

if(NOT yaml-cpp_POPULATED)
    message(STATUS "Fetching yaml-cpp...")
    FetchContent_Populate(yaml-cpp)
    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

add_library(
    mesi_cachesim SHARED
    common.cc
    data.cc
    cache_line.cc
    cache_set.cc
    replacement_policy/replacement_policy.cc
    replacement_policy/least_recently_used.cc
    data_storage.cc
    memory_interface.cc
    fake_memory.cc
    cache_interface.cc
    set_associative_cache.cc
    memory_hierarchy.cc)

target_link_libraries(mesi_cachesim PUBLIC yaml-cpp::yaml-cpp)

target_compile_options(mesi_cachesim INTERFACE "-fsized-deallocation")
