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

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src mesi_cachesim)

# test_data
add_executable(test_data test_data.cc)

target_include_directories(test_data PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_data PRIVATE mesi_cachesim)

add_test(
    test_data_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_data)

set_tests_properties(test_data_build PROPERTIES FIXTURES_SETUP test_fixture)

add_test(NAME test_data COMMAND ./test_data test_fixture)
set_tests_properties(test_data PROPERTIES FIXTURES_SETUP test_fixture)

# test_cache_line
add_executable(test_cache_line test_cache_line.cc)

target_include_directories(test_cache_line
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_cache_line PRIVATE mesi_cachesim)

add_test(
    test_cache_line_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_cache_line)
set_tests_properties(test_cache_line_build PROPERTIES FIXTURES_SETUP
                                                      test_fixture)

add_test(NAME test_cache_line COMMAND ./test_cache_line)
set_tests_properties(test_cache_line PROPERTIES FIXTURES_SETUP test_fixture)

# test_cache_set
add_executable(test_cache_set test_cache_set.cc)

target_include_directories(test_cache_set
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_cache_set PRIVATE mesi_cachesim)

add_test(
    test_cache_set_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_cache_set)
set_tests_properties(test_cache_set_build PROPERTIES FIXTURES_SETUP
                                                     test_fixture)

add_test(NAME test_cache_set COMMAND ./test_cache_set)
set_tests_properties(test_cache_set PROPERTIES FIXTURES_SETUP test_fixture)

# test_set_associative_cache
add_executable(test_set_associative_cache test_set_associative_cache.cc)

target_include_directories(test_set_associative_cache
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_set_associative_cache PRIVATE mesi_cachesim)

add_test(
    test_set_associative_cache_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_set_associative_cache)

set_tests_properties(test_set_associative_cache_build PROPERTIES FIXTURES_SETUP
                                                                 test_fixture)

add_test(NAME test_set_associative_cache COMMAND ./test_set_associative_cache
                                                 test_fixture)

set_tests_properties(test_set_associative_cache PROPERTIES FIXTURES_SETUP
                                                           test_fixture)

# test_fake_memory
add_executable(test_fake_memory test_fake_memory.cc)

target_include_directories(test_fake_memory
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_fake_memory PRIVATE mesi_cachesim)

add_test(
    test_fake_memory_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_fake_memory)

set_tests_properties(test_fake_memory_build PROPERTIES FIXTURES_SETUP
                                                       test_fixture)

add_test(NAME test_fake_memory COMMAND ./test_fake_memory test_fixture)

set_tests_properties(test_fake_memory PROPERTIES FIXTURES_SETUP test_fixture)
