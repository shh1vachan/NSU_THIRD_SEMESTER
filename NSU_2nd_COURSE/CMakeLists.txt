cmake_minimum_required(VERSION 3.26)
project(lab0)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab0 main.cpp
        data_writer.cpp
        data_writer.h
        file_processing.cpp
        file_processing.h)
