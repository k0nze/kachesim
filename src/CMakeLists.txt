cmake_minimum_required(VERSION 3.14)
project(kachesim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(
    kachesim SHARED
    common.cc
    data.cc
    cache_block.cc
    cache_set.cc
    replacement_policy/replacement_policy.cc
    replacement_policy/least_recently_used.cc
    data_storage.cc
    memory_interface.cc
    fake_memory.cc
    cache_interface.cc
    set_associative_cache.cc)

target_compile_options(kachesim INTERFACE "-fsized-deallocation")
