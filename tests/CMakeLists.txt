cmake_minimum_required(VERSION 3.14)
project(test_mesi_cachesim CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    CACHE STRING "" FORCE)

# generate debug symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# test_cache_entry
add_executable(test_cache_entry test_cache_entry.cc)

target_include_directories(test_cache_entry
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_test(
    test_cache_entry_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_cache_entry)

add_test(NAME test_cache_entry COMMAND ./test_cache_entry)
set_tests_properties(test_cache_entry PROPERTIES FIXTURES_SETUP test_fixture)
set_tests_properties(test_cache_entry PROPERTIES FIXTURES_SETUP test_fixture)
