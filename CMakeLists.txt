cmake_minimum_required(VERSION 3.28)
project(OS)

set(CMAKE_CXX_STANDARD 17)

add_executable(writer lab-6/1/writer.cpp)
add_executable(reader lab-6/1/reader.cpp
        lab-6/2/pipe.cpp)

