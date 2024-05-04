cmake_minimum_required(VERSION 3.14)
project(kachesim CXX)

set(CMAKE_CXX_STANDARD 20)
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
    kachesim SHARED
    src/common.cc
    src/data.cc
    src/cache_block.cc
    src/cache_set.cc
    src/replacement_policy/replacement_policy.cc
    src/replacement_policy/least_recently_used.cc
    src/data_storage.cc
    src/data_storage_transaction.cc
    src/memory_interface.cc
    src/memory_hierarchy.cc
    src/fake_memory.cc
    src/cache_interface.cc
    src/set_associative_cache.cc
    src/memory_hierarchy.cc)

target_link_libraries(kachesim PUBLIC yaml-cpp::yaml-cpp)
target_compile_options(kachesim INTERFACE "-fsized-deallocation")
