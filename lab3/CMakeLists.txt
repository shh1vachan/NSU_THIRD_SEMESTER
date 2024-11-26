cmake_minimum_required(VERSION 3.26)
project(lab1a)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest REQUIRED)

add_executable(lab3 main.cpp WAVfile.cpp exceptions.h parser.cpp converters.cpp sound_processor.cpp)

add_executable(lab3_tests tests.cpp WAVfile.cpp exceptions.h parser.cpp converters.cpp sound_processor.cpp)

target_link_libraries(lab3_tests GTest::GTest GTest::Main pthread)

target_link_libraries(lab3 pthread)
