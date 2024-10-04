cmake_minimum_required(VERSION 3.26)
project(lab1a)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest REQUIRED)

add_executable(lab1a main.cpp bitarray.cpp)

add_executable(tests bitarray_tests.cpp bitarray.cpp)

target_link_libraries(tests GTest::GTest GTest::Main pthread)

target_link_libraries(lab1a)