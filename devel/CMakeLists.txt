cmake_minimum_required(VERSION 3.14)
project(c_project_template)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src mesi_cachesim)

add_executable(
    main
    ${PROJECT_SOURCE_DIR}/main.cc
    ${PROJECT_SOURCE_DIR}/../src/replacement_policy/replacement_policy.cc
    ${PROJECT_SOURCE_DIR}/../src/replacement_policy/least_recently_used.cc)
# target_link_libraries(main PRIVATE mesi_cachesim)
