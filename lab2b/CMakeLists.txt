cmake_minimum_required(VERSION 3.26)
project(lab1a)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest REQUIRED)

add_executable(lab2b main.cpp game.cpp cell.cpp parser.cpp file_manager.cpp)

add_executable(GOL_tests main_functional.cpp game.cpp cell.cpp parser.cpp file_manager.cpp)

target_link_libraries(GOL_tests GTest::GTest GTest::Main pthread)

target_link_libraries(lab2b pthread)