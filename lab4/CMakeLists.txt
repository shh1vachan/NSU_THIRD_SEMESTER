cmake_minimum_required(VERSION 3.26)
project(lab1a)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest REQUIRED)

add_executable(lab4 main.cpp csv_parser.cpp)

add_executable(lab4_tests tests.cpp csv_parser.cpp)

target_link_libraries(lab4_tests GTest::GTest GTest::Main pthread)

target_link_libraries(lab4 pthread)
