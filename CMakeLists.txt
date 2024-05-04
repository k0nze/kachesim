cmake_minimum_required(VERSION 3.14)
project(kachesim CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
    src/fake_memory.cc
    src/cache_interface.cc
    src/set_associative_cache.cc
    src/memory_hierarchy.cc)

target_compile_options(kachesim INTERFACE "-fsized-deallocation")
