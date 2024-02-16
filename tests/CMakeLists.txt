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

# test_cache_line
add_executable(test_cache_line test_cache_line.cc ../src/cache_line.cc)

target_include_directories(test_cache_line
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_test(
    test_cache_line_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_cache_line)

add_test(NAME test_cache_line COMMAND ./test_cache_entry)
set_tests_properties(test_cache_line PROPERTIES FIXTURES_SETUP test_fixture)
set_tests_properties(test_cache_line PROPERTIES FIXTURES_SETUP test_fixture)

# test_set_associative_cache
add_executable(test_set_associative_cache test_set_associative_cache.cc
                                          ../src/set_associative_cache.cc)

target_include_directories(test_set_associative_cache
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_test(
    test_set_associative_cache_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_set_associative_cache)

add_test(NAME test_set_associative_cache COMMAND ./test_cache_entry)
set_tests_properties(test_set_associative_cache PROPERTIES FIXTURES_SETUP
                                                           test_fixture)
set_tests_properties(test_set_associative_cache PROPERTIES FIXTURES_SETUP
                                                           test_fixture)
