cmake_minimum_required(VERSION 3.14)
project(mesi_cachesim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(mesi_cachesim SHARED common.cc data.cc cache_line.cc cache_set.cc
                                 set_associative_cache.cc fake_memory.cc)
target_compile_options(mesi_cachesim INTERFACE "-fsized-deallocation")
