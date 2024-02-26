cmake_minimum_required(VERSION 3.14)
project(mesi_cachesim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
    set_associative_cache.cc)

target_compile_options(mesi_cachesim INTERFACE "-fsized-deallocation")
