cmake_minimum_required(VERSION 3.14)
project(test_mesi_cachesim CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT DEFINED DEBUG)
    set(DEBUG 0)
endif()
add_definitions(-DDEBUG=${DEBUG})

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

# test_data_storage_transaction
add_executable(test_data_storage_transaction test_data_storage_transaction.cc)

target_include_directories(test_data_storage_transaction
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_data_storage_transaction PRIVATE mesi_cachesim)

add_test(
    test_data_storage_transaction_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_data_storage_transaction)

set_tests_properties(test_data_storage_transaction_build
                     PROPERTIES FIXTURES_SETUP test_fixture)

add_test(NAME test_data_storage_transaction
         COMMAND ./test_data_storage_transaction test_fixture)
set_tests_properties(test_data_storage_transaction PROPERTIES FIXTURES_SETUP
                                                              test_fixture)

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

# test_doubly_linked_list
add_executable(test_doubly_linked_list test_doubly_linked_list.cc)

target_include_directories(test_doubly_linked_list
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_doubly_linked_list PRIVATE mesi_cachesim)

add_test(
    test_doubly_linked_list_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_doubly_linked_list)
set_tests_properties(test_doubly_linked_list_build PROPERTIES FIXTURES_SETUP
                                                              test_fixture)

add_test(NAME test_doubly_linked_list COMMAND ./test_doubly_linked_list)
set_tests_properties(test_doubly_linked_list PROPERTIES FIXTURES_SETUP
                                                        test_fixture)

# test_least_recently_used
add_executable(test_least_recently_used test_least_recently_used.cc)

target_include_directories(test_least_recently_used
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
target_link_libraries(test_least_recently_used PRIVATE mesi_cachesim)

add_test(
    test_least_recently_used_build
    "${CMAKE_COMMAND}"
    --build
    "${CMAKE_BINARY_DIR}"
    --config
    "$<CONFIG>"
    --target
    test_least_recently_used)
set_tests_properties(test_least_recently_used_build PROPERTIES FIXTURES_SETUP
                                                               test_fixture)

add_test(NAME test_least_recently_used COMMAND ./test_least_recently_used)
set_tests_properties(test_least_recently_used PROPERTIES FIXTURES_SETUP
                                                         test_fixture)

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
