cmake_minimum_required(VERSION 3.14)
project(c_project_template)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main ${PROJECT_SOURCE_DIR}/src/main.cc)
